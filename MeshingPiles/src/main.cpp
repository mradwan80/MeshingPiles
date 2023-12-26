//very important:
//this is copied from D:\work\Visual Studio 2015\Projects\CagesDDS\CagesDDS\src\main.cpp. 
// should get back to this file, if we need something that is perhaps deleted
//later, should create a new CagesDDS, and use the logic in this file

#include "meshing.h"
#include "read_input.h"
#include "write_output.h"
using namespace std;


void convert();

void main()
{
	//convert();
	//return;

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

	checkPiles(cellsNum, PilesNum, PilesCount, PilesOffset, Piles);

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

void convert()
{
	struct Point
	{
		double x, y, z;
	};

	struct Pile
	{
		double zstart, zend;
	};



	//hard coded input
	//std::string file_stem = "bunny2";
	//std::string file_stem = "lion";
	//std::string file_stem = "dragon2";
	//std::string file_stem = "lucy";
	std::string file_stem = "elephant";
	double minx, maxx, miny, maxy;
	int GlobalW = 1024;
	int GlobalH = 1024;

	vector<vector<vector<Pile>>>Grid;
	Grid.resize(GlobalW);
	for (int i = 0; i < GlobalW; i++) Grid[i].resize(GlobalH);

	int cellsNum = GlobalW * GlobalH;

	double cellW;
	double cellH;

	vector<Point> points;


	std::string ifilename = "inputs//ply//" + file_stem + "_raw_piles.txt";
	ifstream ifile(ifilename.c_str(), std::ios_base::in);
	int pnum, tnum;
	{
		double x, y, z;
		double minx1, minx2, miny1, miny2;
		minx1 = std::numeric_limits<double>::max();
		minx2 = std::numeric_limits<double>::max();
		miny1 = std::numeric_limits<double>::max();
		miny2 = std::numeric_limits<double>::max();

		maxx = std::numeric_limits<double>::lowest();
		maxy = std::numeric_limits<double>::lowest();

		ifile >> pnum >> tnum;

		points.resize(pnum);

		for (int p = 0; p < pnum; p++)
		{
			ifile >> x >> y >> z;


			if (x > maxx)
				maxx = x;
			if (y > maxy)
				maxy = y;

			if (x < minx1)
			{
				minx2 = minx1;
				minx1 = x;
			}
			else if (x < minx2 && x != minx1)
			{
				minx2 = x;
			}

			if (y < miny1)
			{
				miny2 = miny1;
				miny1 = y;
			}
			else if (y < miny2 && y != miny1)
			{
				miny2 = y;
			}

			points[p].x = x;
			points[p].y = y;
			points[p].z = z;
		}

		minx = minx1;
		miny = miny1;

		cellW = minx2 - minx1;
		cellH = miny2 - miny1;

		ifile.close();
	}

	ifile.open(ifilename.c_str(), std::ios_base::in);
	int PilesNum = 0;
	{
		ifile >> pnum >> tnum;

		double x, y, z;
		for (int p = 0; p < pnum / 8; p++)
		{
			ifile >> x >> y >> z;

			double z1 = z;

			int gridx = (x - minx) / cellW + 0.5;
			int gridy = (y - miny) / cellH + 0.5;


			ifile >> x >> y >> z;
			ifile >> x >> y >> z;
			ifile >> x >> y >> z;
			ifile >> x >> y >> z;

			double z2 = z;

			Pile pile;
			if (z1 < z2)
			{
				pile.zstart = z1;
				pile.zend = z2;
			}
			else
			{
				pile.zstart = z2;
				pile.zend = z1;
			}



			ifile >> x >> y >> z;
			ifile >> x >> y >> z;
			ifile >> x >> y >> z;


			Grid[gridx][gridy].push_back(pile);

			PilesNum++;
		}
	}


	{

		std::string ofilename = "inputs//" + file_stem + ".txt";
		ofstream ofile(ofilename.c_str(), std::ios_base::out);
		//write header
		ofile << PilesNum << "\n";

		for (int j = 0; j < GlobalH; j++)
		{
			for (int i = 0; i < GlobalW; i++)
			{
				for (int p = 0; p < Grid[i][j].size(); p++)
				{
					int cell = j * GlobalW + i;
					ofile << Grid[i][j][p].zstart << " " << Grid[i][j][p].zend << " " << cell << "\n";
				}
			}
		}
	}

}