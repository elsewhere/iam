#include "mathematics.hpp"

Vector Mathematics::sphereToCartesian(float radius, float phi, float theta)
{
	float sp = (float)sin(phi);
	float cp = (float)cos(phi);
	float st = (float)sin(theta);
	float ct = (float)cos(theta);
	Vector value = Vector(radius*sp*ct, radius*sp*st, radius*cp);
	return value;
}
Vector Mathematics::cartesianToSphere(float x, float y, float z)
{
	float rho = (float)sqrt(x*x+y*y+z*z);
	if (rho == 0.0f) 
		return Vector(0,0,0);
	float phi = (float)acos(z/rho);
	float theta = (float)acos(x/(rho*sin(phi)));
	Vector value = Vector(rho, phi, theta);
	return value;
}
float Mathematics::integrateSimpson(float startpoint, float endpoint, int steps, float(*function)(float))
{
	float val = 0.0f;
	float h = (endpoint-startpoint)/steps;
	val += function(startpoint);
	val += function(endpoint);

	for (int i=0;i<steps-1;i++)
	{
		if (!(i&1)) //parillinen
			val += 4*function(h+i*h);
		else
			val += 2*function(h+i*h);
	}
	val *= h/3.0f;
	return val;
}
