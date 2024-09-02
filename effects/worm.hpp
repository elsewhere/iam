#ifndef _WORM_HPP_
#define _WORM_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Worm : public Base
{
	public:

		Worm();
		~Worm();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		
		void drawFlare(Vector &pos, float size);
		int joints;

		void renderScene(float pos, float alpha);
};

#endif