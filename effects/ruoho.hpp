#ifndef _RUOHO_HPP_
#define _RUOHO_HPP_

#include <stdio.h>

#include "../externs.hpp"

class Ruohonkorsi
{
public:
	Vector pohja;
	Vector karki;
//	Vector vari;
};

class Voikukka
{
public:
	void generate();
	void destroy();
	void draw(float alpha);

	int varsicount;
	int haituvacount;
	Vector *varsi;
	Vector *haituvat;
};

class Timotei
{
public:
	void generate();
	void destroy();
	void draw(float alpha);

	int varsicount;
	int haituvacount;

	Vector *varsi;
	Vector *haituvat;

};


class Ruoho : public Base
{
	public:

		Ruoho();
		~Ruoho();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		int ruohocount;
		int voikukkacount;
		int timoteicount;
		Ruohonkorsi *nurmikko;
		Voikukka *voikukat;
		Timotei *timoteit;

		float *ruohobuffer;

		Path *kamera;

		void renderScene(float pos, float alpha);
};

#endif