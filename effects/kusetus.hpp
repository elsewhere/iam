#ifndef _KUSETUS_HPP_
#define _KUSETUS_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Isoooo kusetus-luokka
//-------------------------------------------------------

class Kusetus : public Base
{
	friend class Base;

	public:

		Kusetus();
		~Kusetus();
		
		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

};

#endif