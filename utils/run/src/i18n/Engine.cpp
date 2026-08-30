#include "Engine.hpp"

#include <hyprutils/i18n/I18nEngine.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
// #include "../config/ConfigValue.hpp" // To read general:locale
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
    registerEntry("en_US", TXT_KEY_RUN_TITLE, "Run an appplication");
    registerEntry("en_US", TXT_KEY_RUN_NOFOUND, "Executable doesn't exist");
    registerEntry("en_US", TXT_KEY_RUN_NORUN, "Couldn't execute process");
    registerEntry("en_US", TXT_KEY_RUN_NOSTART, "Process couldn't start");
    registerEntry("en_US", TXT_KEY_RUN_INPUT, "Input the app name...");
    registerEntry("en_US", TXT_KEY_RUN_CANCEL, "Cancel");
    registerEntry("en_US", TXT_KEY_RUN_RUN, "Run");

    // it_IT (Italian)
    registerEntry("it_IT", TXT_KEY_RUN_TITLE, "Esegui un'applicazione");
    registerEntry("it_IT", TXT_KEY_RUN_NOFOUND, "L'eseguibile non esiste");
    registerEntry("it_IT", TXT_KEY_RUN_NORUN, "Impossibile eseguire il processo");
    registerEntry("it_IT", TXT_KEY_RUN_NOSTART, "Impossibile avviare il processo");
    registerEntry("it_IT", TXT_KEY_RUN_INPUT, "Inserisci il nome dell'appp...");
    registerEntry("it_IT", TXT_KEY_RUN_CANCEL, "Annulla");
    registerEntry("it_IT", TXT_KEY_RUN_RUN, "Esegui");
}

std::string I18n::CI18nEngine::localize(eI18nKeys key, const Hyprutils::I18n::translationVarMap& vars) {
    /*
    static auto CONFIG_LOCALE = CConfigValue<std::string>("general:locale");
    std::string locale        = *CONFIG_LOCALE != "" ? *CONFIG_LOCALE : localeStr;
    */
   std::string locale = localeStr;
    return huEngine->localizeEntry(locale, key, vars);
}
