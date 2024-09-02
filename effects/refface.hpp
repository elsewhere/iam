#ifndef _REFFACE_HPP_
#define _REFFACE_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

class Heart
{
public:
	Heart();
	~Heart();
	void calculate(float pos);
	void draw(Vector &center, float pos, float alpha);
	Vector *points;

	int vaaka;
	int pysty;
};

//-------------------------------------------------------
//	RefFace-luokka
//-------------------------------------------------------

class RefFace : public Base
{
	friend class Base;

	public:

		RefFace();
		~RefFace();
		
		bool init(unsigned long s, unsigned long e);
		void draw();
		
	private:
		Heart *heart;
		Path *kamera;
};

#endif