#ifndef _STRINGS_HPP_
#define _STRINGS_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Joint-luokka
//-------------------------------------------------------

class Joint
{
	public:

		Vector position;
};

//-------------------------------------------------------
//	String-luokka
//-------------------------------------------------------

class String
{
	public:

		String();
		~String();
		
		Joint *joint;

		Vector position;
		float value, value2, value3, value4;
};

//-------------------------------------------------------
//	Strings-luokka
//-------------------------------------------------------

class Strings : public Base
{
	friend class Base;

	public:

		Strings();
		~Strings();
	
		bool init(unsigned long s, unsigned long e);
		void draw();		

	private:

		String *string;
};

#endif