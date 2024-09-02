#ifndef _METAPILAR_HPP_
#define _METAPILAR_HPP_

#include "metatypes.hpp"
#include "../externs.hpp"

#define MAXBALLS 5

class MetaPilar : public Base
{
	public:

		MetaPilar();
		~MetaPilar();

		bool init(unsigned long s, unsigned long e);		
		void draw();		
		void release();
		
		// Efekti funkkareita
		void drawPilar();
		void drawGrid();
		void drawCube(int x, int y, int z);
		void drawPolyList();
		void drawPolyListFlat();
		void drawPolyListWireFrame();
		void recurse(int x, int y, int z);
		int processCube(int x, int y, int z);		
		
		float getGridPointEnergy(MetaGridPoint *point);		
		
		float getBallsEnergyForGridPoint(MetaGridPoint *point);
		void getBallsNormalForGridPoint(MetaGridPoint *point);
		
		float getFloorEnergyForGridPoint(MetaGridPoint *point);
		float getFloorNormalForGridPoint(MetaGridPoint *point);

	private:

		float angle;
		unsigned int nVertices, nCubes, cubeMap;
		
		Texture *texture;
		MetaGridPoint *polyList;
		MetaGridPoint *vertices;
		MetaGridCube *cubes;
		MetaBall pallot[MAXBALLS];

		char *verticeStatus;
		char *cubeStatus;
		unsigned int polyCount;

};

#endif