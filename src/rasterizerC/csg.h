#ifndef __CSG_H__
#define __CSG_H__
#include <memory>
#include "shape.h"
#include "geometry.h"

class CSG : public Shape
{
protected:
	std::shared_ptr<Shape> v1, v2;
public:
	CSG(Shape *v1, Shape *v2, const Color *color = nullptr);
};

/*
class Union : public CSG
{
public:
	Union(const Shape *v1, const Shape *v2, const Color *color = nullptr);
	Union transform(const Transform &t);
	bool contains(const Vector &p) const;
	float signed_distance_bound(const Vector &p) const;
};

class Intersection : public CSG
{
public:
	Intersection(const Shape *v1, const Shape *v2, const Color *color = nullptr);
	Intersection transform(const Transform &t);
	bool contains(const Vector &p) const;
	//float signed_distance_bound(const Vector &p) const;
};

class Subtraction : public CSG
{
public:
	Subtraction(const Shape *v1, const Shape *v2, const Color *color = nullptr);
	Subtraction transform(const Transform &t);
	bool contains(const Vector &p) const;
	//float signed_distance_bound(const Vector &p) const;
};
*/

#endif //__CSG_H__