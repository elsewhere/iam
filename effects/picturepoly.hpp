#ifndef _PICTUREPOLY_HPP_
#define _PICTUREPOLY_HPP_

#include "../externs.hpp"

class PicturePoly
{
public:

	PicturePoly() {};
	~PicturePoly() {};

	Vector startpos;
	Vector endpos;

	Vector startrotate;
	Vector endrotate;

	Vector uv1, uv2, uv3, uv4;
	Vector normal;

	float xsize;
	float ysize;
	float time;

};

#endif