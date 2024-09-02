#ifndef _POTENTIAL_HPP_
#define _POTENTIAL_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Potential : public Base
{
	public:

		Potential();
		~Potential();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		int res;
		Vector *kappale;

		float function(float x, float y, float z);
		void renderScene(float pos, float alpha);

		void renderObject(Vector &pos, Matrix &rotation, float time, float r, float g, float b, float a);

};
#endif