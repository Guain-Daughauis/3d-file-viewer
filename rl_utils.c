#include "rl_utils.h"

Vector3 Vector3Make(float x, float y, float z) {
	Vector3 v;

	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

Color ColorMake(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	Color c;

	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;

	return c;
}