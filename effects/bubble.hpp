#ifndef _BUBBLE_HPP_
#define _BUBBLE_HPP_

#include <stdio.h>

#include "../externs.hpp"

class Bubble : public Base
{
	public:

		Bubble();
		~Bubble();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		void drawCircle(Vector &pos, int slices, float kulma, float radius, float r, float g, float b, float a);
//		Object kappale;
		T3D *kappale;

		float *disttable;
//		TextureGrid *tausta;

		void renderScene(float pos, float alpha);
};

#endif