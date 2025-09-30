#pragma once
#include "GridState.h"
#include "SetupState.h"

void CreateGrid(const SetupState &setup);
void InitializeApp();
void Engine(GridState &grid, const SetupState &setup);
void NextGeneration(GridState &grid, int &genCounter);
void CreateCluster(GridState &grid);

void CreateAtRandomPosition(GridState &grid);
int GetRandomNumberInRange(const int value);
bool OpenSetup(SetupState &setup);
