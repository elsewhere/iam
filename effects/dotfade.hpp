#ifndef _DOTFADE_HPP_
#define _DOTFADE_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	DotFade-luokka
//-------------------------------------------------------

class DotFade : public Base
{
	friend class Base;

	public:

		DotFade();
		~DotFade();
		
		bool init(unsigned long s, unsigned long e);
		void draw();
		void release();		

	private:
		
		Texture *texture;		
};

#endif