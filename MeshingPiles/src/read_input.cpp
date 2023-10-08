#include "read_input.h"

void readPiles(std::string file_stem, int cellsNum, int& PilesNum, vector<int>& PilesCount, vector<int>& PilesOffset, vector<PileStruct>& Piles)
{
//read piles
std::string ifilename = "inputs//" + file_stem + ".txt";
ifstream ifile(ifilename.c_str(), std::ios_base::in);

ifile >> PilesNum;
//PilesNum = 6337;
Piles.resize(PilesNum);
for (int p = 0; p < PilesNum; p++)
{
	ifile >> Piles[p].start >> Piles[p].end >> Piles[p].pixel;
	PilesCount[Piles[p].pixel]++;
}

//round all piles z coordinates

PilesOffset[0] = 0;
for (int i = 1; i < cellsNum; i++)
{
	PilesOffset[i] = PilesOffset[i - 1] + PilesCount[i - 1];
}

}
