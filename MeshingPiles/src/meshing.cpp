#include "meshing.h"


////////
//struct Pixel
////////

Pixel::Pixel() {} //why necessary?
Pixel::Pixel(int x, int y) :x(x), y(y) {}

void Pixel::SetCoordinates(int xx, int yy)
{
	x = xx;
	y = yy;
}

int Pixel::ComputeIndex(int w, int h)
{
	return y * w + x;
}

int Pixel::GetX()
{
	return x;
}

int Pixel::GetY()
{
	return y;
}

int Pixel::GetX(int index, int w, int h)
{
	return index % w;
}

int Pixel::GetY(int index, int w, int h)
{
	return index / w;
}

bool Pixel::ContainsPiles()
{
	return PilesCount > 0;
}

void Pixel::GetNbsWithBiggerIndex(vector<Pixel>& pixels, int w, int h, vector<int>& nbpixels, bool OnlyValid)
{
	vector<Pixel> nbs{ Pixel(x + 1, y), Pixel(x - 1,y + 1),Pixel(x,y + 1),Pixel(x + 1,y + 1) };

	for (auto nb : nbs)
	{
		int nbIndex = nb.ComputeIndex(w, h);
		if (Pixel::CoordsWithinRange(nb.x, nb.y, w, h) && pixels[nbIndex].ContainsPiles())
			nbpixels.push_back(nbIndex);
		else if (!OnlyValid)
			nbpixels.push_back(-1);

	}
}

void Pixel::GetDirectNbs(vector<Pixel>& pixels, int w, int h, vector<int>& nbpixels, bool OnlyValid)
{
	vector<Pixel> nbs{ Pixel(x , y - 1), Pixel(x + 1,y),Pixel(x,y + 1),Pixel(x - 1,y) };

	for (auto nb : nbs)
	{
		int nbIndex = nb.ComputeIndex(w, h);
		if (Pixel::CoordsWithinRange(nb.x, nb.y, w, h) && pixels[nbIndex].ContainsPiles())
			nbpixels.push_back(nbIndex);
		else if (!OnlyValid)
			//nbpixels.push_back(nbIndex);
			nbpixels.push_back(-1);

	}

}

void Pixel::GetCrossNbs(vector<Pixel>& pixels, int w, int h, vector<int>& nbpixels, bool OnlyValid)
{
	vector<Pixel> nbs{ Pixel(x - 1 , y - 1), Pixel(x + 1,y - 1),Pixel(x + 1,y + 1),Pixel(x - 1,y + 1) };

	for (auto nb : nbs)
	{
		int nbIndex = nb.ComputeIndex(w, h);
		if (Pixel::CoordsWithinRange(nb.x, nb.y, w, h) && pixels[nbIndex].ContainsPiles())
			nbpixels.push_back(nbIndex);
		else if (!OnlyValid)
			nbpixels.push_back(nbIndex);
	}


}

//the edge of pixel1 crossing pixel2
int Pixel::GetCrossEdge(const Pixel& pixel1, const Pixel& pixel2)
{
	if ((pixel2.x - pixel1.x == -1) && (pixel2.y - pixel1.y == -1))
		return 0;
	else if ((pixel2.x - pixel1.x == 1) && (pixel2.y - pixel1.y == -1))
		return 1;
	else if ((pixel2.x - pixel1.x == 1) && (pixel2.y - pixel1.y == 1))
		return 2;
	else if ((pixel2.x - pixel1.x == -1) && (pixel2.y - pixel1.y == 1))
		return 3;
	else
		return -1;

}

//the starting vertex of the edge of pixel 2 crossing pixel1
int Pixel::GetCrossVertex(const Pixel& pixel1, const Pixel& pixel2)
{
	if ((pixel2.x - pixel1.x == -1) && (pixel2.y - pixel1.y == -1))
		return 2;
	else if ((pixel2.x - pixel1.x == 1) && (pixel2.y - pixel1.y == -1))
		return 3;
	else if ((pixel2.x - pixel1.x == 1) && (pixel2.y - pixel1.y == 1))
		return 0;
	else if ((pixel2.x - pixel1.x == -1) && (pixel2.y - pixel1.y == 1))
		return 1;
	else
		return -1;
}

//the two vertices of the face of pixel 2 that faces pixel1
void Pixel::GetFacingVertices(const Pixel& pixel1, const Pixel& pixel2, int& nbv0, int& nbv1)
{
	if ((pixel2.x - pixel1.x == 0) && (pixel2.y - pixel1.y == -1))
	{
		nbv0 = 3; nbv1 = 2;
	}
	else if ((pixel2.x - pixel1.x == 1) && (pixel2.y - pixel1.y == 0))
	{
		nbv0 = 0; nbv1 = 3;
	}
	else if ((pixel2.x - pixel1.x == 0) && (pixel2.y - pixel1.y == 1))
	{
		nbv0 = 1; nbv1 = 0;
	}
	else if ((pixel2.x - pixel1.x == -1) && (pixel2.y - pixel1.y == 0))
	{
		nbv0 = 2; nbv1 = 1;
	}
	else
	{
		nbv0 = -1; nbv1 = -1;
	}
}

//get the index of the face of pixel1, that faces pixel2
int Pixel::GetFacingFace(const Pixel& pixel1, const Pixel& pixel2)
{

	if ((pixel2.x - pixel1.x == 0) && (pixel2.y - pixel1.y == -1))
		return 0;
	else if ((pixel2.x - pixel1.x == 1) && (pixel2.y - pixel1.y == 0))
		return 1;
	else if ((pixel2.x - pixel1.x == 0) && (pixel2.y - pixel1.y == 1))
		return 2;
	else if ((pixel2.x - pixel1.x == -1) && (pixel2.y - pixel1.y == 0))
		return 3;
	else
		return -1;
}

bool Pixel::CoordsWithinRange(int x, int y, int w, int h)
{
	return (x >= 0 && x < w&& y >= 0 && y < h);
}

bool Pixel::CoordsWithinRange(int index, int w, int h)
{
	int x = Pixel::GetX(index, w, h);
	int y = Pixel::GetY(index, w, h);
	return CoordsWithinRange(x, y, w, h);
}

////////
//struct PointCoordsExt
////////


//PointCoordsExt(float x, float y, float z) { PointCoords(x, y, z); }
PointCoordsExt::PointCoordsExt(float px, float py, float pz) { x = px; y = py; z = pz; }

int PointCoordsExt::CheckAgainstPilePoints(vector<PointCoordsExt>::iterator begin)
{
	vector<PointCoordsExt>::iterator end = begin + 8;
	for (vector<PointCoordsExt>::iterator it = begin; it != end; it++)
	{
		if (x == it->x && y == it->y && z == it->z)
			return it - begin;
	}
	return -1;
}

////////
//struct Edge
////////

void Edge::RemoveDuplicates()
{
	//remove duplicates
	auto end0 = epoints.end();
	for (auto it = epoints.begin(); it != end0; ++it) {
		end0 = std::remove(it + 1, end0, *it);
	}
}

void Edge::SortByZ(const vector<PointCoordsExt>& points)
{
	for (int i = 0; i < epoints.size(); i++)
	{
		int minind = -1;
		float min = points[epoints[i]].z;
		for (int j = i + 1; j < epoints.size(); j++)
		{
			if (points[epoints[j]].z > min)
			{
				min = points[epoints[j]].z;
				minind = j;
			}
		}

		if (minind != -1)
		{
			int tmp = epoints[minind];
			epoints[minind] = epoints[i];
			epoints[i] = tmp;
		}

	}

}

////////
//methods of other structs
////////

PileEdges::PileEdges()
{
	edges.resize(4);
}

void Face::FillVerticesFromEdgesToEdges(set<int>& E0, set<int>& E1, vector<PointCoordsExt>& points)
{
	//loop over vertices of E0. assign them to e0
	for (int v : E0)
	{
		if (points[v0].z <= points[v].z && points[v].z <= points[v2].z)
			e0.epoints.push_back(v);

	}
	//loop over vertices of e1. assign them to e1 of faces//
	for (int v : E1)
	{
		if (points[v0].z <= points[v].z && points[v].z <= points[v2].z)
			e1.epoints.push_back(v);
	}
}

void Triangle::Triangulate(const Face& fc, vector<Triangle>& triangles)
{

	//start with base0 and base1, then advance them. could happen that one reaches end before the other, which is handled too.

	int e0size = fc.e0.epoints.size();
	int e1size = fc.e1.epoints.size();

	bool BothEnded = false;
	int base0 = 0;
	int base1 = 0;
	int ptr0, ptr1;
	while (!BothEnded)
	{

		if (base0 < e0size - 1)
			ptr0 = base0 + 1;
		if (base1 < e1size - 1)
			ptr1 = base1 + 1;

		//triangulate with base0,base1,ptr0,ptr1//
		if (base1 != ptr1)
		{
			Triangle t{ fc.e0.epoints[base0] , fc.e1.epoints[ptr1], fc.e1.epoints[base1], fc.pile, fc.face };
			triangles.push_back(t);
		}
		if (base0 != ptr0)
		{
			Triangle t{ fc.e0.epoints[base0], fc.e0.epoints[ptr0], fc.e1.epoints[ptr1], fc.pile, fc.face };
			triangles.push_back(t);
		}

		base0 = ptr0;
		base1 = ptr1;

		if (base0 == e0size - 1 && base1 == e1size - 1)
			BothEnded = true;
	}

}

bool TrigEdge::operator < (const TrigEdge& str) const
{
	return ((v0 < str.v0) || (v0 == str.v0 && v1 < str.v1));
}

/////////////////////////////
/////////////////////////////

void MeshPiles(vector<PileStruct>& Piles, vector<Pixel>& pixels, const int w, const int h, float left, float right, float bottom, float top, vector<PointCoordsExt>& points, vector<Triangle>& triangles)
{

	//compute vxs and trigs from piles size
	//int pnum = Piles.size()/100;
	int pnum = Piles.size();
	int vnum = pnum * 8;
	int tnum = 0;


	//starting to use points
	points.reserve(pnum);
	fillPoints(Piles, pixels, w, h, left, right, bottom, top, points);
	cout << "points coords calculations ended\n";


	//find duplicates//
	vector<int>firstOccurance(points.size());
	findDuplicates(points, Piles, pixels, w, h, firstOccurance);
	cout << "finding duplicates ended\n";

	/////////////////
	//now the faces//
	/////////////////

	//starting to use triangles, each triangls has 3 indeces of points


	//upper and lower faces//
	fillUpperLowerFaces(pixels, firstOccurance, w, h, triangles);
	cout << "upper and lower faces ended\n";


	//points on edges//
	//fill EdgesPoints. each pile has 4 edges, and each edge has a set of vertices//
	vector<PileEdges>pileEdges(Piles.size());
	InsertPointsOnEdges(Piles, pixels, firstOccurance, w, h, pileEdges);
	cout << "inserting points on edges ended\n";


	//divide into faces, and distribute points of edges
	//loop over pixels
	//loop over piles
	//for each, loop over piles of 4 direct nbs
	//consider current pile a face
	//each intersecting pile go through the faces, divides one of them into one or two faces
	//after finish, fill the points of the 2 edges of the face

	vector<Face>pilesFaces; pilesFaces.reserve(Piles.size());//could be larger or smaller//

	CreateFaces(points, Piles, pixels, firstOccurance, w, h, pileEdges, pilesFaces);
	cout << "creating faces ended\n";

	//triangulate faces, using vxs of edges
	//for each face, sort vxs of each edge by z. then 
	TriangulateFaces(pilesFaces, points, triangles);
	cout << "triangulating faces ended\n";

	FindNonManifoldEdges(points, triangles);
	FindEdgesOnOneTriangle(points, triangles);


	FixNonManifoldsNew(points, triangles);

	FindNonManifoldEdges(points, triangles);
	FindEdgesOnOneTriangle(points, triangles);

	CheckConnectedComponents(points.size(), triangles);


	//for testing purposes
	/*vector<triangle>triangles2(5);
	triangles2[0].v0 = 0; triangles2[0].v1 = 1; triangles2[0].v2 = 2;
	triangles2[1].v0 = 3; triangles2[1].v1 = 4; triangles2[1].v2 = 5;
	triangles2[2].v0 = 1; triangles2[2].v1 = 2; triangles2[2].v2 = 4;
	triangles2[3].v0 = 4; triangles2[3].v1 = 5; triangles2[3].v2 = 6;
	triangles2[4].v0 = 7; triangles2[4].v1 = 8; triangles2[4].v2 = 9;
	CheckConnectedComponents(10, triangles2);*/

	//FindNonManifoldEdges(triangles, points.size()); //use this to find non manifold edges//

}

//idea: moave to another vector, along with their indices. define how to compare (x,y,z) then index. sort. search for duplicates, all refer to first.
void findDuplicates(vector<PointCoordsExt>& points, vector<PileStruct>& Piles, vector<Pixel>& pixels, const int w, const int h, vector<int>& firstOccurance)
{
	for (int p = 0; p < points.size(); p++)
	{
		firstOccurance[p] = p;
	}

	for (int pixelIndex = 0; pixelIndex < w * h; pixelIndex++)
	{

		if (!pixels[pixelIndex].ContainsPiles())
			continue;

		int pixelPilesOffset = pixels[pixelIndex].PilesOffset;

		//make 4 direct nbs//
		vector<int>nbpixels;				//the 4 nbs whose indeces are bigger than the index of the pixel
		pixels[pixelIndex].GetNbsWithBiggerIndex(pixels, w, h, nbpixels);

		for (int pilePixelIndex = 0; pilePixelIndex < pixels[pixelIndex].PilesCount; pilePixelIndex++) //go through each pile of the pixel
		{

			int pileIndex = pixelPilesOffset + pilePixelIndex;
			int vxoffset = 8 * pileIndex;

			for (int nb = 0; nb < nbpixels.size(); nb++) //go through each neighbor of the pixel (we are still inside the pixel)
			{
				int nbpixelIndex = nbpixels[nb];

				bool intersection = false;
				for (int nbpilePixelIndex = 0; nbpilePixelIndex < pixels[nbpixelIndex].PilesCount && !intersection; nbpilePixelIndex++) //go through each pile of the nb pixel
				{

					int nbpileIndex = pixels[nbpixelIndex].PilesOffset + nbpilePixelIndex;
					int nbvxoffset = 8 * nbpileIndex;

					int ret;

					//check each point of the 8 of the nb pile, against the points of the pile
					for (int j = nbvxoffset; j < nbvxoffset + 8; j++)
					{
						ret = points[j].CheckAgainstPilePoints(points.begin() + vxoffset);
						if (ret != -1) firstOccurance[j] = firstOccurance[ret + vxoffset];
					}

				}
			}

		}
	}
}

//for each pile, fill 4 trigs: 2 for upper face, 2 for lower. edges are not needed for these
void fillUpperLowerFaces(vector<Pixel>& pixels, vector<int>& firstOccurance, const int w, const int h, vector<Triangle>& triangles)
{
	for (int pixelIndex = 0; pixelIndex < w * h; pixelIndex++)
	{

		if (!pixels[pixelIndex].ContainsPiles())
			continue;

		//piles of current pixel//
		for (int pilePixelIndex = 0; pilePixelIndex < pixels[pixelIndex].PilesCount; pilePixelIndex++)
		{

			int pileIndex = pixels[pixelIndex].PilesOffset + pilePixelIndex;
			int pntoffset = 8 * pileIndex;

			//make 4 trigs for upper and lower faces//
			triangles.push_back(Triangle{ firstOccurance[pntoffset], firstOccurance[pntoffset + 2], firstOccurance[pntoffset + 1] });
			triangles.push_back(Triangle{ firstOccurance[pntoffset], firstOccurance[pntoffset + 3], firstOccurance[pntoffset + 2] });
			triangles.push_back(Triangle{ firstOccurance[pntoffset + 4], firstOccurance[pntoffset + 5], firstOccurance[pntoffset + 6] });
			triangles.push_back(Triangle{ firstOccurance[pntoffset + 4], firstOccurance[pntoffset + 6], firstOccurance[pntoffset + 7] });

		}
	}

}

//in short, if an edge intersects with edges of other piles, add the vertices lying inside the edge to the edge vxs
//each pile has 4 edges. each point falling inside the edge of current pile, is added to the points of this edge.
//piles of 4 direct nb pixels intersect the piles of current pixel in faces. an intersection adds 2 points, each on an edge
//piles of 4 cross nb pixels intersect the piles of current pixel in edges. an intersection adds 1 point on an edge
void InsertPointsOnEdges(vector<PileStruct>& Piles, vector<Pixel>& pixels, vector<int>& firstOccurance, const int w, const int h, vector<PileEdges>& pileEdges)
{
	for (int pixelIndex = 0; pixelIndex < w * h; pixelIndex++)
	{

		if (!pixels[pixelIndex].ContainsPiles())
			continue;

		//piles of current pixel//
		for (int pilePixelIndex = 0; pilePixelIndex < pixels[pixelIndex].PilesCount; pilePixelIndex++)
		{

			int pileIndex = pixels[pixelIndex].PilesOffset + pilePixelIndex;
			int vxoffset = 8 * pileIndex;

			float zmin = Piles[pileIndex].start;
			float zmax = Piles[pileIndex].end;

			//each pile has exactly 4 edges. each edge has a set of points
			//We already know two points on each edge of the 4. These are independent on the neighboring pixels//
			pileEdges[pileIndex].edges[0].insert(firstOccurance[vxoffset]); pileEdges[pileIndex].edges[0].insert(firstOccurance[vxoffset + 4]);
			pileEdges[pileIndex].edges[1].insert(firstOccurance[vxoffset + 1]); pileEdges[pileIndex].edges[1].insert(firstOccurance[vxoffset + 5]);
			pileEdges[pileIndex].edges[2].insert(firstOccurance[vxoffset + 2]); pileEdges[pileIndex].edges[2].insert(firstOccurance[vxoffset + 6]);
			pileEdges[pileIndex].edges[3].insert(firstOccurance[vxoffset + 3]); pileEdges[pileIndex].edges[3].insert(firstOccurance[vxoffset + 7]);
		}
	}

	for (int pixelIndex = 0; pixelIndex < w * h; pixelIndex++)
	{

		if (!pixels[pixelIndex].ContainsPiles())
			continue;


		/////////////////////
		//check 4 direct nbs//
		/////////////////////

		vector<int>nbpixels;
		pixels[pixelIndex].GetDirectNbs(pixels, w, h, nbpixels); //pixels that share a face


		//piles of current pixel//
		for (int pilePixelIndex = 0; pilePixelIndex < pixels[pixelIndex].PilesCount; pilePixelIndex++)
		{

			int pileIndex = pixels[pixelIndex].PilesOffset + pilePixelIndex;

			float zmin = Piles[pileIndex].start;
			float zmax = Piles[pileIndex].end;


			for (int nb = 0; nb < nbpixels.size(); nb++) //go through the 4 direct nb pixels (this is for a single pile of the current pixel). compare against the current pile of the current pixel)
			{
				//if a point on an edge of an nb pile intersects an edge of current pile, add the points to the current edge

				int nbpixel = nbpixels[nb];
				bool intersection = false;
				for (int nbpileIndex = 0; nbpileIndex < pixels[nbpixel].PilesCount && !intersection; nbpileIndex++) //go through all piles of each nb pixel
				{

					int nbpile = pixels[nbpixel].PilesOffset + nbpileIndex;
					int nbvxoffset = 8 * nbpile;

					//get zmin and zmax of pile//
					float nbzmin = Piles[nbpile].start;
					float nbzmax = Piles[nbpile].end;

					int facingFace = Pixel::GetFacingFace(pixels[pixelIndex], pixels[nbpixel]);
					int edge0 = facingFace;
					int edge1 = (facingFace + 1) % 4;

					int nbv00, nbv10, nbv01, nbv11;
					Pixel::GetFacingVertices(pixels[pixelIndex], pixels[nbpixel], nbv00, nbv10);
					nbv01 = nbv00 + 4;
					nbv11 = nbv10 + 4;

					//add upper two vxs
					if (nbzmin > zmin && nbzmin < zmax)
					{
						pileEdges[pileIndex].edges[edge0].insert(firstOccurance[nbvxoffset + nbv00]);
						pileEdges[pileIndex].edges[edge1].insert(firstOccurance[nbvxoffset + nbv10]);
					}

					//add lower two vxs
					if (nbzmax > zmin && nbzmax < zmax)
					{
						pileEdges[pileIndex].edges[edge0].insert(firstOccurance[nbvxoffset + nbv01]);
						pileEdges[pileIndex].edges[edge1].insert(firstOccurance[nbvxoffset + nbv11]);
					}
				}
			}

		}


		//////////////////////////
		//check 4 cross nb pixels//
		//////////////////////////

		nbpixels.clear();
		pixels[pixelIndex].GetCrossNbs(pixels, w, h, nbpixels); //pixels that share an edge

		//vector<int>edgesInds;	//which edge of the current pile intersect with the piles of the nb pixel
		//vector<int>nbvs; nbvs.reserve(4);		//which point of the piles of the nb pixel (should be 2 points, the other is the saved + 4)


		for (int pilePixelIndex = 0; pilePixelIndex < pixels[pixelIndex].PilesCount; pilePixelIndex++)
		{

			int pileIndex = pixels[pixelIndex].PilesOffset + pilePixelIndex;

			float zmin = Piles[pileIndex].start;
			float zmax = Piles[pileIndex].end;

			for (int nb = 0; nb < nbpixels.size(); nb++)
			{
				int nbpixel = nbpixels[nb];
				for (int nbpileIndex = 0; nbpileIndex < pixels[nbpixels[nb]].PilesCount; nbpileIndex++)
				{

					int nbpile = pixels[nbpixel].PilesOffset + nbpileIndex;
					int nbvxoffset = 8 * nbpile;

					int facingedge = Pixel::GetCrossEdge(pixels[pixelIndex], pixels[nbpixels[nb]]);

					int nbv0 = Pixel::GetCrossVertex(pixels[pixelIndex], pixels[nbpixels[nb]]);
					int nbv1 = nbv0 + 4;

					//get zmin and zmax
					float nbzmin = Piles[nbpile].start;
					float nbzmax = Piles[nbpile].end;

					if (nbzmin > zmin && nbzmin < zmax)
						pileEdges[pileIndex].edges[facingedge].insert(firstOccurance[nbvxoffset + nbv0]);
					if (nbzmax > zmin && nbzmax < zmax)
						pileEdges[pileIndex].edges[facingedge].insert(firstOccurance[nbvxoffset + nbv1]);
				}
			}

		}

	}
}


//each pile has initial 4 faces
//the function just splits each face, depending on the neighboring faces
//each face will have 4 vertices. more vertices are added later in the function
void CreateFaces(vector<PointCoordsExt>& points, vector<PileStruct>& Piles, vector<Pixel>& pixels, vector<int>& firstOccurance, const int w, const int h, vector<PileEdges>& pileEdges, vector<Face>& PilesFaces)
{
	for (int pixelIndex = 0; pixelIndex < w * h; pixelIndex++) //go through all pixels
	{
		if (!pixels[pixelIndex].ContainsPiles())
			continue;

		/////////////////////
		//check 4 direct nbs//
		/////////////////////

		//Note: faces that have no nb pixels should go through the face extraction too ! and we assume each face has its corresponding nb at same index. so, even if no nbs, keep a -1
		vector<int>nbpixels;
		pixels[pixelIndex].GetDirectNbs(pixels, w, h, nbpixels, false); //pixels that share a face


		//the two vertices (of 8) of the piles of each neighboring pixels that can intersect with the piles of the current pixel
		//actually, should be 4 vertices. the saved two are incremented by 4 to get the other two.
		vector<int>nbv0s; nbv0s.reserve(4);
		vector<int>nbv1s; nbv1s.reserve(4);

		//what happens if face has no corresponding nb pixel? fixed//

		for (int pilePixelIndex = 0; pilePixelIndex < pixels[pixelIndex].PilesCount; pilePixelIndex++) //go through the piles of current pixel
		{

			int pileIndex = pixels[pixelIndex].PilesOffset + pilePixelIndex;
			int vxoffset = 8 * pileIndex;


			for (int nb = 0; nb < nbpixels.size(); nb++)	//go through the 4 direct nbs
			{
				//the two vertices (of 8) of the piles of the current pixel that can intersect with the piles of each neighboring pixels
				//actually, should be 4 vertices. the saved two are incremented by 4 to get the other two.
				int v0 = nb;
				int v1 = (nb + 1) % nbpixels.size();


				//very first 4 vertices of a pile
				Face fc;
				fc.v0 = firstOccurance[vxoffset + v0];
				fc.v1 = firstOccurance[vxoffset + v1];
				fc.v2 = firstOccurance[vxoffset + v0 + 4];
				fc.v3 = firstOccurance[vxoffset + v1 + 4];
				fc.pile = pileIndex;
				fc.face = nb; //from 0 to 3

				vector<Face>FaceSubFaces;

				//can also have them from piles. all upper vxs have same z, and also lower.
				//are these updated after cutting? check!
				//pile has different faces. z values have to be found here, not before loop.//
				float fzmin = points[fc.v0].z;
				float fzmax = points[fc.v2].z;

				//probably cut face in case the faces intersect
				bool TakeFace = true;
				bool stop = false; //gets true when we pass our pile anyway along z
				if (nbpixels[nb] != -1)	//we have an nb pixel. otherwise, will just add the face as it is
				{
					int nbv0, nbv1;
					Pixel::GetFacingVertices(pixels[pixelIndex], pixels[nbpixels[nb]], nbv0, nbv1);


					int nbpixel = nbpixels[nb];
					for (int nbpileIndex = 0; nbpileIndex < pixels[nbpixel].PilesCount && !stop; nbpileIndex++) //in case no pile, or no intersection, the face is moved as is//
					{

						int nbpile = pixels[nbpixel].PilesOffset + nbpileIndex;
						int nbvxoffset = 8 * nbpile;

						//get zmin and zmax
						float nbzmin = Piles[nbpile].start;
						float nbzmax = Piles[nbpile].end;

						//make a face from the current pile here
						Face nbfc; nbfc.v0 = firstOccurance[nbvxoffset + nbv0]; nbfc.v1 = firstOccurance[nbvxoffset + nbv1]; nbfc.v2 = firstOccurance[nbvxoffset + nbv0 + 4]; nbfc.v3 = firstOccurance[nbvxoffset + nbv1 + 4];

						if (fzmin < nbzmin && nbzmin < fzmax && fzmin < nbzmax && nbzmax < fzmax) //splits the face, and lose a part in the middle
						{
							//add a face to addt//
							Face newface;
							newface.v0 = fc.v0;
							newface.v1 = fc.v1;
							newface.v2 = nbfc.v0;
							newface.v3 = nbfc.v1;
							newface.pile = fc.pile;
							newface.face = fc.face;
							FaceSubFaces.push_back(newface);

							//update face//
							fc.v0 = nbfc.v2; fc.v1 = nbfc.v3; //take lower vxs of neighbor//
							fzmin = nbzmax;


						}
						else if (fzmin < nbzmin && nbzmin < fzmax) //nbzmax could be equal to fzmax//
						{
							//update face//
							fc.v2 = nbfc.v0; fc.v3 = nbfc.v1;
							//no need to update z value. we stop anyway//

							stop = true;

						}
						else if (fzmin < nbzmax && nbzmax < fzmax)
						{
							//update face//
							fc.v0 = nbfc.v2; fc.v1 = nbfc.v3;
							fzmin = nbzmax;
						}
						else if (nbzmin <= fzmin && fzmax <= nbzmax) //nbzmin could be equal to fzmin// //pile is totally covered//
						{
							TakeFace = false;
							stop = true;
						}
						else if (nbzmin >= fzmax) //passed the pile
						{
							stop = true;
						}

					}

				}

				if (TakeFace)
					FaceSubFaces.push_back(fc);

				//distribute vxs over edges of faces//
				//PilesEdges are used to add vxs to the 2 edges of the found faces !
				for (auto f : FaceSubFaces)
				{
					f.FillVerticesFromEdgesToEdges(pileEdges[pileIndex].edges[v0], pileEdges[pileIndex].edges[v1], points);
					PilesFaces.push_back(f);
				}

			}

		}
	}
}

void TriangulateFaces(vector<Face>& pilesFaces, vector<PointCoordsExt>& points, vector<Triangle>& triangles)
{
	for (Face fc : pilesFaces)
	{

		//remove duplicates//
		fc.e0.RemoveDuplicates();
		fc.e1.RemoveDuplicates();


		//sort//
		fc.e0.SortByZ(points);
		fc.e1.SortByZ(points);

		//after sorting//
		//triangulate by moving two pointers along the two edges, and advance
		Triangle::Triangulate(fc, triangles);
	}
}


bool TrigHasTwoVxsNotThird(Triangle trig, int v0, int v1, int v2, int& third)
{
	if (trig.v0 == v0 && trig.v1 == v1 && trig.v2 != v2)
	{
		third = trig.v2;
		return  true;
	}
	if (trig.v0 == v0 && trig.v2 == v1 && trig.v1 != v2)
	{
		third = trig.v1;
		return  true;
	}
	if (trig.v1 == v0 && trig.v0 == v1 && trig.v2 != v2)
	{
		third = trig.v2;
		return  true;
	}
	if (trig.v1 == v0 && trig.v2 == v1 && trig.v0 != v2)
	{
		third = trig.v0;
		return  true;
	}
	if (trig.v2 == v0 && trig.v1 == v1 && trig.v0 != v2)
	{
		third = trig.v0;
		return  true;
	}
	if (trig.v2 == v0 && trig.v0 == v1 && trig.v1 != v2)
	{
		third = trig.v1;
		return  true;
	}

	return false;
}

bool TrigHasTwoVxs(Triangle trig, int v0, int v1, int& third)
{
	if (trig.v0 == v0 && trig.v1 == v1)
	{
		third = trig.v2;
		return  true;
	}
	if (trig.v0 == v0 && trig.v2 == v1)
	{
		third = trig.v1;
		return  true;
	}
	if (trig.v1 == v0 && trig.v0 == v1)
	{
		third = trig.v2;
		return  true;
	}
	if (trig.v1 == v0 && trig.v2 == v1)
	{
		third = trig.v0;
		return  true;
	}
	if (trig.v2 == v0 && trig.v1 == v1)
	{
		third = trig.v0;
		return  true;
	}
	if (trig.v2 == v0 && trig.v0 == v1)
	{
		third = trig.v1;
		return  true;
	}

	return false;
}

void FindNonManifoldEdges(vector<PointCoordsExt>& points, vector<Triangle>& triangles)
{
	vector<bool>VxIsMf(points.size(), true);

	vector<vector<int>>vxToTrigs(points.size());
	for (int t = 0; t < triangles.size(); t++)
	{
		vxToTrigs[triangles[t].v0].push_back(t);
		vxToTrigs[triangles[t].v1].push_back(t);
		vxToTrigs[triangles[t].v2].push_back(t);
	}


	vector<TrigEdge>edges; edges.reserve(3 * triangles.size());
	for (int t = 0; t < triangles.size(); t++)
	{
		TrigEdge te;

		te.v2 = triangles[t].v2;
		if (triangles[t].v0 < triangles[t].v1)
		{
			te.v0 = triangles[t].v0;
			te.v1 = triangles[t].v1;
		}
		else
		{
			te.v0 = triangles[t].v1;
			te.v1 = triangles[t].v0;
		}
		edges.push_back(te);

		te.v2 = triangles[t].v0;
		if (triangles[t].v1 < triangles[t].v2)
		{
			te.v0 = triangles[t].v1;
			te.v1 = triangles[t].v2;
		}
		else
		{
			te.v0 = triangles[t].v2;
			te.v1 = triangles[t].v1;
		}
		edges.push_back(te);

		te.v2 = triangles[t].v1;
		if (triangles[t].v0 < triangles[t].v2)
		{
			te.v0 = triangles[t].v0;
			te.v1 = triangles[t].v2;
		}
		else
		{
			te.v0 = triangles[t].v2;
			te.v1 = triangles[t].v0;
		}
		edges.push_back(te);
	}

	std::sort(edges.begin(), edges.end());


	int NonManifoldEdgesNum = 0;
	int EdgeRep = 0;
	int oldv0 = -1, oldv1 = -1;
	int EdgeOnMoreThanFour = 0;
	int EdgeOnOdd = 0;
	for (int i = 0; i < edges.size(); i++)
	{
		//if (i % 100 == 0)
		//	cout << "edge " << i << " out of " << edges.size() << "\n";

		if (edges[i].v0 != oldv0 || edges[i].v1 != oldv1)
		{
			oldv0 = edges[i].v0;
			oldv1 = edges[i].v1;

			EdgeRep = 1;
		}
		else
		{
			EdgeRep++;
			if (EdgeRep > 2)
			{
				VxIsMf[edges[i].v0] = false;
				VxIsMf[edges[i].v1] = false;
			}
		}
	}

	int NonMfVxs = 0;
	for (int p = 0; p < points.size(); p++)
	{
		if (!VxIsMf[p])
			NonMfVxs++;
	}

	cout << "the non manifold edges affect " << NonMfVxs << " vxs\n";

	int u = 3;

}

void FindEdgesOnOneTriangle(vector<PointCoordsExt>& points, vector<Triangle>& triangles)
{
	vector<bool>VxIsMf(points.size(), true);

	vector<vector<int>>vxToTrigs(points.size());
	for (int t = 0; t < triangles.size(); t++)
	{
		vxToTrigs[triangles[t].v0].push_back(t);
		vxToTrigs[triangles[t].v1].push_back(t);
		vxToTrigs[triangles[t].v2].push_back(t);
	}


	vector<TrigEdge>edges; edges.reserve(3 * triangles.size());
	for (int t = 0; t < triangles.size(); t++)
	{
		TrigEdge te;

		te.v2 = triangles[t].v2;
		if (triangles[t].v0 < triangles[t].v1)
		{
			te.v0 = triangles[t].v0;
			te.v1 = triangles[t].v1;
		}
		else
		{
			te.v0 = triangles[t].v1;
			te.v1 = triangles[t].v0;
		}
		edges.push_back(te);

		te.v2 = triangles[t].v0;
		if (triangles[t].v1 < triangles[t].v2)
		{
			te.v0 = triangles[t].v1;
			te.v1 = triangles[t].v2;
		}
		else
		{
			te.v0 = triangles[t].v2;
			te.v1 = triangles[t].v1;
		}
		edges.push_back(te);

		te.v2 = triangles[t].v1;
		if (triangles[t].v0 < triangles[t].v2)
		{
			te.v0 = triangles[t].v0;
			te.v1 = triangles[t].v2;
		}
		else
		{
			te.v0 = triangles[t].v2;
			te.v1 = triangles[t].v0;
		}
		edges.push_back(te);
	}

	std::sort(edges.begin(), edges.end());

	int edgesOnOneTrig = 0;

	int NonManifoldEdgesNum = 0;
	int EdgeRep = 0;
	int oldv0 = -1, oldv1 = -1;
	int EdgeOnMoreThanFour = 0;
	int EdgeOnOdd = 0;
	for (int i = 0; i < edges.size(); i++)
	{
		//if (i % 100 == 0)
		//	cout << "edge " << i << " out of " << edges.size() << "\n";

		if (edges[i].v0 != oldv0 || edges[i].v1 != oldv1)
		{
			if (EdgeRep == 1)
				edgesOnOneTrig++;
			
			oldv0 = edges[i].v0;
			oldv1 = edges[i].v1;

			EdgeRep = 1;
		}
		else
		{
			EdgeRep++;
		}
	}

	cout << "the edges on one triangle only " << edgesOnOneTrig << " vxs\n";

	int u = 3;

}


void FixNonManifoldEdges(vector<PointCoordsExt>& points, vector<Triangle>& triangles)
{
	vector<vector<int>>vxToTrigs(points.size());
	for (int t = 0; t < triangles.size(); t++)
	{
		vxToTrigs[triangles[t].v0].push_back(t);
		vxToTrigs[triangles[t].v1].push_back(t);
		vxToTrigs[triangles[t].v2].push_back(t);
	}

	//for testing purposes
	/*for (int t = 0; t < triangles.size(); t++)
	{
		if (triangles[t].v0 == 22 || triangles[t].v1 == 22 || triangles[t].v2 == 22 || triangles[t].v0 == 101 || triangles[t].v1 == 101 || triangles[t].v2 == 101)
			t = t;
	}*/

	vector<TrigEdge>edges; edges.reserve(3 * triangles.size());
	for (int t = 0; t < triangles.size(); t++)
	{
		TrigEdge te;

		te.v2 = triangles[t].v2;
		if (triangles[t].v0 < triangles[t].v1)
		{
			te.v0 = triangles[t].v0;
			te.v1 = triangles[t].v1;
		}
		else
		{
			te.v0 = triangles[t].v1;
			te.v1 = triangles[t].v0;
		}
		edges.push_back(te);

		te.v2 = triangles[t].v0;
		if (triangles[t].v1 < triangles[t].v2)
		{
			te.v0 = triangles[t].v1;
			te.v1 = triangles[t].v2;
		}
		else
		{
			te.v0 = triangles[t].v2;
			te.v1 = triangles[t].v1;
		}
		edges.push_back(te);

		te.v2 = triangles[t].v1;
		if (triangles[t].v0 < triangles[t].v2)
		{
			te.v0 = triangles[t].v0;
			te.v1 = triangles[t].v2;
		}
		else
		{
			te.v0 = triangles[t].v2;
			te.v1 = triangles[t].v0;
		}
		edges.push_back(te);
	}

	std::sort(edges.begin(), edges.end());


	int NonManifoldEdgesNum = 0;
	int EdgeRep = 0;
	int oldv0 = -1, oldv1 = -1;
	int EdgeOnMoreThanFour = 0;
	int EdgeOnOdd = 0;
	for (int i = 0; i < edges.size(); i++)
	{
		if (i % 100 == 0)
			cout << "edge " << i << " out of " << edges.size() << "\n";

		if (edges[i].v0 != oldv0 || edges[i].v1 != oldv1)
		{
			if (EdgeRep > 2)
			{
				int v0 = oldv0;
				int v1 = oldv1;

				vector<int>TrigsToChange;	//will contain an umbrella of trigs around v0

				int fixed = v0; int othervx = v1;

				int tindex;	int movable;
				bool stop = false;
				for (int vt = 0; vt < vxToTrigs[fixed].size() && !stop; vt++)
				{
					tindex = vxToTrigs[fixed][vt];
					stop = TrigHasTwoVxs(triangles[tindex], fixed, othervx, movable); //find a trig with fixed and othervx, and return the third in movable
				}
				TrigsToChange.push_back(tindex);
				int firstmovable = movable;

				int avoided = v1;
				bool OtherTrigFound = false;
				while (!OtherTrigFound)
				{
					int third;
					bool found = false;
					int tindex;
					for (int vt = 0; vt < vxToTrigs[fixed].size() && !found; vt++)
					{
						//get using fixed, movable, avoided
						tindex = vxToTrigs[fixed][vt];
						found = TrigHasTwoVxsNotThird(triangles[tindex], fixed, movable, avoided, third); //find a trig with fixed and movable, but not avoied, and return the third vx of such trig
					}
					TrigsToChange.push_back(tindex);

					avoided = movable;
					movable = third;

					if (third == othervx)			//add it, but stop
						OtherTrigFound = true;
				}


				int newv0 = points.size();
				points.push_back(points[v0]);

				for (int j = 0; j < TrigsToChange.size(); j++)
				{
					if (triangles[TrigsToChange[j]].v0 == v0)
						triangles[TrigsToChange[j]].v0 = newv0;
					if (triangles[TrigsToChange[j]].v1 == v0)
						triangles[TrigsToChange[j]].v1 = newv0;
					if (triangles[TrigsToChange[j]].v2 == v0)
						triangles[TrigsToChange[j]].v2 = newv0;
				}

				///////////////////////
				//////////////////////
				/////////////////////
				//fixing the other vertex
				////////////////////
				///////////////////

				
				{
					TrigsToChange.clear();

					fixed = v1; othervx = newv0;

					stop = false;
					for (int vt = 0; vt < vxToTrigs[fixed].size() && !stop; vt++)
					{
						tindex = vxToTrigs[fixed][vt];
						stop = TrigHasTwoVxs(triangles[tindex], fixed, othervx, movable); //find a trig with fixed and othervx, and return the third in movable
					}
					TrigsToChange.push_back(tindex);
					firstmovable = movable;

					avoided = newv0;
					OtherTrigFound = false;
					while (!OtherTrigFound)
					{
						int third;
						bool found = false;
						int tindex;
						for (int vt = 0; vt < vxToTrigs[fixed].size() && !found; vt++)
						{
							//get using fixed, movable, avoided
							tindex = vxToTrigs[fixed][vt];
							found = TrigHasTwoVxsNotThird(triangles[tindex], fixed, movable, avoided, third); //find a trig with fixed and movable, but not avoied, and return the third vx of such trig
						}
						TrigsToChange.push_back(tindex);

						avoided = movable;
						movable = third;

						if (third == othervx)			//add it, but stop
							OtherTrigFound = true;
					}


					int newv1 = points.size();
					points.push_back(points[v1]);

					for (int j = 0; j < TrigsToChange.size(); j++)
					{
						if (triangles[TrigsToChange[j]].v0 == v1)
							triangles[TrigsToChange[j]].v0 = newv1;
						if (triangles[TrigsToChange[j]].v1 == v1)
							triangles[TrigsToChange[j]].v1 = newv1;
						if (triangles[TrigsToChange[j]].v2 == v1)
							triangles[TrigsToChange[j]].v2 = newv1;
					}
				}
				
			}
			oldv0 = edges[i].v0;
			oldv1 = edges[i].v1;

			EdgeRep = 1;
		}
		else
			EdgeRep++;
	}



}

//should be better than FixNonManifoldEdges()
void FixNonManifoldsNew(vector<PointCoordsExt>& points, vector<Triangle>& triangles)
{
	vector<vector<int>>vxToTrigs(points.size());
	for (int t = 0; t < triangles.size(); t++)
	{
		vxToTrigs[triangles[t].v0].push_back(t);
		vxToTrigs[triangles[t].v1].push_back(t);
		vxToTrigs[triangles[t].v2].push_back(t);
	}

	vector<TrigEdge>edges; edges.reserve(3 * triangles.size());
	for (int t = 0; t < triangles.size(); t++)
	{
		TrigEdge te;

		te.v2 = triangles[t].v2;
		if (triangles[t].v0 < triangles[t].v1)
		{
			te.v0 = triangles[t].v0;
			te.v1 = triangles[t].v1;
		}
		else
		{
			te.v0 = triangles[t].v1;
			te.v1 = triangles[t].v0;
		}
		edges.push_back(te);

		te.v2 = triangles[t].v0;
		if (triangles[t].v1 < triangles[t].v2)
		{
			te.v0 = triangles[t].v1;
			te.v1 = triangles[t].v2;
		}
		else
		{
			te.v0 = triangles[t].v2;
			te.v1 = triangles[t].v1;
		}
		edges.push_back(te);

		te.v2 = triangles[t].v1;
		if (triangles[t].v0 < triangles[t].v2)
		{
			te.v0 = triangles[t].v0;
			te.v1 = triangles[t].v2;
		}
		else
		{
			te.v0 = triangles[t].v2;
			te.v1 = triangles[t].v0;
		}
		edges.push_back(te);
	}

	std::sort(edges.begin(), edges.end());


	int NonManifoldEdgesNum = 0;
	int EdgeRep = 0;
	int oldv0 = -1, oldv1 = -1;
	int EdgeOnMoreThanFour = 0;
	int EdgeOnOdd = 0;
	for (int i = 0; i < edges.size(); i++)
	{

		if (edges[i].v0 != oldv0 || edges[i].v1 != oldv1)
		{
			if (EdgeRep > 2)
			{
				int v0 = oldv0;
				int v1 = oldv1;

				vector<int>probemsVxs;
				probemsVxs.push_back(v0);	probemsVxs.push_back(v1);

				vector<set<int>>TrigsToChange(2);
				vector<int>newv(2);

				//////////////////////////////////
				// finding to be converted trigs//
				//////////////////////////////////

				for (int v = 0; v < 2; v++)
				{
					int vertex = probemsVxs[v];


					set<int>OtherTrigs;

					set<int>neutralVxs;

					set<int> TakeVxs;
					set<int> DontTakeVxs;

					set<int>group0; set<int>group1; set<int>group2; set<int>group3;

					//create new vertex
					newv[v] = points.size();
					points.push_back(points[vertex]);

					int pile0 = -1, pile1 = -1;
					for (int vt = 0; vt < vxToTrigs[vertex].size(); vt++)
					{
						int tindex = vxToTrigs[vertex][vt];
						if (triangles[tindex].pile == -1)
							continue;

						if (pile0 == -1)
							pile0 = triangles[tindex].pile;
						else if (pile1 == -1 && pile0 != triangles[tindex].pile)
							pile1 = triangles[tindex].pile;
						else if (pile0 != -1 && pile1 != -1 && pile0 != triangles[tindex].pile && pile1 != triangles[tindex].pile)
							pile0 = pile0;
					}

					for (int vt = 0; vt < vxToTrigs[vertex].size(); vt++)
					{
						int tindex = vxToTrigs[vertex][vt];
						if (triangles[tindex].pile != -1)
						{

							if (triangles[tindex].pile == pile0 && (triangles[tindex].face == 0 || triangles[tindex].face == 2))
								group0.insert(tindex);
							else if (triangles[tindex].pile == pile0 && (triangles[tindex].face == 1 || triangles[tindex].face == 3))
								group1.insert(tindex);
							else if (triangles[tindex].pile == pile1 && (triangles[tindex].face == 0 || triangles[tindex].face == 2))
								group2.insert(tindex);
							else if (triangles[tindex].pile == pile1 && (triangles[tindex].face == 1 || triangles[tindex].face == 3))
								group3.insert(tindex);

						}
					}


					for (int t : group0)
					{
						TrigsToChange[v].insert(t);

						set<int>vxs;
						vxs.insert(triangles[t].v0); vxs.insert(triangles[t].v1); vxs.insert(triangles[t].v2);

						for (int vx : vxs)
						{
							if (vx == v0 || vx == v1)
								continue;
							TakeVxs.insert(vx);
						}
					}
					for (int t : group3)
					{
						TrigsToChange[v].insert(t);

						set<int>vxs;
						vxs.insert(triangles[t].v0); vxs.insert(triangles[t].v1); vxs.insert(triangles[t].v2);

						for (int vx : vxs)
						{
							if (vx == v0 || vx == v1)
								continue;
							TakeVxs.insert(vx);
						}
					}

					for (int t : group1)
					{
						OtherTrigs.insert(t);

						set<int>vxs;
						vxs.insert(triangles[t].v0); vxs.insert(triangles[t].v1); vxs.insert(triangles[t].v2);

						for (int vx : vxs)
						{
							if (vx == v0 || vx == v1)
								continue;
							DontTakeVxs.insert(vx);
						}
					}
					for (int t : group2)
					{
						OtherTrigs.insert(t);

						set<int>vxs;
						vxs.insert(triangles[t].v0); vxs.insert(triangles[t].v1); vxs.insert(triangles[t].v2);

						for (int vx : vxs)
						{
							if (vx == v0 || vx == v1)
								continue;
							DontTakeVxs.insert(vx);
						}
					}

					for (int vt = 0; vt < vxToTrigs[vertex].size(); vt++)
					{
						bool convert = false;
						int tindex = vxToTrigs[vertex][vt];
						if (triangles[tindex].pile == -1)
						{
							//get 3 vertices
							set<int>vxs;
							vxs.insert(triangles[tindex].v0); vxs.insert(triangles[tindex].v1); vxs.insert(triangles[tindex].v2);

							bool convert = false;
							for (int vx : vxs)
							{
								if (vx == v0 || vx == v1)
									continue;
								bool InTaken = TakeVxs.find(vx) != TakeVxs.end();
								bool InNotTaken = DontTakeVxs.find(vx) != DontTakeVxs.end();
								if (InTaken && !InNotTaken)
									convert = true;
							}
							if (convert)
								TrigsToChange[v].insert(tindex);
							else
								OtherTrigs.insert(tindex);
						}
					}

					for (int tindex : TrigsToChange[v])
					{
						if (triangles[tindex].pile != -1)
							continue;
						
						//get 3 vertices
						set<int>vxs;
						vxs.insert(triangles[tindex].v0); vxs.insert(triangles[tindex].v1); vxs.insert(triangles[tindex].v2);

						for (int vx : vxs)
						{
							if (vx == v0 || vx == v1)
								continue;
							TakeVxs.insert(vx);
						}
					}
					
					for (int tindex : OtherTrigs)
					{
						if (triangles[tindex].pile != -1)
							continue;

						//get 3 vertices
						set<int>vxs;
						vxs.insert(triangles[tindex].v0); vxs.insert(triangles[tindex].v1); vxs.insert(triangles[tindex].v2);

						for (int vx : vxs)
						{
							if (vx == v0 || vx == v1)
								continue;
							DontTakeVxs.insert(vx);
						}
					}

					for (int vx : TakeVxs)
					{
						if (vx == v0 || vx == v1)	//probably not necessary
							continue;

						if (DontTakeVxs.find(vx) != DontTakeVxs.end())
							neutralVxs.insert(vx);
					}

					//create new trigs
					for (int vx : neutralVxs)
					{
						Triangle t;
						t.v0 = vertex;
						t.v1 = newv[v];
						t.v2 = vx;
						triangles.push_back(t);
					}
				}
			
				
				/////////////////////
				// now updating vxs//
				/////////////////////

				for (int tindex : TrigsToChange[0])
				{
					if (triangles[tindex].v0 == v0)
						triangles[tindex].v0 = newv[0];
					if (triangles[tindex].v1 == v0)
						triangles[tindex].v1 = newv[0];
					if (triangles[tindex].v2 == v0)
						triangles[tindex].v2 = newv[0];
				}
				for (int tindex : TrigsToChange[1])
				{
					if (triangles[tindex].v0 == v1)
						triangles[tindex].v0 = newv[1];
					if (triangles[tindex].v1 == v1)
						triangles[tindex].v1 = newv[1];
					if (triangles[tindex].v2 == v1)
						triangles[tindex].v2 = newv[1];
				}

			}
			oldv0 = edges[i].v0;
			oldv1 = edges[i].v1;

			EdgeRep = 1;
		}
		else
			EdgeRep++;
	}

}
void CheckConnectedComponents(int pnum, vector<Triangle>& triangles)
{
	vector<bool> taken(pnum, false);

	vector<set<int>>nbs(pnum); //of vertices

	for (int t = 0; t < triangles.size(); t++)
	{
		nbs[triangles[t].v0].insert(triangles[t].v1);
		nbs[triangles[t].v0].insert(triangles[t].v2);
		nbs[triangles[t].v1].insert(triangles[t].v0);
		nbs[triangles[t].v1].insert(triangles[t].v2);
		nbs[triangles[t].v2].insert(triangles[t].v0);
		nbs[triangles[t].v2].insert(triangles[t].v1);
	}

	int cc = 0;
	for (int p = 0; p < pnum; p++)
	{
		if (!taken[p] && nbs[p].size() != 0)
		{
			taken[p] = true;
			stack<int>st;
			st.push(p);
			while (!st.empty())
			{
				int pnt = st.top();
				st.pop();

				for (int nb : nbs[pnt])
				{
					if (!taken[nb])
					{
						taken[nb] = true;
						st.push(nb);
					}
				}
			}
			cout << "one connected component finished ***\n";
			cc++;
		}
	}
	cout << "connected components number: " << cc << '\n';

	/*
	int takenNum = 0;
	bool AllTaken = false;
	while (takenNum < pnum)
	{
		for (int t = 0; t < triangles.size(); t++)
		{
			if(triangles[t].v0)
		}
	}
	*/

}

//each pile has 8 points. will be added to a vector
void fillPoints(vector<PileStruct>& Piles, vector<Pixel>& pixels, const int w, const int h, const int left, const int right, float bottom, float top, vector<PointCoordsExt>& points)
{
	for (int pixelIndex = 0; pixelIndex < w * h; pixelIndex++)
	{
		if (pixels[pixelIndex].PilesCount == 0)
			continue;

		float x = pixelIndex % w;
		float y = pixelIndex / w;
		float inc = 1;

		for (int pilePixelIndex = 0; pilePixelIndex < pixels[pixelIndex].PilesCount; pilePixelIndex++)	//index within the pixel piles
		{

			int pileIndex = pixels[pixelIndex].PilesOffset + pilePixelIndex;	//index within whole set of piles
			float zmin = Piles[pileIndex].start;
			float zmax = Piles[pileIndex].end;

			vector<PointCoordsExt>corners{ PointCoordsExt{x + 0, y + 0, zmin} ,
				PointCoordsExt{ x + inc, y + 0, zmin },
				PointCoordsExt{ x + inc, y + inc, zmin },
				PointCoordsExt{ x + 0, y + inc, zmin },
				PointCoordsExt{ x + 0, y + 0, zmax } ,
				PointCoordsExt{ x + inc, y + 0, zmax },
				PointCoordsExt{ x + inc, y + inc, zmax },
				PointCoordsExt{ x + 0, y + inc, zmax }
			};

			for (int i = 0; i < corners.size(); i++)
			{
				PointCoordsExt pc = corners[i];
				pc.x = ((((pc.x / w) * 2 - 1) * (right - left)) + (right + left)) / 2;
				pc.y = ((((pc.y / h) * 2 - 1) * (top - bottom)) + (top + bottom)) / 2;
				points.push_back(pc);


			}
		}
	}
}




