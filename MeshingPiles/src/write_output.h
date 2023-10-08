#include "meshing.h"

void visualizeOriginalPiles(std::string file_stem, int GlobalW, int GlobalH, double cellW, double cellH, vector<vector<PileStruct>>cellsPiles, int PilesNum, int cellsNum, double minx, double maxx, double miny, double maxy);

void writeOutput(std::string file_stem, vector<PointCoordsExt> points, vector<Triangle> triangles);