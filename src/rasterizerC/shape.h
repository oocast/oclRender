#ifndef __SHAPE_H__
#define __SHAPE_H__
#include <memory>
#include "geometry.h"
#include "image.h"

enum ShapeType
{
	ELLIPSE, CONVEXPOLY
};

class SceneObject
{
public:
  virtual void draw(PPMImage &image, int super_sampling) = 0;
};

class Shape : public SceneObject
{
public:
  Color ShapeColor;
  AABox Bound;
public:
  Shape(const Color *ColorInputPointer = nullptr);
  virtual bool contains(const Vector &Point) const = 0;
  //virtual float signed_distance_bound(const Vector &p) const = 0;
  void draw(PPMImage &Image, int SuperSampling);
	virtual void getParameters(std::vector<float> &Parameters, ShapeType *ShapeType) = 0;
	virtual std::shared_ptr<Shape> transformPointer(const Transform &Xform) = 0;
	// TODO: transform return true caller (derived) class obj
	//template <class Shape_drv> // derived class of Shape
	//virtual Shape_drv transform(const Transform &transform) const = 0;
};

#endif //__SHAPE_H__