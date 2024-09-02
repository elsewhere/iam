#ifndef _EFEKTI3_HPP_
#define _EFEKTI3_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Efekti1-luokka
//-------------------------------------------------------

class Efekti3 : public Base
{
	friend class Base;

	public:

		Efekti3();
		~Efekti3();
		
		bool init(unsigned long s, unsigned long e);
		void draw();

		void drawHole(float x, float y);
		void drawDots();

	private:

		Texture *texture, *dot;
		Vector *dots;
};

#endif