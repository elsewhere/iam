#ifndef _LIEROT_HPP_
#define _LIEROT_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

#ifndef MAXJOINTS
	#define MAXJOINTS 20
#endif

#ifndef MAXLIEROT
	#define MAXLIEROT 300
#endif

class Liero
{
	public:
		
		float speed;
		float nx, ny, nz;
		Vector joints[MAXJOINTS];
};

//-------------------------------------------------------
//	Lierot-luokka
//-------------------------------------------------------

class Lierot : public Base
{
	friend class Base;

	public:

		Lierot();
		~Lierot();
		
		bool init(unsigned long s, unsigned long e);
		void draw();		
		
	private:

		Liero liero[MAXLIEROT];
		Texture *flare;
};

#endif