#include "Engine.hpp"

#include <hyprutils/i18n/I18nEngine.hpp>
#include <hyprutils/memory/SharedPtr.hpp>
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

/* would needed for plurals
[[gnu::noinline]] static void registerEntry(const char* locale, eI18nKeys key, const char* (*translationFunc)(const Hyprutils::I18n::translationVarMap&)) {
    huEngine->registerEntry(locale, key, translationFunc);
}
*/

I18n::CI18nEngine::CI18nEngine() {
    huEngine = makeShared<Hyprutils::I18n::CI18nEngine>();
    huEngine->setFallbackLocale("en_US");
    localeStr = huEngine->getSystemLocale().locale();

    // en_US (English)
    registerEntry("en_US", TXT_KEY_WELCOME_TITLE, "Welcome to Hyprland!");
    registerEntry("en_US", TXT_KEY_WELCOME_GETSTART, "Getting started");
    registerEntry("en_US", TXT_KEY_WELCOME_APPS, "Default apps");
    registerEntry("en_US", TXT_KEY_WELCOME_CONFIG, "Basic configuration");
    registerEntry("en_US", TXT_KEY_WELCOME_ECOSYSTEM, "Hypr ecosystem");
    registerEntry("en_US", TXT_KEY_WELCOME_THATSIT, "That's it!");

    registerEntry("en_US", TXT_KEY_WELCOME_CONTENT1,
                  R"#(We hope you enjoy your stay. In order to help you get accommodated to Hyprland in an easier manner, we prepared a little basic setup tutorial, just for you.

If you feel adventurous, or are an advanced user, you can click the "Thanks, but I don't need help" button on the bottom. It will close this window and never show it again.

If you want to manually launch this welcome app, just execute hyprland-welcome in your terminal.

Click the "Next" button to proceed to the next step of your setup :)
)#");

    registerEntry("en_US", TXT_KEY_WELCOME_CONTENT2,
                  R"#(The first thing we'll need to do is get some packages installed that you absolutely need in order for your system to be working properly.
Apps with a <span foreground="#cc2222">*</span> are <span foreground="red"><i>absolutely necessary</i></span> for a working system. All other are <span foreground="red"><i>highly</i></span> recommended, as they provide core parts of a working environment.
You can proceed without any of those, but it's not advised.

There is a possibility that this app is unable to detect some of your installed binaries. In that case, it's okay to ignore them.

Use the <i>Launch terminal</i> button to launch a terminal.
Use SUPER+M to exit Hyprland.
Supported terminals: kitty, alacritty, foot, wezterm, konsole, gnome-terminal, xterm.

<i>Hint: Hover on the different components to see what options are accepted. <span foreground="#22cc22">Green</span> means the component is found to be installed, <span foreground="#22cccc">blue</span> means it's running.
This list refreshes automatically.</i>)#");

    registerEntry("en_US", TXT_KEY_WELCOME_CONTENT3, R"#(We know that not everyone uses kitty and dolphin. That's why we let you choose.
If you wish to change the defaults, use the dropdowns below.)#");

    registerEntry(
        "en_US", TXT_KEY_WELCOME_CONTENT4,
        R"#(Now that you've installed the basic apps, you might want some of them to autostart. Hyprland doesn't automatically start anything for you, you need to tell it to.
Go to ~/.config/hypr/hyprland.lua, and add "hl.exec_cmd("appname")" surrounded by hl.on() to launch your apps, for example:
hl.on("hyprland.start", function ()
    hl.exec_cmd("hyprpaper")
    hl.exec_cmd("waybar")
end)

In general, configuring apps is something for you to do. Each app you install may come with its own config file and options.

A great point to start is the Hyprland wiki at https://wiki.hypr.land. There, the master tutorial will teach you everything and link to further docs.

If you prefer pre-configured settings, or "dotfiles", you can see the "Preconfigured setups" section on the wiki, or search online. <span foreground="#cc2222">Important note:</span> dotfiles can run <i>anything</i> on your computer. Make sure you trust the source.)#");

    registerEntry("en_US", TXT_KEY_WELCOME_CONTENT5, R"#(Hyprland has a wide ecosystem of apps specifically made for it.
Unlike some other popular DEs, it does not force you to use most of them by default.

You can install those elements separately, only those that you need.

Check the wiki under "Hypr Ecosystem" to see all of the apps, their usage and configuration.)#");

    registerEntry("en_US", TXT_KEY_WELCOME_CONTENT6, R"#(That's it for this small introduction! Explore the wiki, and various apps, and enjoy your journey!

Here are some important default shortcuts:
• SUPER + Q <span foreground="#666666">=</span> Terminal
• SUPER + E <span foreground="#666666">=</span> File Manager
• SUPER + R <span foreground="#666666">=</span> Launcher
• SUPER + C <span foreground="#666666">=</span> Close window
• SUPER + V <span foreground="#666666">=</span> Toggle floating
• SUPER + M <span foreground="#666666">=</span> Exit Hyprland
• SUPER + [1 - 9] <span foreground="#666666">=</span> Workspaces 1 - 9
• SUPER + SHIFT + [1 - 9] <span foreground="#666666">=</span> Move window to workspace 1 - 9
• SUPER + [ ← ↑ ↓ → ] <span foreground="#666666">=</span> Move focus around

<i>You can easily change these in your hyprland.lua.</i>
    
Thank you for choosing Hyprland! ❤️)#");

    registerEntry("en_US", TXT_KEY_WELCOME_RUNNING, "{type}{star}: <span foreground=\"#22cccc\">Running</span>: {name}");
    registerEntry("en_US", TXT_KEY_WELCOME_INSTALLED, "{type}{star}: <span foreground=\"#22cc22\">Installed</span>: {name}");
    registerEntry("en_US", TXT_KEY_WELCOME_MISSING, "{type}{star}: <span foreground=\"#cc2222\">Missing</span>");
    registerEntry("en_US", TXT_KEY_WELCOME_ACCEPTED, "Accepted: {accepted}");
    registerEntry("en_US", TXT_KEY_WELCOME_RECOMMENDED, "Recommended: {recommended}\nAccepted: {accepted}");
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR1, "Can't save: neither $XDG_CONFIG_HOME nor $HOME env is set");
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR1, "Can't save: failed to read Lua config");
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR1, "Can't save: config isn't default, doesn't have Lua default variable");
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR1, "Can't save: failed to open Lua config");
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR1, "Can't save: failed to write Lua config");
    registerEntry("en_US", TXT_KEY_WELCOME_AUTHAGENT, "Authentication agent");
    registerEntry("en_US", TXT_KEY_WELCOME_FILES, "File manager");
    registerEntry("en_US", TXT_KEY_WELCOME_TERM, "Terminal");
    registerEntry("en_US", TXT_KEY_WELCOME_PIPEWIRE, "Pipewire");
    registerEntry("en_US", TXT_KEY_WELCOME_WALLPAPER, "Wallpaper");
    registerEntry("en_US", TXT_KEY_WELCOME_PORTAL, "XDG Desktop Portal");
    registerEntry("en_US", TXT_KEY_WELCOME_NOTIF, "Notification daemon");
    registerEntry("en_US", TXT_KEY_WELCOME_NOTIFNOTE, "Please note you can have custom notification daemons with your shell, e.g. quickshell.");
    registerEntry("en_US", TXT_KEY_WELCOME_SHELL, "Status bar / shell");
    registerEntry("en_US", TXT_KEY_WELCOME_SHELLNOTE, "For new users we recommend waybar, for advanced users quickshell.");
    registerEntry("en_US", TXT_KEY_WELCOME_LAUNCHER, "Application launcher");
    registerEntry("en_US", TXT_KEY_WELCOME_CLIPBOARD, "Clipboard");
    registerEntry("en_US", TXT_KEY_WELCOME_CLIPBOARDNOTE, "wl-copy is provided by wl-clipboard in most distros.");
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR6, "<span foreground=\"#cc2222\">⚠ Error: {error}</span>");
    registerEntry("en_US", TXT_KEY_WELCOME_APPINST, "<span foreground=\"#22cc22\">✓ {name} is installed</span>");
    registerEntry("en_US", TXT_KEY_WELCOME_APPNOINST, "<span foreground=\"#cc2222\">⚠ {name} is not installed</span>");
    registerEntry("en_US", TXT_KEY_WELCOME_CHANGELATER, "<i>You can always change these later in your hyprland.lua</i>");

    registerEntry("en_US", TXT_KEY_WELCOME_THANKS, "Thanks, but I don't need help");
    registerEntry("en_US", TXT_KEY_WELCOME_BACK, "Back");
    registerEntry("en_US", TXT_KEY_WELCOME_NEXT, "Next");
    registerEntry("en_US", TXT_KEY_WELCOME_OPENTERM, "Launch terminal");
    registerEntry("en_US", TXT_KEY_WELCOME_FINISH, "Finish");
    registerEntry("en_US", TXT_KEY_WELCOME_WIKI, "🔗 Open wiki");
    registerEntry("en_US", TXT_KEY_WELCOME_OPENED, "🔗 Opened in your browser");

    // it_IT (Italian)
    registerEntry("it_IT", TXT_KEY_WELCOME_TITLE, "Benvenuto/a su Hyprland!");
    registerEntry("it_IT", TXT_KEY_WELCOME_GETSTART, "Iniziamo");
    registerEntry("it_IT", TXT_KEY_WELCOME_APPS, "App predefinite");
    registerEntry("it_IT", TXT_KEY_WELCOME_CONFIG, "Configurazione base");
    registerEntry("it_IT", TXT_KEY_WELCOME_ECOSYSTEM, "Ecosistema Hypr");
    registerEntry("it_IT", TXT_KEY_WELCOME_THATSIT, "Ecco fatto!");

    registerEntry("it_IT", TXT_KEY_WELCOME_CONTENT1, R"#(Speriamo che tu ti trovi bene. Per aiutarti ad abituarti ad Hyprland, abbiamo preparato un breve tutorial, solo per te.

Se ti piace l'avventura o hai conoscenze pregresse, scegli "Grazie, non mi serve aiuto" in basso. Chiuderà questa finestra e non la vedrai mai più.

Per aprire manualmente quest'app, esegui semplicemente hyprland-welcome in un terminale.

Scegli "Avanti" per procedere ai prossimi passi della guida :)
)#");

    registerEntry("it_IT", TXT_KEY_WELCOME_CONTENT2, R"#(La prima cosa da fare è assicurarci di aver installato alcuni pacchetti necessari per far funzionare a dovere il sistema.
Le app segnate con <span foreground="#cc2222">*</span> sono <span foreground="red"><i>fondamentali</i></span> per un sistema funzionante. Le altre sono <span foreground="red"><i>fortemente</i></span> consigliate, in quanto parti importanti di un ambiente funzionante.
Puoi procedere senza nessuna di queste ma non è consigliato.

È possibile che quest'app non riesca a rilevare alcuni eseguibili. In quel caso, puoi tranquillamente ignorarli.

Usa <i>Apri terminale</i> per aprire un terminale.
Usa SUPER+M per chiudere Hyprland.
Terminali supportati: kitty, alacritty, foot, wezterm, konsole, gnome-terminal, xterm.

<i>Consiglio: Passa col mouse sopra alle opzioni per vedere le scelte possibili. <span foreground="#22cc22">Verde</span> significa che la componente è installata, <span foreground="#22cccc">blu</span> che è in esecuzione.
Questa lista si aggiorna automaticamente.</i>)#");

    registerEntry("it_IT", TXT_KEY_WELCOME_CONTENT3, R"#(Sappiamo che non tutti usano kitty o dolphin. Per questo puoi scegliere.
Se vuoi cambiare i default, usa i menu qui sotto.)#");

    registerEntry("it_IT", TXT_KEY_WELCOME_CONTENT4,
                  R"#(Ora che hai installato queste app, potresti volere che si avviassero da sole. Hyprland non avvia niente automaticamente, per questo devi dirgli tu di farlo.
Apri ~/.config/hypr/hyprland.lua e aggiungi "hl.exec_cmd("appname")" in un hl.on() per avviarle, ad esempio:
hl.on("hyprland.start", function ()
    hl.exec_cmd("hyprpaper")
    hl.exec_cmd("waybar")
end)

In generale, configurare le app sta a te. Ogni app che installi potrebbe avere il suo file di configurazione.

Un buon posto per iniziare è la wiki di Hyprland, https://wiki.hypr.land. Lì troverai una pagina che ti insegnerà l'essenziale e collegamenti aggiuntivi per più documentazione.

Se preferisci configurazioni già fatte, o "dotfile", puoi vedere la sezione "Preconfigured setups" della wiki, o cercare in rete. <span foreground="#cc2222">Nota bene:</span> questi possono eseguire <i>qualsiasi cosa</i> sul tuo computer. Usa fonti fidate.)#");

    registerEntry("it_IT", TXT_KEY_WELCOME_CONTENT5, R"#(Hyprland ha un ampio ecosistema di app ad-hoc.
Al contrario di altri DE popolari, non ti forza ad utilizzarle di default.

Puoi installare queste componenti a parte (quelle che ti servono).

Dai un'occhiata alla wiki sotto "Hypr Ecosystem" per vedere l'app, come usarle e configurarle.)#");

    registerEntry("it_IT", TXT_KEY_WELCOME_CONTENT6, R"#(È tutto per questa piccola introduzione! Esplora la wiki, le varie app e goditi il viaggio!

Eccoti alcune scorciatoie importanti:
• SUPER + Q <span foreground="#666666">=</span> Terminale
• SUPER + E <span foreground="#666666">=</span> Gestore di file
• SUPER + R <span foreground="#666666">=</span> Launcher
• SUPER + C <span foreground="#666666">=</span> Chiudi finestra
• SUPER + V <span foreground="#666666">=</span> Alterna fluttuante
• SUPER + M <span foreground="#666666">=</span> Esci da Hyprland
• SUPER + [1 - 9] <span foreground="#666666">=</span> Area di lavoro 1 - 9
• SUPER + SHIFT + [1 - 9] <span foreground="#666666">=</span> Sposta la finestra all'area di lavoro 1 - 9
• SUPER + [ ←↑↓→ ] <span foreground="#666666">=</span> Scegli finestra attiva

<i>Puoi cambiarle facilmente nel tuo hyprland.lua.</i>
    
Grazie per aver scelto Hyprland! ❤️)#");

    registerEntry("it_IT", TXT_KEY_WELCOME_RUNNING, "{type}{star}: <span foreground=\"#22cccc\">In esecuzione</span>: {name}");
    registerEntry("it_IT", TXT_KEY_WELCOME_INSTALLED, "{type}{star}: <span foreground=\"#22cc22\">Installato</span>: {name}");
    registerEntry("it_IT", TXT_KEY_WELCOME_MISSING, "{type}{star}: <span foreground=\"#cc2222\">Mancante</span>");
    registerEntry("it_IT", TXT_KEY_WELCOME_ACCEPTED, "Accettati: {accepted}");
    registerEntry("it_IT", TXT_KEY_WELCOME_RECOMMENDED, "Consigliato: {recommended}\nAccettati: {accepted}");
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR1, "Impossibile salvarew: né $XDG_CONFIG_HOME né $HOME impostato");
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR1, "Impossibile salvarew: impossibile leggere config Lua");
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR1, "Impossibile salvarew: config diverso dal default, non ha la variabile Lua");
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR1, "Impossibile salvarew: impossibile aprire config Lua");
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR1, "Impossibile salvarew: impossibile salvare config Lua");
    registerEntry("it_IT", TXT_KEY_WELCOME_AUTHAGENT, "Agente d'autenticazione");
    registerEntry("it_IT", TXT_KEY_WELCOME_FILES, "Gestore di file");
    registerEntry("it_IT", TXT_KEY_WELCOME_TERM, "Terminale");
    registerEntry("it_IT", TXT_KEY_WELCOME_PIPEWIRE, "Pipewire");
    registerEntry("it_IT", TXT_KEY_WELCOME_WALLPAPER, "Sfondo");
    registerEntry("it_IT", TXT_KEY_WELCOME_PORTAL, "XDG Desktop Portal");
    registerEntry("it_IT", TXT_KEY_WELCOME_NOTIF, "Demone di notifiche");
    registerEntry("it_IT", TXT_KEY_WELCOME_NOTIFNOTE, "Nota che puoi creare demoni di notifiche custom con la tua shell, es. quickshell.");
    registerEntry("it_IT", TXT_KEY_WELCOME_SHELL, "Barra di stato / shell");
    registerEntry("it_IT", TXT_KEY_WELCOME_SHELLNOTE, "Per gli utenti nuovi consigliamo waybar, per quelli avanzati quickshell.");
    registerEntry("it_IT", TXT_KEY_WELCOME_LAUNCHER, "Launcher di applicazioni");
    registerEntry("it_IT", TXT_KEY_WELCOME_CLIPBOARD, "Appunti");
    registerEntry("it_IT", TXT_KEY_WELCOME_CLIPBOARDNOTE, "wl-copy è fornito da wl-clipboard in molte distribuzioni.");
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR6, "<span foreground=\"#cc2222\">⚠ Errore: {error}</span>");
    registerEntry("it_IT", TXT_KEY_WELCOME_APPINST, "<span foreground=\"#22cc22\">✓ {name} è installato</span>");
    registerEntry("it_IT", TXT_KEY_WELCOME_APPNOINST, "<span foreground=\"#cc2222\">⚠ {name} non è installato</span>");
    registerEntry("it_IT", TXT_KEY_WELCOME_CHANGELATER, "<i>Puoi sempre cambiarli dopo nel tuo hyprland.lua</i>");

    registerEntry("it_IT", TXT_KEY_WELCOME_THANKS, "Grazie, non mi serve aiuto");
    registerEntry("it_IT", TXT_KEY_WELCOME_BACK, "Indietro");
    registerEntry("it_IT", TXT_KEY_WELCOME_NEXT, "Avanti");
    registerEntry("it_IT", TXT_KEY_WELCOME_OPENTERM, "Apri terminale");
    registerEntry("it_IT", TXT_KEY_WELCOME_FINISH, "Fine");
    registerEntry("it_IT", TXT_KEY_WELCOME_WIKI, "🔗 Apri wiki");
    registerEntry("it_IT", TXT_KEY_WELCOME_OPENED, "🔗 Aperto nel browser");
}

std::string I18n::CI18nEngine::localize(eI18nKeys key, const Hyprutils::I18n::translationVarMap& vars) {
    return huEngine->localizeEntry(localeStr, key, vars);
}
