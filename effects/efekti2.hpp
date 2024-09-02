#ifndef _EFEKTI2_HPP_
#define _EFEKTI2_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Efekti1-luokka
//-------------------------------------------------------

class Efekti2 : public Base
{
	friend class Base;

	public:

		Efekti2();
		~Efekti2();
		
		bool init(unsigned long s, unsigned long e);
		void draw();

	private:		
};

#endif