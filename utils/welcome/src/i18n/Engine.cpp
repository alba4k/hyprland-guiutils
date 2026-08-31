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
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR2, "Can't save: failed to read Lua config");
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR3, "Can't save: config isn't default, doesn't have Lua default variable");
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR4, "Can't save: failed to open Lua config");
    registerEntry("en_US", TXT_KEY_WELCOME_ERROR5, "Can't save: failed to write Lua config");
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
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR2, "Impossibile salvarew: impossibile leggere config Lua");
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR3, "Impossibile salvarew: config diverso dal default, non ha la variabile Lua");
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR4, "Impossibile salvarew: impossibile aprire config Lua");
    registerEntry("it_IT", TXT_KEY_WELCOME_ERROR5, "Impossibile salvarew: impossibile salvare config Lua");
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

    // de_DE (German)
    registerEntry("de_DE", TXT_KEY_WELCOME_TITLE, "Willkommen bei Hyprland!");
    registerEntry("de_DE", TXT_KEY_WELCOME_GETSTART, "Loslegen");
    registerEntry("de_DE", TXT_KEY_WELCOME_APPS, "Standardanwendungen");
    registerEntry("de_DE", TXT_KEY_WELCOME_CONFIG, "Einfache Konfiguration");
    registerEntry("de_DE", TXT_KEY_WELCOME_ECOSYSTEM, "Hypr Ökosystem");
    registerEntry("de_DE", TXT_KEY_WELCOME_THATSIT, "Das war's!");

    registerEntry("de_DE", TXT_KEY_WELCOME_CONTENT1,
                    R"#(Wir hoffen, du genießt deinen Aufenthalt. Um dir zu helfen, Hyprland einfacher kennenzulernen, haben wir ein kleines Einstiegstutorial vorbereitet, nur für dich.

Falls du dich abenteuerlich fühlst oder bereits ein fortgeschrittener Benutzer bist, kannst du unten auf "Danke, aber ich brauche keine Hilfe" klicken. Das wird dieses Fenster schließen und es nie mehr anzeigen.

Falls du diese Willkommens-App manuell öffnen willst, kannst du einfach hyprland-welcome in deiner Konsole ausführen.

Klicke auf "Weiter", um zum nächsten Schritt deiner Einrichtung fortzufahren :)
)#");

    registerEntry("de_DE", TXT_KEY_WELCOME_CONTENT2,
                    R"#(Zuerst müssen wir ein paar Pakete installieren, die du unbedingt brauchst, damit dein System richtig funktioniert.
Apps mit einem <span foreground="#cc2222">*</span> sind <span foreground="red"><i>unbedingt notwendig</i></span> für ein funktionierendes System. Alle anderen sind <span foreground="red"><i>dringend</i></span> empfohlen, da sie grundlegende Bestandteile eines funktionierenden Systems sind.
Du kannst auch ohne sie fortfahren, aber davon wird abgeraten.

Es ist möglich, dass diese App gewisse deiner installierten Binaries nicht erkennt. In diesem Fall ist es okay, diese zu ignorieren.

Nutze den <i>Terminal öffnen</i> Knopf, um ein Terminal zu öffnen.
Nutze SUPER+M, um Hyprland zu verlassen.
Unterstützte Terminals: kitty, alacritty, foot, wezterm, konsole, gnome-terminal, xterm.

<i>Tipp: Fahre mit dem Mauszeiger über die verschiedenen Komponenten, um zu sehen, welche Optionen akzeptiert sind. <span foreground="#22cc22">Grün</span> bedeutet, dass die Komponente als installiert erkannt wurde, <span foreground="#22cccc">blau</span> heißt, es läuft bereits.
Diese Liste aktualisiert sich automatisch.</i>)#");

    registerEntry("de_DE", TXT_KEY_WELCOME_CONTENT3, R"#(Wir wissen, dass nicht jeder Kitty und Dolphin verwendet. Deswegen lassen wir dich wählen.
Falls du die Standardeinstellungen ändern willst, nutze die folgenden Dropdowns.)#");

    registerEntry(
        "de_DE", TXT_KEY_WELCOME_CONTENT4,
        R"#(Da du jetzt die grundlegenden Apps installiert hast, möchtest du vielleicht ein paar von ihnen automatisch starten. Hyprland startet nichts automatisch für dich, du musst dies selbst festlegen.
Gehe zu ~/.config/hypr/hyprland.lua und füge "hl.exec_cmd("appname")" umgeben von hl.on() ein, um deine Apps zu starten, zum Beispiel:
hl.on("hyprland.start", function ()
    hl.exec_cmd("hyprpaper")
    hl.exec_cmd("waybar")
end)

Generell ist das Konfigurieren von Apps etwas, das du selbst machen musst. Jede App, die du installierst, kann eine eigene Konfigurationsdatei und eigene Optionen haben.

Ein guter Startpunkt ist das Hyprland-Wiki auf https://wiki.hypr.land. Dort wird dir das "Master tutorial" alles beibringen und zu weiterer Dokumentation verlinken.

Falls du vorgefertigte Einstellungen oder "dotfiles" bevorzugst, kannst du dir die "Preconfigured setups" Seite im Wiki anschauen oder online suchen. <span foreground="#cc2222">Wichtiger Hinweis:</span> dotfiles können <i>alles</i> auf deinem Computer ausführen. Stelle sicher, dass du der Quelle vertraust.)#");

    registerEntry("de_DE", TXT_KEY_WELCOME_CONTENT5, R"#(Hyprland hat ein breites Ökosystem von Apps, die spezifisch dafür gemacht wurden.
Anders als andere DEs zwingt es dich aber nicht, die meisten davon standardmäßig zu verwenden.

Du kannst diese Elemente separat installieren, und nur die, die du brauchst.

Schau im Wiki unter "Hypr Ecosystem" nach, um alle Apps und deren Nutzung und Konfiguration zu sehen.)#");

    registerEntry("de_DE", TXT_KEY_WELCOME_CONTENT6, R"#(Das war's für diese kleine Einführung! Erkunde das Wiki, viele verschiedene Apps, und genieße deine Reise!

Hier sind ein paar wichtige Standard-Tastenkombinationen:
• SUPER + Q <span foreground="#666666">=</span> Terminal
• SUPER + E <span foreground="#666666">=</span> Dateimanager
• SUPER + R <span foreground="#666666">=</span> Launcher
• SUPER + C <span foreground="#666666">=</span> Fenster schließen
• SUPER + V <span foreground="#666666">=</span> Floating umschalten
• SUPER + M <span foreground="#666666">=</span> Hyprland verlassen
• SUPER + [1 - 9] <span foreground="#666666">=</span> Workspace 1 - 9
• SUPER + SHIFT + [1 - 9] <span foreground="#666666">=</span> Fenster zu Workspace 1 - 9 verschieben
• SUPER + [ ← ↑ ↓ → ] <span foreground="#666666">=</span> Fokus verschieben

<i>Du kannst diese einfach in hyprland.lua verändern.</i>

Danke, dass du dich für Hyprland entschieden hast! ❤️)#");

    registerEntry("de_DE", TXT_KEY_WELCOME_RUNNING, "{type}{star}: <span foreground=\"#22cccc\">Läuft</span>: {name}");
    registerEntry("de_DE", TXT_KEY_WELCOME_INSTALLED, "{type}{star}: <span foreground=\"#22cc22\">Installiert</span>: {name}");
    registerEntry("de_DE", TXT_KEY_WELCOME_MISSING, "{type}{star}: <span foreground=\"#cc2222\">Fehlt</span>");
    registerEntry("de_DE", TXT_KEY_WELCOME_ACCEPTED, "Akzeptiert: {accepted}");
    registerEntry("de_DE", TXT_KEY_WELCOME_RECOMMENDED, "Empfohlen: {recommended}\nAkzeptiert: {accepted}");
    registerEntry("de_DE", TXT_KEY_WELCOME_ERROR1, "Speichern fehlgeschlagen: weder $XDG_CONFIG_HOME noch $HOME ist gesetzt");
    registerEntry("de_DE", TXT_KEY_WELCOME_ERROR2, "Speichern fehlgeschlagen: konnte die Lua-Konfiguration nicht lesen");
    registerEntry("de_DE", TXT_KEY_WELCOME_ERROR3, "Speichern fehlgeschlagen: Konfiguration ist nicht der Standard, hat keine Lua-Standardvariable");
    registerEntry("de_DE", TXT_KEY_WELCOME_ERROR4, "Speichern fehlgeschlagen: konnte die Lua-Konfiguration nicht öffnen");
    registerEntry("de_DE", TXT_KEY_WELCOME_ERROR5, "Speichern fehlgeschlagen: konnte die Lua-Konfiguration nicht schreiben");
    registerEntry("de_DE", TXT_KEY_WELCOME_AUTHAGENT, "Authentifizierungsagent");
    registerEntry("de_DE", TXT_KEY_WELCOME_FILES, "Dateimanager");
    registerEntry("de_DE", TXT_KEY_WELCOME_TERM, "Terminal");
    registerEntry("de_DE", TXT_KEY_WELCOME_PIPEWIRE, "Pipewire");
    registerEntry("de_DE", TXT_KEY_WELCOME_WALLPAPER, "Hintergrundbild");
    registerEntry("de_DE", TXT_KEY_WELCOME_PORTAL, "XDG Desktop Portal");
    registerEntry("de_DE", TXT_KEY_WELCOME_NOTIF, "Benachrichtigungsdienst");
    registerEntry("de_DE", TXT_KEY_WELCOME_NOTIFNOTE, "Bitte beachte, dass deine Shell (z.B. quickshell) auch einen eigenen Benachrichtigungsdienst beinhalten kann.");
    registerEntry("de_DE", TXT_KEY_WELCOME_SHELL, "Status bar / shell");
    registerEntry("de_DE", TXT_KEY_WELCOME_SHELLNOTE, "Für neue Nutzer empfehlen wir waybar, für fortgeschrittene quickshell.");
    registerEntry("de_DE", TXT_KEY_WELCOME_LAUNCHER, "App Launcher");
    registerEntry("de_DE", TXT_KEY_WELCOME_CLIPBOARD, "Zwischenablage");
    registerEntry("de_DE", TXT_KEY_WELCOME_CLIPBOARDNOTE, "wl-copy wird bei den meisten Distributionen von wl-clipboard zur Verfügung gestellt.");
    registerEntry("de_DE", TXT_KEY_WELCOME_ERROR6, "<span foreground=\"#cc2222\">⚠ Fehler: {error}</span>");
    registerEntry("de_DE", TXT_KEY_WELCOME_APPINST, "<span foreground=\"#22cc22\">✓ {name} ist installiert</span>");
    registerEntry("de_DE", TXT_KEY_WELCOME_APPNOINST, "<span foreground=\"#cc2222\">⚠ {name} ist nicht installiert</span>");
    registerEntry("de_DE", TXT_KEY_WELCOME_CHANGELATER, "<i>Du kannst diese später immer in hyprland.lua anpassen.</i>");

    registerEntry("de_DE", TXT_KEY_WELCOME_THANKS, "Danke, aber ich brauche keine Hilfe");
    registerEntry("de_DE", TXT_KEY_WELCOME_BACK, "Zurück");
    registerEntry("de_DE", TXT_KEY_WELCOME_NEXT, "Weiter");
    registerEntry("de_DE", TXT_KEY_WELCOME_OPENTERM, "Terminal öffnen");
    registerEntry("de_DE", TXT_KEY_WELCOME_FINISH, "Fertig");
    registerEntry("de_DE", TXT_KEY_WELCOME_WIKI, "🔗 Wiki öffnen");
    registerEntry("de_DE", TXT_KEY_WELCOME_OPENED, "🔗 In deinem Browser geöffnet");

    // de_CH (Swiss German)
    registerEntry("de_CH", TXT_KEY_WELCOME_TITLE, "Wellkomme bi Hyprland!");
    registerEntry("de_CH", TXT_KEY_WELCOME_GETSTART, "Los loh");
    registerEntry("de_CH", TXT_KEY_WELCOME_APPS, "Standardaawändige");
    registerEntry("de_CH", TXT_KEY_WELCOME_CONFIG, "Eifachi Konfiguration");
    registerEntry("de_CH", TXT_KEY_WELCOME_ECOSYSTEM, "Hypr Ökosystem");
    registerEntry("de_CH", TXT_KEY_WELCOME_THATSIT, "Das wärs!");

    registerEntry("de_CH", TXT_KEY_WELCOME_CONTENT1,
                    R"#(Mer hoffed du gniessisch din Ufenthalt. Om der z hälfe, eifacher Hyprland könne z lehre, hend mer extra för dech es chliises Iistegstutorial vorbereitet.

Falls du dech abentürlech fühlsch, oder bereits en fortgschrettne Benotzer besch, chasch du "Danke, aber ech bruuch kei Helf" unde klicke. Das werd das Fänster schliesse onds nie meh aazeige.

Falls du manuell die Wellkommensapp öffne wottsch, chasch du eifach hyprland-welcome i dinere Konsole uusfüehre.

Klick uf "Wiiter" om zom nöchste Schrett i dinere Iirechtig fortzfahre :)
)#");

    registerEntry("de_CH", TXT_KEY_WELCOME_CONTENT2,
                    R"#(Zerst mömmer paar Päckli installiere, wo du ombedingt bruuchsch dass dis System rechtig funktioniert.
Apps meteme <span foreground="#cc2222">*</span> send <span foreground="red"><i>ombedingt notwändig</i></span> för es funktionierends System. Alli andere send <span foreground="red"><i>drengend</i></span> empfohle, well sie grondlegendi Bestandteil vomene funktionierende System send.
Du chasch au ohni sie wiiter go, aber devo werd abgrote.

Es esch möglech, dass die App gwössnigi vo dine installierte Binaries ned erkönnt. I demm Fall esch es okay, die z ignoriere.

Benotz de <i>Terminal öffne</i> Chnopf, om es Terminal z öffne.
Benotz SUPER+M om Hyprland z verloh.
Onderstötzti Terminals: kitty, alacritty, foot, wezterm, konsole, gnome-terminal, xterm.

<i>Tipp: Fahr met de Muus öber die verschedene Komponänte, om z gseh weli Optione akzeptiert wärded. <span foreground="#22cc22">Grüen</span> bedüütet dass die Komponänte als installiert erkennt worde esch, <span foreground="#22cccc">blau</span> heisst es lauft scho.
Die Lischte aktualisiert automatisch.</i>)#");

    registerEntry("de_CH", TXT_KEY_WELCOME_CONTENT3, R"#(Mer wössed dass ned jede Kitty ond Dolphin bruucht. Wäge demm lömmer dech lo wähle.
Wenn du d Standardiistellige ändere wettsch, denn notz die folgende Dropdowns.)#");

    registerEntry(
        "de_CH", TXT_KEY_WELCOME_CONTENT4,
        R"#(Do du jetzt die grondlegende Apps installiert hesch, wettsch vellecht es paar vo ene automatisch starte. Hyprland started nüd automatisch för dech, das muesch du sälber fest legge.
Goh zu ~/.config/hypr/hyprland.lua ond füeg "hl.exec_cmd("appname")" omgäbe vo "hl.on()" ii, om dini Apps z starte, zom Bispel:
hl.on("hyprland.start", function ()
    hl.exec_cmd("hyprpaper")
    hl.exec_cmd("waybar")
end)

Generell, Apps konfiguriere esch öppis wo du sälber muesch mache. Jedi App wo du installiersch chan en eigeti Konfigurationsdatei ond eigeti Optione ha.

En guete Startponkt esch s Hyprland-Wiki uf https://wiki.hypr.land. Det werd s "Master tutorial" der alles biibrenge ond zo wiitere Dokumentation linke.

Falls du vorgfertigti Iistellige oder "dotfiles" bevorzugsch, chasch du der d "Preconfigured setups" Siite uf em Wiki aaluege, oder online sueche. <span foreground="#cc2222">Wechtige Hiwiis:</span> dotfiles chönd <i>alles</i> uf dim Computer uusfüehre. Stell secher dass du de Quelle vertrousch.)#");

    registerEntry("de_CH", TXT_KEY_WELCOME_CONTENT5, R"#(Hyprland hed es breits Ökosystem vo Apps wo spezifisch deför gmacht worde send.
Ned wie anderi DEs zwengt s dech aber ned, die meiste devo standardmässig z bruuche.

Du chasch die Element separat installiere, ond nor die wo du au bruuchsch.

Lueg im Wiki onder "Hypr Ecosystem" om alli Apps ond ehri Notzig ond Konfiguration z gseh.)#");

    registerEntry("de_CH", TXT_KEY_WELCOME_CONTENT6, R"#(Das wärs för die chlii Iifüehrig! Erkond s Wiki, veli verschedni Apps ond gniess dini Reis!

Do send paar wechtige Standard-Tastekombinatione:
• SUPER + Q <span foreground="#666666">=</span> Terminal
• SUPER + E <span foreground="#666666">=</span> Dateimanager
• SUPER + R <span foreground="#666666">=</span> Launcher
• SUPER + C <span foreground="#666666">=</span> Fänster schliesse
• SUPER + V <span foreground="#666666">=</span> Floating omschalte
• SUPER + M <span foreground="#666666">=</span> Hyprland verloh
• SUPER + [1 - 9] <span foreground="#666666">=</span> Workspace 1 - 9
• SUPER + SHIFT + [1 - 9] <span foreground="#666666">=</span> Fänster zu Workspace 1 - 9 verschiebe
• SUPER + [ ← ↑ ↓ → ] <span foreground="#666666">=</span> Fokus verschiebe

<i>Du chasch die eifach in hyprland.lua verändere.</i>

Danke dass du dech för Hyprland entschede hesch! ❤️)#");

    registerEntry("de_CH", TXT_KEY_WELCOME_RUNNING, "{type}{star}: <span foreground=\"#22cccc\">Lauft</span>: {name}");
    registerEntry("de_CH", TXT_KEY_WELCOME_INSTALLED, "{type}{star}: <span foreground=\"#22cc22\">Installiert</span>: {name}");
    registerEntry("de_CH", TXT_KEY_WELCOME_MISSING, "{type}{star}: <span foreground=\"#cc2222\">Fählt</span>");
    registerEntry("de_CH", TXT_KEY_WELCOME_ACCEPTED, "Akzeptiert: {accepted}");
    registerEntry("de_CH", TXT_KEY_WELCOME_RECOMMENDED, "Empfohle: {recommended}\nAkzeptiert: {accepted}");
    registerEntry("de_CH", TXT_KEY_WELCOME_ERROR1, "Spichere fählgschlage: weder $XDG_CONFIG_HOME no $HOME esch gsetzt");
    registerEntry("de_CH", TXT_KEY_WELCOME_ERROR2, "Spichere fählgschlage: d Lua-Konfiguration hed ned chönne gläse wärde");
    registerEntry("de_CH", TXT_KEY_WELCOME_ERROR3, "Spichere fählgschlage: Konfiguration esch ned Standard, sie hed kei Lua-Standardvariable");
    registerEntry("de_CH", TXT_KEY_WELCOME_ERROR4, "Spichere fählgschlage: d Lua-Konfiguration hed ned chönne göffnet wärde");
    registerEntry("de_CH", TXT_KEY_WELCOME_ERROR5, "Spichere fählgschlage: d Lua-Konfiguration hed ned chönne gschrebe wärde");
    registerEntry("de_CH", TXT_KEY_WELCOME_AUTHAGENT, "Authentifizierigsagent");
    registerEntry("de_CH", TXT_KEY_WELCOME_FILES, "Dateimanager");
    registerEntry("de_CH", TXT_KEY_WELCOME_TERM, "Terminal");
    registerEntry("de_CH", TXT_KEY_WELCOME_PIPEWIRE, "Pipewire");
    registerEntry("de_CH", TXT_KEY_WELCOME_WALLPAPER, "Hendergrondbeld");
    registerEntry("de_CH", TXT_KEY_WELCOME_PORTAL, "XDG Desktop Portal");
    registerEntry("de_CH", TXT_KEY_WELCOME_NOTIF, "Benochrechtigungsdienst");
    registerEntry("de_CH", TXT_KEY_WELCOME_NOTIFNOTE, "Bitte beacht, dass dini Shell (z.B. quickshell) au en eigete Benochrechtigungsdienst cha beinhalte.");
    registerEntry("de_CH", TXT_KEY_WELCOME_SHELL, "Status bar / shell");
    registerEntry("de_CH", TXT_KEY_WELCOME_SHELLNOTE, "För neui Notzer empfähled mer Waybar, för fortgschrettni Quickshell.");
    registerEntry("de_CH", TXT_KEY_WELCOME_LAUNCHER, "App Launcher");
    registerEntry("de_CH", TXT_KEY_WELCOME_CLIPBOARD, "Zwöscheablag");
    registerEntry("de_CH", TXT_KEY_WELCOME_CLIPBOARDNOTE, "wl-copy werd bi de meiste Distributione vo wl-clipboard zor Verfüegig gstellt.");
    registerEntry("de_CH", TXT_KEY_WELCOME_ERROR6, "<span foreground=\"#cc2222\">⚠ Fähler: {error}</span>");
    registerEntry("de_CH", TXT_KEY_WELCOME_APPINST, "<span foreground=\"#22cc22\">✓ {name} esch installiert</span>");
    registerEntry("de_CH", TXT_KEY_WELCOME_APPNOINST, "<span foreground=\"#cc2222\">⚠ {name} esch ned installiert</span>");
    registerEntry("de_CH", TXT_KEY_WELCOME_CHANGELATER, "<i>Du chasch die spöter emmer in hyprland.lua aapasse.</i>");

    registerEntry("de_CH", TXT_KEY_WELCOME_THANKS, "Danke, aber ech bruuch kei Helf");
    registerEntry("de_CH", TXT_KEY_WELCOME_BACK, "Zrogg");
    registerEntry("de_CH", TXT_KEY_WELCOME_NEXT, "Wiiter");
    registerEntry("de_CH", TXT_KEY_WELCOME_OPENTERM, "Terminal öffne");
    registerEntry("de_CH", TXT_KEY_WELCOME_FINISH, "Fertig");
    registerEntry("de_CH", TXT_KEY_WELCOME_WIKI, "🔗 Wiki öffne");
    registerEntry("de_CH", TXT_KEY_WELCOME_OPENED, "🔗 I dim Browser göffnet");
}

std::string I18n::CI18nEngine::localize(eI18nKeys key, const Hyprutils::I18n::translationVarMap& vars) {
    return huEngine->localizeEntry(localeStr, key, vars);
}
