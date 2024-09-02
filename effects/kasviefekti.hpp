#ifndef _KASVIEFEKTI_HPP_
#define _KASVIEFEKTI_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Kasvi
{
public:

	Kasvi() { nodecount = 0;}
	Kasvi(Lista *vertices, Lista *colors);
	void draw(Vector &pos, Vector &rot, float rotamount, float alpha);
	void destroy();
	
	int nodecount;

	float *colorArray;
	float *vertexArray;

	Vector *lineList;
	Vector *colorList;
};

class Kasviefekti : public Base
{
	public:

		Kasviefekti();
		~Kasviefekti();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		void generate(Lista *vertices, Lista *col, int seed);
		void recurse(Vector *p1, Vector *p2, Lista *vertices, Lista *colors, int depth);
		void renderScene(float pos, float alpha);
		void nelio(float size, Vector *position, Vector *color, float alpha);

		int squarecount;
	
		Vector *positions;

		int kasveja;
		Kasvi **kasvit;
};

#endif