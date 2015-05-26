#ifndef __SHAPE_H__
#define __SHAPE_H__
#include "geometry.h"

class SceneObject
{
	virtual void draw(const PPMImage &image) = 0;
};

class Shape : public SceneObject
{
	Color color;
	AABox bound;
public:
 	Shape(const Color *o_color = null_ptr);
 	virtual bool contains(const Vector &p) = 0;
 	virtual double signed_distance_bound(const Vector &p) = 0;
 	void draw(const PPMImage &image, int super_sampling);
};

#endif //__SHAPE_H__