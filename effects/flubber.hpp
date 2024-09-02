#ifndef _FLUBBER_HPP_
#define _FLUBBER_HPP_

#include <stdio.h>
#include "../externs.hpp"

class FlubberSpike
{
public:

	void calculatePos(float xmul, float ymul, float pos);
	float speed;
	float pspeed;
	float xangle;
	float yangle;
	float pangle;
	float power;
	float x, y;

};

class FlubberVertex
{
public:
	float u, v;	
	float light;
 	float alpha;
 	Vector position;
 	Vector old;
 	Vector normal;
 	Vector rnormal;
};

class FlubberFace
{
public:
	FlubberVertex *a, *b, *c;
	Vector normal;
};


class Flubber : public Base
{
	public:

		Flubber();
		~Flubber();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		void morph(float pos);
		void updateSpikes(float power, float pos);
		void renderScene(float pos, float alpha);
		void calculateNormals();

		void drawSpike(float xpos, float ypos, float power);

		void drawTausta(float pos, float alpha); 

		int yres;
		int slices;

		int spikecount;
		int spikexres;
		int spikeyres;
		float *spikedata;
		FlubberSpike *spikes;

		float *spikebuffer;
		
		FlubberVertex *vertices;
		FlubberFace *faces;

		int taustacount;
		float *taustaradiuses;
		float *taustalenghts;
		float *taustaspeed;
		float *taustapos;
};

#endif