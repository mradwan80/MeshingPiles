#pragma once

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <set>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "DDSstuff.h" //should be replaced by DDS.h later
using namespace std;

struct Pixel
{
	int x, y;
	int PilesOffset;
	int PilesCount;

	Pixel();
	Pixel(int x, int y);

	void SetCoordinates(int xx, int yy);

	int ComputeIndex(int w, int h);

	bool ContainsPiles();

	void GetNbsWithBiggerIndex(vector<Pixel>& pixels, int w, int h, vector<int>& nbpixels, bool OnlyValid = true);

	void GetDirectNbs(vector<Pixel>& pixels, int w, int h, vector<int>& nbpixels, bool OnlyValid = true);

	void GetCrossNbs(vector<Pixel>& pixels, int w, int h, vector<int>& nbpixels, bool OnlyValid = true);

	//the edge of pixel1 crossing pixel2
	static int GetCrossEdge(const Pixel& pixel1, const Pixel& pixel2);

	//the starting vertex of the edge of pixel 2 crossing pixel1
	static int GetCrossVertex(const Pixel& pixel1, const Pixel& pixel2);

	//the two vertices of the face of pixel 2 that faces pixel1
	static void GetFacingVertices(const Pixel& pixel1, const Pixel& pixel2, int& nbv0, int& nbv1);

	//get the index of the face of pixel1, that faces pixel2
	static int GetFacingFace(const Pixel& pixel1, const Pixel& pixel2);

	static bool CoordsWithinRange(int x, int y, int w, int h);

};

struct PointCoordsExt : PointCoords
{
	PointCoordsExt(float px, float py, float pz);
	int CheckAgainstPilePoints(vector<PointCoordsExt>::iterator begin);
};

struct Edge
{
	vector<int>epoints;
	void RemoveDuplicates();
	void SortByZ(const vector<PointCoordsExt>& points);
};

struct PileEdges
{
	vector<set<int>> edges;
	PileEdges();
};

struct Face
{
	int v0, v1, v2, v3;
	Edge e0, e1;

	void FillVerticesFromEdgesToEdges(set<int>& E0, set<int>& E1, vector<PointCoordsExt>& points);
};

struct Triangle
{
	int v0, v1, v2;

	static void Triangulate(const Face& fc, vector<Triangle>& triangles);
};

struct TrigEdge
{
	int v0, v1, v2;

	bool operator < (const TrigEdge& str) const;
};


//specific for ortho projection//
void MeshPiles(vector<PileStruct>& Piles, vector<Pixel>& pixels, const int w, const int h, float left, float right, float bottom, float top, vector<PointCoordsExt>& points, vector<Triangle>& triangles);

//idea: moave to another vector, along with their indices. define how to compare (x,y,z) then index. sort. search for duplicates, all refer to first.
void findDuplicates(vector<PointCoordsExt>& points, vector<PileStruct>& Piles, vector<Pixel>& pixels, const int w, const int h, vector<int>& firstOccurance);

//for each pile, fill 4 trigs: 2 for upper face, 2 for lower. edges are not needed for these
void fillUpperLowerFaces(vector<Pixel>& pixels, vector<int>& firstOccurance, const int w, const int h, vector<Triangle>& triangles);

//in short, if an edge intersects with edges of other piles, add the vertices lying inside the edge to the edge vxs
//each pile has 4 edges. each point falling inside the edge of current pile, is added to the points of this edge.
//piles of 4 direct nb pixels intersect the piles of current pixel in faces. an intersection adds 2 points, each on an edge
//piles of 4 cross nb pixels intersect the piles of current pixel in edges. an intersection adds 1 point on an edge
void InsertPointsOnEdges(vector<PileStruct>& Piles, vector<Pixel>& pixels, vector<int>& firstOccurance, const int w, const int h, vector<PileEdges>& pileEdges);

//each pile has initial 4 faces
//the function just splits each face, depending on the neighboring faces
//each face will have 4 vertices. more vertices are added later in the function
void CreateFaces(vector<PointCoordsExt>& points, vector<PileStruct>& Piles, vector<Pixel>& pixels, vector<int>& firstOccurance, const int w, const int h, vector<PileEdges>& pileEdges, vector<Face>& PilesFaces);

void TriangulateFaces(vector<Face>& pilesFaces, vector<PointCoordsExt>& points, vector<Triangle>& triangles);


bool TrigHasTwoVxsNotThird(Triangle trig, int v0, int v1, int v2, int& third);

bool TrigHasTwoVxs(Triangle trig, int v0, int v1, int& third);

void FindNonManifoldEdges(vector<PointCoordsExt>& points, vector<Triangle>& triangles);

void FixNonManifoldEdges(vector<PointCoordsExt>& points, vector<Triangle>& triangles);

void CheckConnectedComponents(int pnum, vector<Triangle>& triangles);

//each pile has 8 points. will be added to a vector
void fillPoints(vector<PileStruct>& Piles, vector<Pixel>& pixels, const int w, const int h, const int left, const int right, float bottom, float top, vector<PointCoordsExt>& points);
