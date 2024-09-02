#ifndef _TEHDAS_HPP_
#define _TEHDAS_HPP_

#include <stdio.h>
#include <math.h>
#include "../externs.hpp"

//edestakaisin liikkuva tikku
class Tikku
{
public:
	Tikku() {};
	Tikku(Vector &keskipiste, float pituus, float range, int suunta, float looping);
	~Tikku() {};

	//päädyt
	Vector v1;
	Vector v2;

	//liikerata
	Vector endpoint1;
	Vector endpoint2;

	//muut ominaisuudet
	Vector color;
	float length; //tikun pituus
	float repeat; //kuinka monta kertaa liikkuu edestakaisin
	int orientation; //miten päin tikku on

	void calcPos(float pos);
	void draw(float alpha);
};

//rakennuslaatta
class Laatta
{
public:
	Laatta(float alkuaika, float loppuaika, Vector &startpos, Vector &endpos, 
		   float leveys, float korkeus, Vector &vari, int suunta);

	//kulmat
	Vector v1;
	Vector v2;
	Vector v3;
	Vector v4;

	//liikerata
	Vector start;
	Vector stop;

	//muut
	Vector color;
	float width;
	float height;
	int orientation;

	//lähtöaika ja loppuaika
	float starttime;
	float endtime;

	void calcPos(float pos);
	void draw(float pos, float alpha);
};


class Tehdas : public Base
{
	public:

		Tehdas();
		~Tehdas();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		int tikkucount;
		Tikku **tikut;

		int laattacount;
		Laatta **laatat;

		int putkistocount;
		Vector *putkisto;

		Path *kamera;

		void drawAlusta(float pos, float alpha);
		void renderScene(float pos, float alpha);
};

#endif
