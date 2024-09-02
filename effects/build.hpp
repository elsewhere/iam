#ifndef _BUILD_HPP_
#define _BUILD_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "picturepoly.hpp"

class Build : public Base
{
	public:

		Build();
		~Build();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		int xres, yres;
		PicturePoly *polygons;

		void renderScene(float pos, float alpha);
};

#endif