#ifndef _METATYPES_HPP_
#define _METATYPES_HPP_

#ifndef MAXGRIDSIZE
	#define MAXGRIDSIZE 30
#endif

class MetaGridPoint
{
	public:

		MetaGridPoint()
		{
			x = y = z = 0.0f;
			nx = ny = nz = 0.0f;
			value = 0.0f;
			alpha = 1.0f;
			isComputed = false;
		};

		float x, y, z;
		float nx, ny, nz;
		float value;
		float alpha;
		bool isComputed;
};

class MetaGridCube
{
	public:

		MetaGridCube()
		{
		}
					
		MetaGridPoint *vertices[8];
};

class MetaBall
{
	public:

		MetaBall()
		{
			x = y = z = 0.0f;
			radius = 0.0f;
		}

		inline init(float xx, float yy, float zz, float rr)
		{
			x = xx; y = yy; z = zz;
			radius = rr;
		}

		float x, y, z;
		float radius;
		float speed;
};

#endif