#ifndef _START_HPP_
#define _START_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Start-luokka
//-------------------------------------------------------

class Start : public Base
{
	friend class Base;

	public:

		Start();
		~Start();
		
		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

};

#endif