#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>

#include <hyprutils/memory/SharedPtr.hpp>

template <typename T>
using SP = Hyprutils::Memory::CSharedPointer<T>;

namespace I18n {

    enum eI18nKeys : uint8_t {
        TXT_KEY_WELCOME_TITLE = 0,
        TXT_KEY_WELCOME_GETSTART,
        TXT_KEY_WELCOME_APPS,
        TXT_KEY_WELCOME_CONFIG,
        TXT_KEY_WELCOME_ECOSYSTEM,
        TXT_KEY_WELCOME_THATSIT,
        TXT_KEY_WELCOME_CONTENT1,
        TXT_KEY_WELCOME_CONTENT2,
        TXT_KEY_WELCOME_CONTENT3,
        TXT_KEY_WELCOME_CONTENT4,
        TXT_KEY_WELCOME_CONTENT5,
        TXT_KEY_WELCOME_CONTENT6,
        TXT_KEY_WELCOME_RUNNING,
        TXT_KEY_WELCOME_INSTALLED,
        TXT_KEY_WELCOME_MISSING,
        TXT_KEY_WELCOME_ACCEPTED,
        TXT_KEY_WELCOME_RECOMMENDED,
        TXT_KEY_WELCOME_ERROR1,
        TXT_KEY_WELCOME_ERROR2,
        TXT_KEY_WELCOME_ERROR3,
        TXT_KEY_WELCOME_ERROR4,
        TXT_KEY_WELCOME_ERROR5,
        TXT_KEY_WELCOME_AUTHAGENT,
        TXT_KEY_WELCOME_FILES,
        TXT_KEY_WELCOME_TERM,
        TXT_KEY_WELCOME_PIPEWIRE,
        TXT_KEY_WELCOME_WALLPAPER,
        TXT_KEY_WELCOME_PORTAL,
        TXT_KEY_WELCOME_NOTIF,
        TXT_KEY_WELCOME_NOTIFNOTE,
        TXT_KEY_WELCOME_SHELL,
        TXT_KEY_WELCOME_SHELLNOTE,
        TXT_KEY_WELCOME_LAUNCHER,
        TXT_KEY_WELCOME_CLIPBOARD,
        TXT_KEY_WELCOME_CLIPBOARDNOTE,
        TXT_KEY_WELCOME_ERROR6,
        TXT_KEY_WELCOME_APPINST,
        TXT_KEY_WELCOME_APPNOINST,
        TXT_KEY_WELCOME_CHANGELATER,
        TXT_KEY_WELCOME_THANKS,
        TXT_KEY_WELCOME_BACK,
        TXT_KEY_WELCOME_NEXT,
        TXT_KEY_WELCOME_OPENTERM,
        TXT_KEY_WELCOME_FINISH,
        TXT_KEY_WELCOME_WIKI,
        TXT_KEY_WELCOME_OPENED,
    };

    class CI18nEngine {
      public:
        CI18nEngine();
        ~CI18nEngine() = default;

        std::string localize(eI18nKeys key, const std::unordered_map<std::string, std::string>& vars = {});
    };

    SP<CI18nEngine> i18nEngine();
};