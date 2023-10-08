#pragma once
#include "meshing.h"

void readPiles(std::string file_stem, int cellsNum, int& PilesNum, vector<int>& PilesCount, vector<int>& PilesOffset, vector<PileStruct>& Piles);
