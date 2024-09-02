#ifndef _FADEIN_HPP_
#define _FADEIN_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"
#include "gridtypes.hpp"

//-------------------------------------------------------
//	FadeIn-luokka
//-------------------------------------------------------

class FadeIn : public Base
{
	friend class Base;

	public:

		FadeIn();
		~FadeIn();
		
		bool init(unsigned long s, unsigned long e);
		void draw();		

	private:
		
		Texture *texture;
		GridPoint2D *grid;
};

#endif