#ifndef _Ripple2_HPP_
#define _Ripple2_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

class GridPoint2
{
	public:

		float u, v;
};

//-------------------------------------------------------
//	Ripple2-luokka
//-------------------------------------------------------

class Ripple2 : public Base
{
	friend class Base;

	public:

		Ripple2();
		~Ripple2();
		
		bool init(unsigned long s, unsigned long e);
		void draw();		

	private:
		
		Texture *texture;		
		GridPoint2 *grid;
};

#endif