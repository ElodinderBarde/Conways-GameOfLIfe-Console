#include "DisplayRenderer.h"

void DisplayRenderer::RenderSetupScreen(const SetupState &state)
{
    ClearScreen();
    DrawBorder();
    PrintAtPosition(1, 2, CenterText(L"GAME OF LIFE - SETUP", ConsoleWidth - 2));

    PrintAtPosition(2, 5, L"Grid Width:  " + std::to_wstring(state.GridWidth));
    PrintAtPosition(2, 6, L"Grid Height: " + std::to_wstring(state.GridHeight));
    PrintAtPosition(2, 7, L"Cluster Interval: " + std::to_wstring(state.ClusterInterval) + L" *");
    PrintAtPosition(2, 8, L"Initial Clusters: " + std::to_wstring(state.InitialClusters) + L" Clusters");

    PrintAtPosition(2, 10, L"Commands:");
    PrintAtPosition(4, 11, L"[W] Change Width");
    PrintAtPosition(4, 12, L"[H] Change Height");
    PrintAtPosition(4, 13, L"[I] Change Interval");
    PrintAtPosition(4, 14, L"[C] Change Clusters");
    PrintAtPosition(4, 16, L"[S] Start Simulation");
    PrintAtPosition(4, 17, L"[Q] Quit");

    PrintAtPosition(4, 20, L"Please use 'CTRL + MouseWheel' to ZoomOut after Confirming your Settings");
    PrintAtPosition(4, 22, L"*Intervals before Adding new Clusters");

    DrawBottomBorder();
}
