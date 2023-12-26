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

void checkPiles(int cellsNum, int& PilesNum, vector<int>& PilesCount, vector<int>& PilesOffset, vector<PileStruct>& Piles)
{
	for (int i = 0; i < cellsNum; i++)
	{
		if (PilesCount[i] == 0 || PilesCount[i] == 1)
			continue;

		for (int p = 0; p < PilesCount[i] - 1; p++)
		{
			int pileIndex = PilesOffset[i] + p;
			if (Piles[pileIndex].end == Piles[pileIndex + 1].start)
				pileIndex = pileIndex;
		}
	}

}
