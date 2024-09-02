#ifndef _CRYSTAL_HPP_
#define _CRYSTAL_HPP_

#include "../externs.hpp"

class Crystal : public Base
{
	public:

		Crystal();
		~Crystal();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		float *utable;
		float *vtable;
		float *utableadd;
		float *vtableadd;
//		Object kappale;
//		Object reunus;
		T3D *kappale;
		T3D *reunus;

		Path *kamera;
		
		void renderScene(float pos, float alpha);
};

#endif