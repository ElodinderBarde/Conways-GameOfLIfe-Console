
## Projekt: Game of Life (C++ / Windows Console)

Das Programm ist eine **vollständig textbasierte Simulation von Conways Game of Life**, erweitert um:

- **konfigurierbare Parameter** über ein Setup-Menü (z. B. Grid-Größe, Cluster-Intervall),
    
- **Cluster-basierte Initialisierung** (statt Zufallspunktverteilung),
    
- **Stagnations-Logik**, die neue Zellen bei Stillstand einfügt,
    
- **automatische Regeneration**, wenn keine Zellen mehr leben,
    
- **Logging-System** (Debug-Ausgabe in Datei),
    
- **visuelles Rendering in der Konsole** (Unicode / UTF-16),
    
- **ausgeblendeten Cursor** für saubere Darstellung.
    

---

##  **Projektstruktur (Module & Aufgaben)**

### 🔹 `main.cpp`

- Einstiegspunkt des Programms.
    
- Ruft `InitializeApp()` auf → initialisiert alle Komponenten.
    

---

### 🔹 `init.cpp` / `Init.h`

Zentrale Steuerlogik des gesamten Programms. Enthält:

#### 🔸 `InitializeApp()`

1. Setzt Konsolenmodus auf UTF-16 für Unicode-Zeichen.
    
2. Initialisiert den Logger (z. B. `debug.log`).
    
3. Zeigt Setup-Menü (`OpenSetup(setup)`), um Benutzerparameter abzufragen.
    
4. Erstellt ein `GridState`-Objekt basierend auf `setup`.
    
5. Startet die Hauptsimulation mit `Engine(grid, setup)`.
    

#### 🔸 `Engine(GridState &grid, const SetupState &setup)`

Die **Hauptschleife** der Simulation.

Funktionen:

- Startet Generationenzähler (`generation`, `regenCount`).
    
- Erzeugt initiale Cluster mit `CreateCluster(grid)`.
    
- Läuft in Endlosschleife:
    
    - Zählt lebende Zellen.
        
    - Falls keine lebenden Zellen: erzeugt neue zufällige Cluster.
        
    - Falls lebende Zellen vorhanden:
        
        - Führt `NextGeneration()` aus (Lebensregeln).
            
        - Prüft Stagnation (wenn über viele Generationen keine Änderung).
            
        - Erzeugt periodisch (z. B. alle 30 Generationen) neue Cluster.
            
    - Ruft `grid.Print()` zum Rendern auf.
        
    - Nutzt `system("cls")` für Refresh-Effekt (Bildschirm löschen).
        
    - Optional Sleep/Delay für Lesbarkeit.
        

---

#### 🔸 `NextGeneration(GridState &grid, int &genCount)`

Implementiert **Conway’s Regeln**:

- Für jede Zelle:
    
    - Lebende Zelle mit <2 oder >3 Nachbarn stirbt.
        
    - Tote Zelle mit genau 3 Nachbarn wird geboren.
        
- Führt Änderungen auf Kopie (`nextGen`) durch, dann Austausch.
    
- Zählt Geburten, Tode, Lebende.
    
- Erkennt **Stagnation** (keine Änderungen über mehrere Generationen).
    
    - Falls Stagnation → fügt zusätzliche Cluster oder Zellen hinzu.
        
- Schreibt Log-Einträge zu jeder Generation.
    

---

#### 🔸 `CreateAtRandomPosition(GridState &grid)`

- Wählt zufällige Position (max. 100 Versuche).
    
- Falls Zelle dort tot → aktiviert sie (`alive`).
    
- Falls kein Platz → erzwingt eine neue Position.
    
- Nutzt Logger für Debugausgaben.
    

#### 🔸 `CreateCluster(GridState &grid)`

- Platziert eine kleine Gruppe (Cluster) von lebenden Zellen (z. B. 3×3-Bereich).
    
- Erhöht Wahrscheinlichkeit von stabilen Mustern.
    

---

### 🔹 `GridState.cpp` / `.h`

Repräsentiert den **Zustand des Spielfeldes**.

Funktionen:

- Konstruktor: erzeugt 2D-Array (Matrix) mit `CellState::dead`.
    
- `GetM_Width()` / `GetM_Height()` → Abmessungen.
    
- `GetState(x, y)` / `SetState(x, y, state)` → Zugriff auf Zellen.
    
- `Print()` → zeichnet Grid in Konsole (z. B. `█` für lebend, `·` für tot).
    
- Optionale Farb-/Symbolzuordnung via `EnumIcon.h`.
    

---

### 🔹 `Cells.cpp` / `.h`

Definiert **Zellenlogik & Nachbarn**.

Funktionen:

- `CheckNeighbor(grid, x, y)` → zählt lebende Nachbarn (8 Richtungen).
    
- `CellState` (Enum: `alive`, `dead`).
    
- Hilfsfunktionen für Lebensregeln.
    

---

### 🔹 `SetupState.h`

Enthält Konfigurationsparameter des Spiels:

```cpp
struct SetupState {
    int gridWidth;
    int gridHeight;
    int clusterInterval;
    int initialClusters;
};
```

Diese Werte werden im Setup-Menü gesetzt.

---

### 🔹 `DisplayRenderer.cpp` / `.h`

Verantwortlich für **Anzeige und UI-Elemente**:

- Anzeigen von Menüs (Setup, Begrüßung, Anweisungen).
    
- Anzeigen von aktuellen Parametern.
    
- Optional Eingabeaufforderungen für Benutzer.
    

---

### 🔹 `Logger.h`

Ein einfacher **Singleton-Logger**:

- `Init(path)` → öffnet Log-Datei.
    
- `Write(message)` → schreibt Zeitstempel + Nachricht in Datei.
    
- Thread-sicher (typischerweise über `std::mutex` oder sequentiellen Zugriff).
    

Beispielausgaben:

```
[17:12:45] Engine gestartet
[17:12:46] Generation 10: Alive = 57
[17:12:47] Keine Änderung seit 10 Generationen → Neue Zellen entstehen...
```

---

### 🔹 `ConsoleUtils` (empfohlen)

Zusatzmodul mit:

- `HideCursor()`
    
- `ShowCursor()`
    
- `ClearScreen()`
    

→ Dient zur Verbesserung der visuellen Darstellung.

---

## ⚙️ **Ablaufdiagramm (vereinfacht)**

```text
Start
 │
 ▼
InitializeApp()
 │
 ├─> Logger.Init()
 ├─> OpenSetup(setup)
 ├─> GridState grid(setup.width, setup.height)
 │
 ▼
Engine(grid, setup)
 │
 ├─> CreateCluster(grid) x setup.initialClusters
 │
 └─> while(true)
      ├─ Count Alive
      ├─ if none → CreateCluster(grid)
      ├─ else NextGeneration()
      ├─ Every Nth Gen → CreateCluster(grid)
      ├─ grid.Print()
      ├─ ClearScreen()
      └─ Sleep(delay)
```

---

## ✨ **Besonderheiten**

✅ Unicode-fähig (UTF-16-Konsole)  
✅ Logging mit Zeitstempeln  
✅ Konfigurierbar über Setup  
✅ Automatische Wiederbelebung bei Stillstand  
✅ Clustering statt Zufallsverteilung  
✅ Saubere Darstellung (Cursor-Hide, ClearScreen)  
✅ Modulares Design – gut erweiterbar

---


## 🧭 **Architekturübersicht – Game of Life**

|**Ebene / Schicht**|**Datei(en)**|**Hauptverantwortung**|**Abhängigkeiten**|**Wird genutzt von**|
|---|---|---|---|---|
|🟩 **Entry Point**|`main.cpp`|Einstiegspunkt. Ruft `InitializeApp()` auf.|Init.h|–|
|🧱 **Initialisierung**|`init.cpp` / `Init.h`|Initialisiert Programmumgebung, Logger, Setup. Startet Engine-Schleife.|GridState, Logger, SetupState, DisplayRenderer|main.cpp|
|⚙️ **Setup / UI**|`SetupState.h`, `DisplayRenderer.cpp/.h`|Zeigt Konfigurationsmenü, liest Benutzer-Einstellungen (Gridgröße, Intervall etc.)|ConsoleUtils, iostream, struct SetupState|Init.cpp|
|🧩 **Engine / Simulation**|`init.cpp` (Funktion `Engine`)|Führt Hauptschleife aus. Berechnet Generationen, erkennt Stagnation, erzeugt Cluster, schreibt Logs.|GridState, Logger, Cells, SetupState|Init.cpp (Hauptsteuerung)|
|🧬 **Grid-Verwaltung**|`GridState.cpp/.h`|Repräsentiert das Spielfeld (Matrix). Zugriff & Änderung einzelner Zellen, Ausgabe (`Print()`).|Cells.h, structPosition.h|Engine, NextGeneration, Display|
|🔁 **Zelllogik**|`Cells.cpp/.h`|Definiert Zellzustände (`alive`/`dead`), Nachbarschaftsprüfung (`CheckNeighbor()`), Lebensregeln.|EnumIcon.h, GridState|Engine, NextGeneration|
|📦 **Struktur & Position**|`structPosition.h`|Enthält Positionsstrukturen (z. B. `struct Position { int x; int y; }`).|–|GridState, Cells|
|🧮 **Icons & Symbole**|`EnumIcon.h`|Enum für Anzeigezeichen (z. B. `ICON_ALIVE = █`, `ICON_DEAD = ·`).|–|GridState, DisplayRenderer|
|🧰 **Hilfsfunktionen**|`ConsoleUtils.h` _(optional)_|Cursor ausblenden, Konsole leeren, Farben setzen.|Windows.h /|DisplayRenderer, Engine|
|📖 **Logger**|`Logger.h`|Singleton für Logdatei. Schreibt zeitgestempelte Nachrichten.|,|Init, Engine, NextGeneration|
|⚙️ **Setup-Datenmodell**|`SetupState.h`|Enthält Parameter: Gridgröße, Clusteranzahl, Intervall, Delay usw.|–|Init, Engine, DisplayRenderer|

---

## 🔄 **Datenfluss – Kurzdiagramm**

```text
             ┌────────────────┐
             │    main.cpp    │
             └──────┬─────────┘
                    │
                    ▼
           ┌──────────────────┐
           │ InitializeApp()  │
           └──────┬───────────┘
                  │
        ┌─────────┼──────────────────────────┐
        ▼                                        ▼
┌──────────────┐                         ┌────────────────┐
│ Logger::Init │                         │ OpenSetup()    │
└──────────────┘                         └──────┬─────────┘
                                               │
                                               ▼
                                      ┌──────────────────┐
                                      │ SetupState setup │
                                      └──────┬───────────┘
                                             │
                                             ▼
                                      ┌─────────────────────┐
                                      │ GridState grid(...) │
                                      └──────┬──────────────┘
                                             │
                                             ▼
                                    ┌────────────────────────┐
                                    │ Engine(grid, setup)    │
                                    └────────────────────────┘
                                             │
                    ┌────────────────────────┼────────────────────────────┐
                    ▼                        ▼                            ▼
           ┌──────────────┐       ┌────────────────────┐         ┌─────────────────────┐
           │ NextGen()    │       │ CreateCluster()    │         │ Logger::Write()     │
           └──────────────┘       └────────────────────┘         └─────────────────────┘
                    │                        │                            │
                    ▼                        ▼                            ▼
             ┌────────────────────────────────────────────────────────────────────┐
             │ GridState::SetState / GetState / Print                            │
             └────────────────────────────────────────────────────────────────────┘
```

---

## 🧠 **Logische Zusammenhänge**

|**Modul**|**Steuert / nutzt**|**Begründung**|
|---|---|---|
|`Engine`|`NextGeneration`, `CreateCluster`, `Logger`|Hauptschleife|
|`NextGeneration`|`GridState`, `CheckNeighbor`, `Logger`|Lebensregeln|
|`GridState`|`Cells`, `EnumIcon`, `structPosition`|Spielfeldlogik|
|`DisplayRenderer`|`SetupState`, `ConsoleUtils`|Benutzeroberfläche|
|`SetupState`|wird an `Engine()` übergeben|Konfiguration|
|`Logger`|wird global genutzt|Debug und Statusausgabe|
|`ConsoleUtils`|von `DisplayRenderer` oder `Engine` für UI|Darstellung|

---

## **Mögliche Erweiterungen**

|**Ziel**|**Empfohlene Änderung**|
|---|---|
|🕹️ Interaktive Steuerung (Pause, Speed-Up)|Tasten-Abfrage in `Engine()` (z. B. `_kbhit()` + `getch()`)|
|🧭 Menü vor Simulation|`DisplayRenderer` erweitern mit Hauptmenü|
|📈 Statistik|Zähler für „max alive“, „average alive“ etc. im Logger|
|⏱️ Performance|`Sleep` durch Zeitmessung mit `chrono` ersetzen|
|💾 Konfig speichern|`SetupState` in Datei schreiben/lesen|

---
