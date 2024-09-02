#ifndef _WATER_HPP_
#define _WATER_HPP_

#include "../externs.hpp"

#define VITUSTILASTUJA 300

class Lastu
{
	public:

		float life;
		float speed;
		Vector pos;
};

class WaterCell
{
public:
	Vector pos;
	Vector normal;

	float u, v;
private:
};

class WaterFace
{
public:
	WaterCell *v1;
	WaterCell *v2;
	WaterCell *v3;

};

class Water : public Base
{
	friend class Base;

	public:

		Water();
		~Water();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		void filterWater(float *source, float *dest);
		void calculateMesh(float time);
		void render(float pos, float alpha);

		void surfer(float *buffer, float xpos, float ypos, float size);
		void drop(float *buffer, int xpos, int ypos, float size);
		void picture(float *buffer, float alpha);

		void drawLastut(float pos);

		WaterFace *Faces;
		WaterCell *Grid;

		Vector *sphere;
		Lastu *lastut;
		Lastu *lastut2;
//		TextureGrid *tausta;

		float *water1;
		float *water2;
		bool waterflag;
		float frametime;

		int gridx, gridy;
};

#endif