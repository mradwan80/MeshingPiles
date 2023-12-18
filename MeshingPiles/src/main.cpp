//very important:
//this is copied from D:\work\Visual Studio 2015\Projects\CagesDDS\CagesDDS\src\main.cpp. 
// should get back to this file, if we need something that is perhaps deleted
//later, should create a new CagesDDS, and use the logic in this file

#include "meshing.h"
#include "read_input.h"
#include "write_output.h"
using namespace std;



void main()
{

	//hard coded input
	/*std::string file_stem = "example9";
	bool reverse = false;
	double minx, maxx, miny, maxy;
	minx = 0.5; maxx = 449.5;
	miny = 0.5; maxy = 449.5; //double miny = -60.0, maxy = 60.0;
	int GlobalW = 512;
	int GlobalH = 512;*/


	/*std::string file_stem = "bunny2";
	bool reverse = true;
	double minx, maxx, miny, maxy;
	minx = 0.0; maxx = 1.0;
	miny = 0.0; maxy = 1.0;
	int GlobalW = 1024;
	int GlobalH = 1024;*/

	/*std::string file_stem = "lion";
	bool reverse = true;
	double minx, maxx, miny, maxy;
	minx = 0.0; maxx = 10000.0;
	miny = 0.0; maxy = 10000.0;
	int GlobalW = 1024;
	int GlobalH = 1024;*/

	/*std::string file_stem = "dragon2";
	bool reverse = true;
	double minx, maxx, miny, maxy;
	minx = -0.25; maxx = 0.25;
	miny = -0.25; maxy = 0.25;
	int GlobalW = 1024;
	int GlobalH = 1024;*/

	std::string file_stem = "elephant";
	bool reverse = true;
	double minx, maxx, miny, maxy;
	minx = -0.5; maxx = 0.5;
	miny = -0.5; maxy = 0.5;
	int GlobalW = 512;
	int GlobalH = 512;

	/*std::string file_stem = "lucy";
	bool reverse = true;
	double minx, maxx, miny, maxy;
	minx = 0.0; maxx = 100.0;
	miny = 0.0; maxy = 100.0;
	int GlobalW = 1024;
	int GlobalH = 1024;*/

	///////////////////////
	///////////////////////
	///////////////////////

	int cellsNum = GlobalW * GlobalH;
	double cellW = (maxx - minx) / GlobalW;
	double cellH = (maxy - miny) / GlobalH;

	vector<int>PilesCount(cellsNum, 0);
	vector<int>PilesOffset(cellsNum);
	vector<PileStruct> Piles;
	int PilesNum;

	//read points, and calc other vars
	readPiles(file_stem, cellsNum, PilesNum, PilesCount, PilesOffset, Piles); //in future, should read points, and compute DDS

	if (reverse)
	{

		for (int cell = 0; cell < cellsNum; cell++)
		{
			if (PilesCount[cell] > 1)
				std::reverse(Piles.begin() + PilesOffset[cell], Piles.begin() + PilesOffset[cell] + PilesCount[cell]);
		}
	}

	//create and fill my own structs. (***) not sure if they are going to be of any importance !!! for now, only good for visualizing original piles
	vector<vector<PileStruct>>cellsPiles(cellsNum);
	for (int cell = 0; cell < cellsNum; cell++)
	{
		for (int pileIndex = PilesOffset[cell]; pileIndex < PilesOffset[cell] + PilesCount[cell]; pileIndex++)
		{
			PileStruct pile = Piles[pileIndex];
			cellsPiles[cell].push_back(pile);
		}
	}

	//visualize piles//
	visualizeOriginalPiles(file_stem, GlobalW, GlobalH, cellW, cellH, cellsPiles, PilesNum, cellsNum, minx, maxx, miny, maxy);
	cout << "wirting original piles finished\n";
	//return;

	//continue filling my structs
	vector<Pixel> pixels(GlobalW * GlobalH);
	for (int px = 0; px < GlobalW; px++)
	{
		for (int py = 0; py < GlobalH; py++)
		{
			int pixelIndex = py * GlobalW + px;
			pixels[pixelIndex].SetCoordinates(px, py);
			pixels[pixelIndex].PilesCount = PilesCount[pixelIndex];
			pixels[pixelIndex].PilesOffset = PilesOffset[pixelIndex];
		}
	}
	for (int pxlIndex = 0; pxlIndex < pixels.size(); pxlIndex++)
	{

	}

	//create a mesh//
	vector<PointCoordsExt> points;
	vector<Triangle> triangles;
	MeshPiles(Piles, pixels, GlobalW, GlobalH, minx, maxx, miny, maxy, points, triangles);
	cout << "meshing finished\n";

	//write output
	writeOutput(file_stem, points, triangles);
	cout << "wirting output piles finished\n";


	//meshing logic. 
	//for each pile, make 4 trigs (2 faces)
	//for upper face, check the 4 neighbors
	//if a (first) pile intersects, but our upper is above, then connect with 2 trigs
	//if a (first) pile intersects, but our upper is covered, ignore this face
	//stop when a first pile is found
	//if pile does not interect anything, connect with lower !
	//for lower face, check the 4 neighbors (from down to up!)
	//if a (first) pile intersects, but our lower is below, then connect with 2 trigs
	//if a (first) pile intersects, but our lower is covered, ignore this face
	//check the pile below and the 4 neighbors. 
	//if a pile covers lower/upper, then connect

	//decimate (how?)

}

