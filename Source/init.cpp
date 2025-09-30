#include "Init.h"
#include "Cells.h"
#include "DisplayRenderer.h"
#include "Logger.h"
#include "SetupState.h"
#include <chrono>
#include <corecrt_io.h>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <thread>

void InitializeApp()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    HideCursor();
    Logger::Init(L"debug.log");

    bool setupDone = false;
    SetupState setup;
    while (!setupDone)
    {
        setupDone = OpenSetup(setup);
    }
    srand(static_cast<unsigned int>(time(nullptr)));
    CreateGrid(setup);
}

void CreateGrid(const SetupState &setup)
{
    GridState grid(setup.GridWidth, setup.GridHeight);

    Engine(grid, setup);
}

void Engine(GridState &grid, const SetupState &setup)
{
    int refreshRate = 8;

    int generation = 0;
    int regenCount = 0;
    int genCounter = 0;
    Logger::Write(L"Engine gestartet");

    for (int i = 0; i < setup.InitialClusters; ++i)
        CreateCluster(grid);

    Logger::Write(L"Erste Generation erzeugt (" + std::to_wstring(setup.InitialClusters) + L" Zellen erstellt)");

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(refreshRate));
        bool hasAlive = false;
        int aliveCount = 0;

        for (std::size_t y = 0; y < grid.GetM_Height(); ++y)
        {
            for (std::size_t x = 0; x < grid.GetM_Width(); ++x)
            {
                if (grid.GetState(x, y) == CellState::alive)
                {
                    hasAlive = true;
                    aliveCount++;
                }
            }
        }

        Logger::Write(L"Generation " + std::to_wstring(generation) + L": Alive = " + std::to_wstring(aliveCount));
        if (!hasAlive)
        {
            regenCount++;
            Logger::Write(L" Keine Zellen vorhanden → neue zufällig erzeugen (" + std::to_wstring(regenCount) +
                          L". Mal)");

            std::wcout << L" Keine Zellen vorhanden → neue zufällig erzeugen...\n";
            for (int i = 0; i < 50; ++i)
            {

                CreateAtRandomPosition(grid);
            }
        }
        else
        {

            NextGeneration(grid, genCounter);
        }
        Logger::Write(L"[DEBUG] Prüfe Intervall: Generation = " + std::to_wstring(generation) + L", Interval = " +
                      std::to_wstring(setup.ClusterInterval));

        if (generation > 0 && generation % setup.ClusterInterval == 0)
        {
            Logger::Write(L"[DEBUG] Intervall erreicht, nach if: Generation = " + std::to_wstring(generation) +
                          L", Interval = " + std::to_wstring(setup.ClusterInterval));

            Logger::Write(L" Intervall erreicht – Erzeuge neuen Cluster (Generation " + std::to_wstring(generation) +
                          L")");
            CreateCluster(grid);
            for (int i = 0; i < 10; i++)
            {
                CreateAtRandomPosition(grid);
            }
        }

        auto system1 = system;
        system1("cls");
        grid.Print();
        generation++;
    }
}

void CreateAtRandomPosition(GridState &grid)
{
    int width = grid.GetM_Width();
    int height = grid.GetM_Height();

    for (int tries = 0; tries < 100; ++tries)
    {
        int x = GetRandomNumberInRange(width);
        int y = GetRandomNumberInRange(height);

        if (grid.GetState(x, y) == CellState::dead)
        {
            grid.SetState(x, y, CellState::alive);
            Logger::Write(L"Neue Zelle bei (" + std::to_wstring(x) + L"," + std::to_wstring(y) + L") erstellt");
            return;
        }
    }

    int x = GetRandomNumberInRange(width);
    int y = GetRandomNumberInRange(height);
    grid.SetState(x, y, CellState::alive);
    Logger::Write(L"[WARNUNG] Kein freies Feld nach 100 Versuchen – erzwungene Platzierung bei (" + std::to_wstring(x) +
                  L"," + std::to_wstring(y) + L")");
}

int GetRandomNumberInRange(int value)
{
    return rand() % value;
}

void NextGeneration(GridState &grid, int &genCount)
{
    const std::size_t width = grid.GetM_Width();
    const std::size_t height = grid.GetM_Height();
    static int stagnationCounter = 0;

    int born = 0, died = 0, aliveNext = 0;
    bool changed = false;

    CellState **nextGen = new CellState *[height];
    for (std::size_t i = 0; i < height; ++i)
        nextGen[i] = new CellState[width];

    for (std::size_t y = 0; y < height; ++y)
    {
        for (std::size_t x = 0; x < width; ++x)
        {
            const int neighbors = CheckNeighbor(grid, x, y);
            const CellState current = grid.GetState(x, y);
            CellState nextState = current;

            if (current == CellState::alive)
            {
                if (neighbors < 2 || neighbors > 3)
                {
                    nextState = CellState::dead;
                    died++;
                }
            }
            else if (neighbors == 3)
            {
                nextState = CellState::alive;
                born++;
            }

            if (nextState != current)
                changed = true;

            nextGen[y][x] = nextState;
            if (nextState == CellState::alive)
                aliveNext++;
        }
    }

    if (!changed)
    {
        stagnationCounter++;
        Logger::Write(L"Keine Änderung in Generation " + std::to_wstring(genCount) + L" (Zähler: " +
                      std::to_wstring(stagnationCounter) + L")");

        if (stagnationCounter >= 10)
        {
            Logger::Write(L"🔄 Stagnation erkannt → Neue Cluster werden erzeugt...");
            for (int i = 0; i < 3; ++i)
                CreateCluster(grid);
            stagnationCounter = 0;
        }
    }
    else
    {
        stagnationCounter = 0;
    }

    for (std::size_t y = 0; y < height; ++y)
        for (std::size_t x = 0; x < width; ++x)
            grid.SetState(x, y, nextGen[y][x]);

    for (std::size_t i = 0; i < height; ++i)
        delete[] nextGen[i];
    delete[] nextGen;

    Logger::Write(L"NextGeneration: born = " + std::to_wstring(born) + L", died = " + std::to_wstring(died) +
                  L", alive = " + std::to_wstring(aliveNext));

    genCount++;
}

void CreateCluster(GridState &grid)
{
    int width = grid.GetM_Width();
    int height = grid.GetM_Height();

    int x = GetRandomNumberInRange(width - 2);
    int y = GetRandomNumberInRange(height - 2);

    grid.SetState(x, y, CellState::alive);
    grid.SetState(x + 1, y, CellState::alive);
    grid.SetState(x, y + 1, CellState::alive);
    grid.SetState(x + 1, y + 1, CellState::alive);
}

bool OpenSetup(SetupState &setup)
{
    DisplayRenderer renderer;
    renderer.RenderSetupScreen(setup);
    std::wcout << std::endl;
    std::wcout << std::endl;
    std::wcout << std::endl;
    std::wcout << L"....";
    wchar_t input;
    std::wcin >> input;
    input = towlower(input);

    switch (input)
    {
    case L'w':
        std::wcout << L"Width: ";
        std::wcin >> setup.GridWidth;
        break;
    case L'h':
        std::wcout << L"Height: ";
        std::wcin >> setup.GridHeight;
        break;
    case L'i':
        std::wcout << L"Amount ->add new Cluster-Intervall: ";
        std::wcin >> setup.ClusterInterval;
        break;
    case L'c':
        std::wcout << L"Amount -> Start-Cluster: ";
        std::wcin >> setup.InitialClusters;
        break;
    case L'\n':
    case L'\r':
    case L's':
        // Start
        return true; // Fertig mit Setup
    case L'q':
        exit(0);
    default:
        std::wcout << L"Ungültige Eingabe.\n";
        break;
    }
    system("cls");

    Logger::Write(L"Setup abgeschlossen. Starte Simulation mit " + std::to_wstring(setup.GridWidth) + L"x" +
                  std::to_wstring(setup.GridHeight) + L", Cluster-Intervall: " +
                  std::to_wstring(setup.ClusterInterval));

    return false;
}
