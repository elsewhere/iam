#ifndef _SLIDE_HPP_
#define _SLIDE_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Blur-luokka
//-------------------------------------------------------

class Slide : public Base
{
	friend class Base;

	public:

		Slide();
		~Slide();
		
		bool init(unsigned long s, unsigned long e);
		void draw();		

	private:
		
		Texture *prev, *next;
};

#endif