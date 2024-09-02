#ifndef _DIGITAL_HPP_
#define _DIGITAL_HPP_

#include <stdio.h>
#include "../externs.hpp"

class DigitalNode
{
public:
	Vector position;

	float height;
	float brightness;
	int neighbour;
};

class LineGridNode
{
public:
	Vector position;
	float brightness;

};

class Digital : public Base
{
	public:

		Digital();
		~Digital();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		void building(Vector &position, float xsize, float ysize, float zsize);
		void cube(Vector &position, float size); 

		int nodecount;
		int regularcount;
		int gridcount;
		int zdim, xdim;
		DigitalNode *nodes;
		DigitalNode *regular;
		LineGridNode *grid;

		float *gridbrightness;

		int citycount;
		int citylinecount;
		Vector *cities;
		LineGridNode *citylines;

		void renderScene(float pos, float alpha);
};

#endif
