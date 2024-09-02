#ifndef _MUSTIKKA_HPP_
#define _MUSTIKKA_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "../mathematics.hpp"

class Varpu
{
public:
	Varpu() {};
	~Varpu() {};

	int marjacount;
	int varsicount;
	Vector *marjat;
	Vector *varsi;

	float r, g, b;

	void render();
	void destroy();


};


class Mustikka : public Base
{
	public:

		Mustikka();
		~Mustikka();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		T3D *kappale;

		void calculateFades(float pos); 
		int mustikkacount;
		Varpu *mustikat;
		float *fadetable;
		float *fadetimes_start;
		float *fadetimes_stop; 
		void renderScene(float pos, float alpha);
};

#endif