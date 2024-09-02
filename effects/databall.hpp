#ifndef _DATABALL_HPP_
#define _DATABALL_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	DataBall luokka
//-------------------------------------------------------

class DataBall : public Base
{
	friend class Base;

	public:

		DataBall();
		~DataBall();
		
		bool init(unsigned long s, unsigned long e);
		void draw();		
		
	private:

};

#endif