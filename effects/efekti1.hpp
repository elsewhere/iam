#ifndef _EFEKTI1_HPP_
#define _EFEKTI1_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Efekti1-luokka
//-------------------------------------------------------

class Efekti1 : public Base
{
	friend class Base;

	public:

		Efekti1();
		~Efekti1();
		
		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
};

#endif