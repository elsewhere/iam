#ifndef _LEHTIPUUEFEKTI_HPP_
#define _LEHTIPUUEFEKTI_HPP_

#include <stdio.h>

#include "../externs.hpp"

class Lehti
{
public:
	Lehti() {};
	~Lehti() {};
	Vector pos;
	Vector rotation;
	Vector color;
	Vector normal;

	Vector v1, v2, v3;

};

class Lehtipuu
{
public:

	Lehtipuu() { nodecount = 0;}
	Lehtipuu(Lista *vertices, Lista *colors, Lista *leaves);
	void draw(Vector &pos, Vector &rot, float rotamount, float alpha);
	void destroy();
	
	int nodecount;
	int leafcount;

	float *branchArray;
	float *leafArray;
	float *branchColorArray; //oksien värit + alphakomponentti valmiina openGL:lle
	float *leafColorArray; //lehtien värit + alphakomponentti valmiina openGL:lle
};
class Lehtipuuefekti : public Base
{
	public:

		Lehtipuuefekti();
		~Lehtipuuefekti();

		bool init(unsigned long s, unsigned long e);
		void draw();
		void release();

	private:

		void generate(Lista *vertices, Lista *col, Lista *leaf, int seed);
		void recurse(Vector *p1, Vector *p2, Lista *vertices, Lista *colors, Lista *leaf, int depth);
		void renderScene(float pos, float alpha);

		int puita;
		Lehtipuu **puut;

		//turhia

		Lista **vertexlist;
		Lista **colorlist;
		Lista **leaflist;

};

#endif