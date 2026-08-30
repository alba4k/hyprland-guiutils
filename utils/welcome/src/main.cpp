#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/window/Window.hpp>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprtoolkit/element/Image.hpp>
#include <hyprtoolkit/element/Button.hpp>
#include <hyprtoolkit/element/Null.hpp>
#include <hyprtoolkit/element/Combobox.hpp>

#include <hyprutils/memory/SharedPtr.hpp>
#include <hyprutils/memory/UniquePtr.hpp>
#include <hyprutils/string/VarList.hpp>
#include <hyprutils/string/String.hpp>
#include <hyprutils/os/Process.hpp>
#include "i18n/Engine.hpp"

#include <print>
#include <ranges>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <optional>

using namespace Hyprutils::Memory;
using namespace Hyprutils::Math;
using namespace Hyprutils::String;
using namespace Hyprutils::OS;
using namespace Hyprtoolkit;
using namespace std::string_literals;

#define SP  CSharedPointer
#define ASP CAtomicSharedPointer
#define WP  CWeakPointer
#define UP  CUniquePointer

constexpr const size_t                         TABS_NUMBER       = 6;
constexpr const size_t                         INNER_NULL_MARGIN = 5;

std::array<std::string, TABS_NUMBER> TITLES = {
    I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_TITLE),
    I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_GETSTART),
    I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_APPS),
    I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CONFIG),
    I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_ECOSYSTEM),
    I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_THATSIT),
};

const std::vector<const char*> TERMINALS = {
    "kitty", "alacritty", "wezterm", "foot", "konsole", "gnome-terminal",
};

const std::vector<const char*> FILE_MANAGERS = {"dolphin", "thunar", "pcmanfm", "nautilus", "nemo"};

const std::vector<const char*> LAUNCHERS = {"hyprlauncher", "fuzzel", "wofi", "rofi -show run", "anyrun", "tofi-drun --drun-launch=true"};

struct SAppState {
    std::string              name;
    std::vector<std::string> binaryNames;
    bool                     mandatory = false;
    SP<CTextElement>         labelEl;
};

static struct {
    SP<IBackend>                              backend;
    SP<CRectangleElement>                     tabContainer;
    std::array<SP<CNullElement>, TABS_NUMBER> tabs;
    SP<CTextElement>                          topText;
    SP<CRowLayoutElement>                     buttonLayout;
    SP<CNullElement>                          buttonSpacer;
    SP<CButtonElement>                        buttonBack, buttonNext, buttonQuit, buttonFinish, buttonOpenWiki, buttonLaunchTerm;
    size_t                                    tab = 0;
    std::vector<SP<SAppState>>                appStates;
    ASP<CTimer>                               appRefreshTimer, wikiOpenTimer;
} state;

static bool appExists(std::string binName) {
    static auto PATH = getenv("PATH");

    if (!PATH)
        return false;

    static CVarList paths(PATH, 0, ':', true);

    for (const auto& p : paths) {
        std::error_code ec;
        if (!std::filesystem::exists(std::filesystem::path(p) / binName, ec) || ec)
            continue;
        return true;
    }

    return false;
}

static bool appIsRunning(std::string binName) {
    // loop over /proc/ entries, check exe
    std::error_code ec_it;
    for (const std::filesystem::path& procEntry : std::filesystem::directory_iterator("/proc", ec_it)) {
        if (ec_it)
            continue;

        std::error_code ec;

        if (!std::filesystem::exists(procEntry / "exe", ec) || ec)
            continue;

        const auto CANONICAL = std::filesystem::canonical(procEntry / "exe", ec);

        if (ec)
            continue;

        if (!CANONICAL.has_filename())
            continue;

        if (CANONICAL.filename() == binName)
            return true;
    }

    return false;
}

static void updateApps() {
    if (state.tab != 1)
        return;

    state.appRefreshTimer = state.backend->addTimer(std::chrono::seconds(1), [](ASP<CTimer> t, void* d) { updateApps(); }, nullptr);

    for (const auto& a : state.appStates) {

        bool found = false;

        for (const auto& bn : a->binaryNames) {
            if (!appIsRunning(bn))
                continue;

            found = true;

            a->labelEl->rebuild()
                ->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_RUNNING, {{"type", a->name}, {"star", (a->mandatory ? "<span foreground=\"#cc2222\">*</span>" : "")}, {"name", bn}}))
                ->commence();
            break;
        }
        if (!found) {
            for (const auto& bn : a->binaryNames) {
                if (!appExists(bn))
                    continue;

                found = true;

                a->labelEl->rebuild()
                    ->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_INSTALLED, {{"type", a->name}, {"star", (a->mandatory ? "<span foreground=\"#cc2222\">*</span>" : "")}, {"name", bn}}))
                    ->commence();
                break;
            }
        }

        if (!found)
            a->labelEl->rebuild()
                ->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_MISSING, {{"type", a->name}, {"star", (a->mandatory ? "<span foreground=\"#cc2222\">*</span>" : "")}}))
                ->commence();
    }
}

static void updateTab() {
    state.tabContainer->clearChildren();
    state.tabContainer->addChild(state.tabs[state.tab]);
    state.topText->rebuild()->text(std::string(TITLES[state.tab]))->commence();
    updateApps();

    state.buttonLayout->clearChildren();

    if (state.tab == 0) {
        state.buttonLayout->addChild(state.buttonSpacer);
        state.buttonLayout->addChild(state.buttonQuit);
        state.buttonLayout->addChild(state.buttonNext);
    } else if (state.tab == 1) {
        state.buttonLayout->addChild(state.buttonBack);
        state.buttonLayout->addChild(state.buttonSpacer);
        state.buttonLayout->addChild(state.buttonLaunchTerm);
        state.buttonLayout->addChild(state.buttonNext);
    } else if (state.tab == 2) {
        state.buttonLayout->addChild(state.buttonBack);
        state.buttonLayout->addChild(state.buttonSpacer);
        state.buttonLayout->addChild(state.buttonNext);
    } else if (state.tab == 3) {
        state.buttonLayout->addChild(state.buttonBack);
        state.buttonLayout->addChild(state.buttonSpacer);
        state.buttonLayout->addChild(state.buttonOpenWiki);
        state.buttonLayout->addChild(state.buttonNext);
    } else if (state.tab == 4) {
        state.buttonLayout->addChild(state.buttonBack);
        state.buttonLayout->addChild(state.buttonSpacer);
        state.buttonLayout->addChild(state.buttonOpenWiki);
        state.buttonLayout->addChild(state.buttonNext);
    } else if (state.tab == 5) {
        state.buttonLayout->addChild(state.buttonBack);
        state.buttonLayout->addChild(state.buttonSpacer);
        state.buttonLayout->addChild(state.buttonOpenWiki);
        state.buttonLayout->addChild(state.buttonFinish);
    }
}

static void tabBack() {
    if (state.tab == 0)
        return;

    state.tab--;
    updateTab();
}

static void tabNext() {
    if (state.tab == TITLES.size() - 1)
        return;

    state.tab++;
    updateTab();
}

static void registerAppState(std::string&& name, std::vector<std::string>&& binaries, bool mandatory, const std::string& recommend = "", const std::string& note = "") {
    auto appState         = makeShared<SAppState>();
    appState->name        = std::move(name);
    appState->binaryNames = std::move(binaries);
    appState->labelEl     = CTextBuilder::begin()->color([] { return state.backend->getPalette()->m_colors.text; })->fontSize({CFontSize::HT_FONT_TEXT})->text("")->commence();
    appState->mandatory   = mandatory;

    std::string acceptedStr = "";
    for (const auto& b : appState->binaryNames) {
        acceptedStr += b + ", ";
    }
    if (!acceptedStr.empty())
        acceptedStr = acceptedStr.substr(0, acceptedStr.length() - 2);

    std::string tooltip = recommend.empty() ? I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_ACCEPTED, {{"accepted", acceptedStr}}) : I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_RECOMMENDED, {{"recommended", recommend}, {"accepted", acceptedStr}});
    if (!note.empty())
        tooltip += "\n" + note;

    appState->labelEl->setTooltip(std::move(tooltip));

    state.appStates.emplace_back(std::move(appState));
}

static SP<Hyprtoolkit::CRowLayoutElement> spaceOut(std::string&& label, SP<IElement> el) {
    auto text   = CTextBuilder::begin()->text(std::move(label))->commence();
    auto spacer = CNullBuilder::begin()->commence();
    spacer->setGrow(true, false);
    auto layout = CRowLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
    layout->addChild(text);
    layout->addChild(spacer);
    layout->addChild(el);
    return layout;
}

static std::optional<std::string> readFileAsString(const std::string& path) {
    std::error_code ec;

    if (!std::filesystem::exists(path, ec) || ec)
        return std::nullopt;

    std::ifstream file(path);
    if (!file.good())
        return std::nullopt;

    return std::string((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
}

static std::optional<std::filesystem::path> getHyprlandLuaConfigPath() {
    if (const auto XDG_CONFIG_HOME = getenv("XDG_CONFIG_HOME"); XDG_CONFIG_HOME && XDG_CONFIG_HOME[0] != '\0')
        return std::filesystem::path{XDG_CONFIG_HOME} / "hypr" / "hyprland.lua";

    if (const auto HOME = getenv("HOME"); HOME && HOME[0] != '\0')
        return std::filesystem::path{HOME} / ".config" / "hypr" / "hyprland.lua";

    return std::nullopt;
}

static bool isLuaWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r';
}

static std::string luaQuotedString(const std::string_view& str) {
    std::string result = "\"";
    result.reserve(str.length() + 2);

    for (const auto c : str) {
        switch (c) {
            case '\\': result += "\\\\"; break;
            case '"': result += "\\\""; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c; break;
        }
    }

    result += '"';
    return result;
}

static std::optional<std::pair<size_t, size_t>> findLuaLocalLine(const std::string& config, const std::string_view& var) {
    for (size_t lineStart = 0; lineStart <= config.length();) {
        size_t lineEnd = config.find('\n', lineStart);
        if (lineEnd == std::string::npos)
            lineEnd = config.length();

        const std::string_view line{config.data() + lineStart, lineEnd - lineStart};

        size_t pos = 0;
        while (pos < line.length() && isLuaWhitespace(line[pos])) {
            pos++;
        }

        if (line.substr(pos).starts_with("local")) {
            pos += 5;

            if (pos < line.length() && isLuaWhitespace(line[pos])) {
                while (pos < line.length() && isLuaWhitespace(line[pos])) {
                    pos++;
                }

                if (line.substr(pos, var.length()) == var) {
                    pos += var.length();

                    while (pos < line.length() && isLuaWhitespace(line[pos])) {
                        pos++;
                    }

                    if (pos < line.length() && line[pos] == '=')
                        return std::pair<size_t, size_t>{lineStart, lineEnd};
                }
            }
        }

        if (lineEnd == config.length())
            break;

        lineStart = lineEnd + 1;
    }

    return std::nullopt;
}

static std::string luaLocalLineReplacement(const std::string& config, const std::pair<size_t, size_t>& line, const std::string_view& var, const char* newValue) {
    size_t firstNonWhitespace = line.first;
    while (firstNonWhitespace < line.second && isLuaWhitespace(config[firstNonWhitespace])) {
        firstNonWhitespace++;
    }

    return std::format("{}local {} = {}", config.substr(line.first, firstNonWhitespace - line.first), var, luaQuotedString(newValue));
}

static bool isLuaAutogeneratedLine(const std::string_view& line) {
    const auto COMMENT_POS = line.find("--");
    const auto CODE        = line.substr(0, COMMENT_POS);

    std::string compact;
    compact.reserve(CODE.length());

    for (const auto c : CODE) {
        if (!isLuaWhitespace(c))
            compact += c;
    }

    return compact == "hl.config({autogenerated=true})" || compact == "hl.config({autogenerated=1})";
}

static std::optional<std::pair<size_t, size_t>> findLuaAutogeneratedLine(const std::string& config) {
    for (size_t lineStart = 0; lineStart <= config.length();) {
        size_t lineEnd = config.find('\n', lineStart);
        if (lineEnd == std::string::npos)
            lineEnd = config.length();

        if (isLuaAutogeneratedLine({config.data() + lineStart, lineEnd - lineStart})) {
            const size_t removeEnd = lineEnd == config.length() ? lineEnd : lineEnd + 1;
            return std::pair<size_t, size_t>{lineStart, removeEnd};
        }

        if (lineEnd == config.length())
            break;

        lineStart = lineEnd + 1;
    }

    return std::nullopt;
}

static std::optional<std::string> updateDefaultConfigVar(const std::string_view& var, const char* newValue) {
    const auto PATH = getHyprlandLuaConfigPath();
    if (!PATH)
        return I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_ERROR1);

    const auto STR = readFileAsString(PATH->string());

    if (!STR)
        return I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_ERROR2);

    std::string newConfig = *STR;

    const auto VAR_LINE = findLuaLocalLine(newConfig, var);
    if (!VAR_LINE)
        return I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_ERROR3);

    newConfig.replace(VAR_LINE->first, VAR_LINE->second - VAR_LINE->first, luaLocalLineReplacement(newConfig, *VAR_LINE, var, newValue));

    std::ofstream ofs(*PATH, std::ios::trunc);
    if (!ofs.good())
        return I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_ERROR4);

    ofs << newConfig;
    ofs.close();

    if (ofs.fail())
        return I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_ERROR5);

    return std::nullopt;
}

static void removeAutogen() {
    const auto PATH = getHyprlandLuaConfigPath();
    if (!PATH)
        return;

    const auto STR = readFileAsString(PATH->string());

    if (!STR)
        return;

    std::string newConfig = *STR;

    const auto AUTOGEN_LINE = findLuaAutogeneratedLine(newConfig);
    if (!AUTOGEN_LINE)
        return;

    newConfig.erase(AUTOGEN_LINE->first, AUTOGEN_LINE->second - AUTOGEN_LINE->first);

    std::ofstream ofs(*PATH, std::ios::trunc);
    ofs << newConfig;
    ofs.close();
}

static void initTabs() {
    {
        // Tab 1
        auto nullEl = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto layout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto text   = CTextBuilder::begin()->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CONTENT1))->color([] { return state.backend->getPalette()->m_colors.text; })->commence();
        auto spacer = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {1, 1}})->commence();
        spacer->setGrow(true);

        layout->addChild(text);
        layout->addChild(spacer);
        nullEl->addChild(layout);
        nullEl->setGrow(true);
        nullEl->setMargin(INNER_NULL_MARGIN);
        state.tabs[0] = nullEl;
    }

    {
        // Tab 2
        auto nullEl = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto layout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->gap(20)->commence();
        auto text   = CTextBuilder::begin()->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CONTENT2))->color([] { return state.backend->getPalette()->m_colors.text; })->commence();
        auto spacer = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {1, 1}})->commence();
        spacer->setGrow(true);

        layout->addChild(text);

        nullEl->addChild(layout);
        nullEl->setGrow(true);
        nullEl->setMargin(INNER_NULL_MARGIN);

        auto appLayoutParent = CRowLayoutBuilder::begin()->size(CDynamicSize{CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        SP<CColumnLayoutElement> appLayouts[2] = {
            CColumnLayoutBuilder::begin()->gap(4)->size(CDynamicSize{CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {0.5F, 1.F}})->commence(),
            CColumnLayoutBuilder::begin()->gap(4)->size(CDynamicSize{CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {0.5F, 1.F}})->commence(),
        };

        appLayouts[0]->setGrow(false, true);
        appLayouts[0]->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
        appLayouts[0]->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_LEFT, true);
        appLayouts[1]->setGrow(false, true);
        appLayouts[1]->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
        appLayouts[1]->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_LEFT, true);

        appLayoutParent->addChild(appLayouts[0]);
        appLayoutParent->addChild(appLayouts[1]);

        layout->addChild(appLayoutParent);

        // app states
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_AUTHAGENT), {"hyprpolkitagent", "polkit-kde-agent", "lxpolkit"}, true, "hyprpolkitagent");
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_FILES), {"dolphin", "ranger", "thunar", "pcmanfm", "nautilus", "nemo", "nnn", "yazi"}, true);
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_TERM), {"kitty", "alacritty", "wezterm", "foot", "konsole", "gnome-terminal"}, true, "kitty");
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_PIPEWIRE), {"pipewire", "wireplumber"}, true);
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_WALLPAPER), {"hyprpaper", "swww", "awww", "swaybg", "wpaperd"}, false, "hyprpaper");
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_PORTAL), {"xdg-desktop-portal-hyprland"}, true);
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_NOTIF), {"dunst", "mako", "swaync"}, true, "", I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_NOTIFNOTE));
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_SHELL), {"quickshell", "waybar", "eww", "ags"}, false, "", I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_SHELLNOTE));
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_LAUNCHER), {"hyprlauncher", "fuzzel", "wofi", "rofi", "anyrun", "vicinae-server", "walker", "tofi"}, false, "hyprlauncher");
        registerAppState(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CLIPBOARD), {"wl-copy"}, true, "", I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CLIPBOARDNOTE));

        // register them
        bool flip = false;
        for (const auto& e : state.appStates) {
            appLayouts[flip ? 1 : 0]->addChild(e->labelEl);
            flip = !flip;
        }

        state.tabs[1] = nullEl;
    }

    {
        // Tab 3
        auto nullEl = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto layout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->gap(4)->commence();
        auto text   = CTextBuilder::begin()->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CONTENT3))->color([] { return state.backend->getPalette()->m_colors.text; })->commence();
        auto spacer = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {1, 1}})->commence();
        auto hr     = CRectangleBuilder::begin()
                          ->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_ABSOLUTE, {0.5F, 11.F}})
                          ->color([] { return state.backend->getPalette()->m_colors.base; })
                          ->commence();
        auto hr2    = CRectangleBuilder::begin()
                          ->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_ABSOLUTE, {0.5F, 11.F}})
                          ->color([] { return state.backend->getPalette()->m_colors.base; })
                          ->commence();

        auto defaultContainer = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {0.6F, 1.F}})->commence();
        auto defaultLayout    = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->gap(4)->commence();
        spacer->setGrow(true);
        hr->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
        hr->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_HCENTER, true);
        hr->setMargin(5);
        hr2->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
        hr2->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_HCENTER, true);
        hr2->setMargin(5);

        auto addSelector = [&](const char* name, const char* label, const auto& arr) {
            auto                     text     = CTextBuilder::begin()->text("")->color([] { return state.backend->getPalette()->m_colors.text; })->commence();
            auto                     textNull = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();

            std::vector<std::string> strs;
            strs.reserve(arr.size());
            for (const auto& t : arr) {
                strs.emplace_back(t);
            }
            text->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
            text->setPositionFlag(sc<Hyprtoolkit::IElement::ePositionFlag>(Hyprtoolkit::IElement::HT_POSITION_FLAG_VCENTER | Hyprtoolkit::IElement::HT_POSITION_FLAG_RIGHT), true);

            auto updateText = [](SP<CTextElement> textEl, const std::string_view& app, std::string err = "") -> void {
                if (!err.empty()) {
                    textEl->rebuild()->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_ERROR6, {{"error", err}}))->commence();
                    return;
                }

                const std::string_view APP_STEM = app.contains(' ') ? app.substr(0, app.find(' ')) : app;

                if (appExists(std::string{APP_STEM}))
                    textEl->rebuild()->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_APPINST, {{"name", std::string(app)}}))->commence();
                else
                    textEl->rebuild()->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_APPNOINST, {{"name", std::string(app)}}))->commence();
            };

            defaultLayout->addChild(spaceOut(label,
                                             CComboboxBuilder::begin()
                                                 ->items(std::move(strs))
                                                 ->size({CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {200, 25}})
                                                 ->onChanged([tt = text, updateText, arr, name](SP<CComboboxElement> el, size_t idx) {
                                                     const auto& TERM_NAME = arr[idx];
                                                     const auto  RESULT    = updateDefaultConfigVar(name, TERM_NAME);
                                                     if (RESULT)
                                                         updateText(tt, TERM_NAME, *RESULT);
                                                     else
                                                         updateText(tt, TERM_NAME);
                                                 })
                                                 ->commence()));
            textNull->addChild(text);
            defaultLayout->addChild(textNull);

            updateText(text, arr[0]);
        };

        addSelector("terminal", I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_TERM).c_str(), TERMINALS);
        addSelector("fileManager", I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_FILES).c_str(), FILE_MANAGERS);
        addSelector("menu", I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_LAUNCHER).c_str(), LAUNCHERS);

        defaultContainer->addChild(defaultLayout);
        layout->addChild(text);
        layout->addChild(hr);
        layout->addChild(defaultContainer);
        layout->addChild(hr);
        layout->addChild(CTextBuilder::begin()
                             ->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CHANGELATER))
                             ->color([] { return state.backend->getPalette()->m_colors.text; })
                             ->commence());
        layout->addChild(spacer);
        nullEl->addChild(layout);
        nullEl->setGrow(true);
        nullEl->setMargin(INNER_NULL_MARGIN);
        state.tabs[2] = nullEl;
    }

    {
        // Tab 4
        auto nullEl = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto layout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto text   = CTextBuilder::begin()->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CONTENT4))->color([] { return state.backend->getPalette()->m_colors.text; })->commence();
        auto spacer = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {1, 1}})->commence();
        spacer->setGrow(true);

        layout->addChild(text);
        layout->addChild(spacer);
        nullEl->addChild(layout);
        nullEl->setGrow(true);
        nullEl->setMargin(INNER_NULL_MARGIN);
        state.tabs[3] = nullEl;
    }

    {
        // Tab 5
        auto nullEl = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto layout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto text   = CTextBuilder::begin()->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CONTENT5))->color([] { return state.backend->getPalette()->m_colors.text; })->commence();
        auto spacer = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {1, 1}})->commence();
        spacer->setGrow(true);

        layout->addChild(text);
        layout->addChild(spacer);
        nullEl->addChild(layout);
        nullEl->setGrow(true);
        nullEl->setMargin(INNER_NULL_MARGIN);
        state.tabs[4] = nullEl;
    }

    {
        // Tab 6
        auto nullEl = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto layout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->commence();
        auto text   = CTextBuilder::begin()->text(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_CONTENT6))->color([] { return state.backend->getPalette()->m_colors.text; })->commence();
        auto spacer = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {1, 1}})->commence();
        spacer->setGrow(true);

        layout->addChild(text);
        layout->addChild(spacer);
        nullEl->addChild(layout);
        nullEl->setGrow(true);
        nullEl->setMargin(INNER_NULL_MARGIN);
        state.tabs[5] = nullEl;
    }
}

int main(int argc, char** argv, char** envp) {

    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            std::println(R"#(hyprland-welcome, part of hyprland-guiutils v{})#", GUIUTILS_VERSION);
            return 0;
        }

        std::print(stderr, "invalid arg {}\n", argv[i]);
        return 1;
    }

    state.backend = IBackend::create();

    const auto FONT_SIZE   = CFontSize{CFontSize::HT_FONT_TEXT}.ptSize();
    const auto WINDOW_SIZE = Vector2D{FONT_SIZE * 90.F, FONT_SIZE * 50.F};

    //
    auto window =
        CWindowBuilder::begin()->preferredSize(WINDOW_SIZE)->minSize(WINDOW_SIZE)->maxSize(WINDOW_SIZE)->appTitle(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_TITLE))->appClass("hyprland-welcome")->commence();

    initTabs();

    window->m_rootElement->addChild(CRectangleBuilder::begin()->color([] { return state.backend->getPalette()->m_colors.background; })->commence());

    auto rootLayout = CColumnLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}})->gap(10)->commence();
    rootLayout->setMargin(3);

    window->m_rootElement->addChild(rootLayout);

    // top null: title

    auto topNull = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 10}})->commence();
    topNull->setMargin(4);

    state.topText = CTextBuilder::begin()->color([] { return state.backend->getPalette()->m_colors.text; })->text(std::string(TITLES[state.tab]))->fontSize(CFontSize::HT_FONT_H2)->commence();
    state.topText->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
    state.topText->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_CENTER, true);

    topNull->addChild(state.topText);
    rootLayout->addChild(topNull);

    // // content
    state.tabContainer = CRectangleBuilder::begin()
                             ->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})
                             ->color([] { return state.backend->getPalette()->m_colors.background; })
                             ->borderThickness(1)
                             ->borderColor([] { return state.backend->getPalette()->m_colors.background.brighten(0.2F); })
                             ->rounding(state.backend->getPalette()->m_vars.smallRounding)
                             ->commence();
    state.tabContainer->setGrow(false, true);

    rootLayout->addChild(state.tabContainer);

    state.buttonLayout = CRowLayoutBuilder::begin()->size({CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1, 1}})->gap(5)->commence();
    state.buttonLayout->setMargin(2);

    state.buttonBack       = CButtonBuilder::begin()->label(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_BACK))->onMainClick([](SP<CButtonElement> self) { tabBack(); })->commence();
    state.buttonNext       = CButtonBuilder::begin()->label(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_NEXT))->onMainClick([](SP<CButtonElement> self) { tabNext(); })->commence();
    state.buttonQuit       = CButtonBuilder::begin()
                                 ->label(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_THANKS))
                                 ->onMainClick([w = WP<IWindow>{window}](SP<CButtonElement> self) {
                               if (w)
                                   w->close();
                               state.backend->destroy();
                                 })
                                 ->commence();
    state.buttonLaunchTerm = CButtonBuilder::begin()
                                 ->label(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_OPENTERM))
                                 ->onMainClick([w = WP<IWindow>{window}](SP<CButtonElement> self) {
                                     for (const auto& t : TERMINALS) {
                                         if (!appExists(t))
                                             continue;

                                         CProcess proc(t, {});
                                         proc.runAsync();
                                         break;
                                     }
                                 })
                                 ->commence();
    state.buttonFinish     = CButtonBuilder::begin()
                                 ->label(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_FINISH))
                                 ->onMainClick([w = WP<IWindow>{window}](SP<CButtonElement> self) {
                                 removeAutogen();
                                 if (w)
                                     w->close();
                                 state.backend->destroy();
                                 })
                                 ->commence();
    state.buttonOpenWiki   = CButtonBuilder::begin()
                                 ->label(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_WIKI))
                                 ->onMainClick([w = WP<IWindow>{window}](SP<CButtonElement> self) {
                                   CProcess proc("xdg-open", {"https://wiki.hypr.land/"});
                                   proc.runAsync();

                                   state.buttonOpenWiki->rebuild()->label(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_OPENED))->commence();
                                   state.wikiOpenTimer = state.backend->addTimer(
                                       std::chrono::seconds(1), [](ASP<CTimer> t, void* d) { state.buttonOpenWiki->rebuild()->label(I18n::i18nEngine()->localize(I18n::TXT_KEY_WELCOME_WIKI))->commence(); }, nullptr);
                                 })
                                 ->commence();

    state.buttonSpacer = CNullBuilder::begin()->size({CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {1, 1}})->commence();
    state.buttonSpacer->setGrow(true);

    rootLayout->addChild(state.buttonLayout);

    window->m_events.closeRequest.listenStatic([w = WP<IWindow>{window}] {
        w->close();
        state.backend->destroy();
    });

    updateTab();

    window->open();

    state.backend->enterLoop();

    return 0;
}
