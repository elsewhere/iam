#ifndef _Keiju_HPP_
#define _Keiju_HPP_

#include <stdio.h>
#include "../externs.hpp"

class KeijuItem
{
public:
	KeijuItem() {};
	~KeijuItem() {};

	Vector pos;
	Vector rotate;
	Vector rotatespeed;

	Vector beziertangent;
	Vector bezierrotate;
	Vector v1, v2, v3;
	Vector n;

};
class Keiju : public Base
{
	public:

		Keiju();
		~Keiju();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		void drawFlare(Vector &pos, float size);

		int itemcount;
		int bezierstrips;
		KeijuItem *items;
		Vector *path;

		void renderScene(float pos, float alpha);
};

#endif