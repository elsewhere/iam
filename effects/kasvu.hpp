#ifndef _KASVU_HPP_
#define _KASVU_HPP_

#include <stdio.h>
#include "../externs.hpp"

class KasvuNode
{
public:
	Vector pos;
	Vector currentvector;
	Vector flow;
	Vector flowadd;
};

class KasvuParticle
{
public:
	Vector pos;
	Vector speed;
	float energy;

	void init(Vector &position);
	void kill();
//	Lista *vertices;

	Vector *vertices;
	int vertexcount;


};

class Kasvu : public Base
{
	public:

		Kasvu();
		~Kasvu();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		void renderScene(float pos, float alpha);
		void updateFlow(float pos, float flowpower);
		void updateParticles(float dt);
		
		KasvuNode *grid;
		KasvuParticle *particles;

		Vector generatorpoint;

		int particlecount;
		//gridin reso
		int xdim, ydim, zdim;

		//rajat joiden ulkopuolelle ei saa mennä 
		float xmax, ymax, zmax;
		float xmin, ymin, zmin; 

		float frametime;
		float lastframetime;

		int maxvertexcount;
};

#endif