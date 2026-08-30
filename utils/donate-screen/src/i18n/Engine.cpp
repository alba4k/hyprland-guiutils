#include "Engine.hpp"

#include <hyprutils/i18n/I18nEngine.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
// #include "../config/ConfigValue.hpp" // To read general:localeit_IT
using namespace Hyprutils::Memory;

using namespace I18n;
using namespace Hyprutils::I18n;

static SP<Hyprutils::I18n::CI18nEngine> huEngine;
static std::string                      localeStr;

//
SP<I18n::CI18nEngine> I18n::i18nEngine() {
    static SP<I18n::CI18nEngine> engine = makeShared<I18n::CI18nEngine>();
    return engine;
}

// prevents CI18nEngine constructor from being bloated by std::string allocations/deallocations
[[gnu::noinline]] static void registerEntry(const char* locale, eI18nKeys key, const char* translation) {
    huEngine->registerEntry(locale, key, translation);
}

/* only needed for plurals
[[gnu::noinline]] static void registerEntry(const char* locale, eI18nKeys key, const char* (*translationFunc)(const Hyprutils::I18n::translationVarMap&)) {
    huEngine->registerEntry(locale, key, translationFunc);
}
*/

I18n::CI18nEngine::CI18nEngine() {
    huEngine = makeShared<Hyprutils::I18n::CI18nEngine>();
    huEngine->setFallbackLocale("en_US");
    localeStr = huEngine->getSystemLocale().locale();

    // en_US (English)
    registerEntry("en_US", TXT_KEY_DONATE_TITLE, "Support Hyprland");
    registerEntry("en_US", TXT_KEY_DONATE_CONTENT, R"#(Hyprland is built and maintained by volunteers, with one person working on it full-time.

If Hyprland is useful to you, supporting the project helps keep that work sustainable and lets us keep improving it.

You can make a one-time donation or support us monthly.
There's also Hyprperks for 5€ + tax / month, which includes a few small "thank you" goodies from us.

)#");
    registerEntry("en_US", TXT_KEY_DONATE_SUPPORT, "💝 Support");
    registerEntry("en_US", TXT_KEY_DONATE_THANKYOU, "💝 Thank you!");
    registerEntry("en_US", TXT_KEY_DONATE_NOTHANKS, "No thanks");

    // it_IT (Italian)
    registerEntry("it_IT", TXT_KEY_DONATE_TITLE, "Supporta Hyprland");
    registerEntry("it_IT", TXT_KEY_DONATE_CONTENT, R"#(Hyprland è creato e mantenuto da volontari, con una persona che ci lavora a tempo pieno.

Se trovi Hyprland utile, supportare il progetto aiuta a mantenere questo lavoro sostenibile e ci permette di continuare a migliorarlo.

Puoi fare una singola donazione oppure supportarci mensilmente.
Inoltre per 5€ + tasse / mese c'è Hyprperks, Che include un po' di regali come "grazie" da parte nostra.

)#");
    registerEntry("it_IT", TXT_KEY_DONATE_SUPPORT, "💝 Supportaci");
    registerEntry("it_IT", TXT_KEY_DONATE_THANKYOU, "💝 Grazie!");
    registerEntry("it_IT", TXT_KEY_DONATE_NOTHANKS, "No grazie");
}

std::string I18n::CI18nEngine::localize(eI18nKeys key, const Hyprutils::I18n::translationVarMap& vars) {
    /*
    static auto CONFIG_LOCALE = CConfigValue<std::string>("general:locale");
    std::string locale        = *CONFIG_LOCALE != "" ? *CONFIG_LOCALE : localeStr;
    */
    std::string locale = localeStr;
    return huEngine->localizeEntry(locale, key, vars);
}
