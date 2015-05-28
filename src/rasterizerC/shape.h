#ifndef __SHAPE_H__
#define __SHAPE_H__
#include "geometry.h"
#include "image.h"

class SceneObject
{
	virtual void draw(PPMImage &image, int super_sampling) = 0;
};

class Shape : public SceneObject
{
public:
	Color color;
	AABox bound;
public:
 	Shape(const Color *o_color = null_ptr);
 	virtual bool contains(const Vector &p) = 0;
 	virtual double signed_distance_bound(const Vector &p) = 0;
 	virtual void draw(PPMImage &image, int super_sampling);
};

#endif //__SHAPE_H__