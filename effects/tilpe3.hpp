#ifndef _TILPE3_HPP_
#define _TILPE3_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Tilpe3-luokka
//-------------------------------------------------------

class Tilpe3 : public Base
{
	friend class Base;

	public:

		Tilpe3();
		~Tilpe3();
		
		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

};

#endif