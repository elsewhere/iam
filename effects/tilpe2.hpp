#ifndef _TILPE2_HPP_
#define _TILPE2_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Tilpe2-luokka
//-------------------------------------------------------

class Tilpe2 : public Base
{
	friend class Base;

	public:

		Tilpe2();
		~Tilpe2();
		
		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

};

#endif