#ifndef _TILPE1_HPP_
#define _TILPE1_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Tilpe1-luokka
//-------------------------------------------------------

class Tilpe1 : public Base
{
	friend class Base;

	public:

		Tilpe1();
		~Tilpe1();
		
		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

};

#endif