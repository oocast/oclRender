#ifndef __CSG_H__
#define __CSG_H__
#include <memory>
#include <vector>
#include "shape.h"
#include "geometry.h"

class CSG : public Shape
{
protected:
  std::vector<std::shared_ptr<Shape>> elements;
public:
	CSG(const Color *color = nullptr,
			bool positive = true);
  CSG(const std::vector<std::shared_ptr<Shape>> &, 
      const Color *color = nullptr, 
      bool positive = true);
	void GetParameters(std::vector<float> &, std::vector<int> &);
	std::shared_ptr<Shape> TransformPointer(const Transform &);
	void AddElement(std::shared_ptr<Shape>);
};

class Union : public CSG
{
public:
  Union(std::vector<std::shared_ptr<Shape>> &,
        const Color *color = nullptr,
        bool positive = true);
  Union(const Union &&);
  bool Contains(const Vector &) const;
  //void GetParameters(std::vector<float> &, std::vector<int> &);
  //std::shared_ptr<Shape> TransformPointer(const Transform &);
};

class Intersection : public CSG
{
public:
  Intersection(std::vector<std::shared_ptr<Shape>> &,
               const Color *color = nullptr,
               bool positive = true);
  Intersection(const Intersection &&);
  bool Contains(const Vector &) const;
  //void GetParameters(std::vector<float> &, std::vector<int> &);
  //std::shared_ptr<Shape> TransformPointer(const Transform &);
};


#endif //__CSG_H__