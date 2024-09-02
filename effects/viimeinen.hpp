#ifndef _VIIMEINEN_HPP_
#define _VIIMEINEN_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Viimeinen : public Base
{
	public:

		Viimeinen();
		~Viimeinen();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		int oksacount;
		int pointcount;
		Vector *points;
		Path **oksat; 

		void renderScene(float pos, float alpha);
		void drawFlare(Vector &pos, float size);

};

#endif