#ifndef _NOISE_HPP_
#define _NOISE_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Noise-luokka
//-------------------------------------------------------

class Noise : public Base
{
	friend class Base;

	public:

		Noise();
		~Noise();
		
		bool init(unsigned long s, unsigned long e);
		void draw();
		void release();

	private:

		float angle;		
		Texture *texture;

		unsigned int *data;
};

#endif