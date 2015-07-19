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
    //void GetParameters(std::vector<float> &, std::vector<int> &);
    //std::shared_ptr<Shape> TransformPointer(const Transform &);
    virtual void AddElement(const std::shared_ptr<Shape> &);
    std::shared_ptr<Shape> AccessElement(size_t);
};

/*!!!!!!!!!!!!!
IMPORTANT:
CSG must be used as:
A Union of Intersections of Primitives
Or
An Intersection of Primitives 
!!!!!!!!!!!!!*/

class Union : public CSG
{
public:
    // default
    Union(const Color *color = nullptr, bool positive = true);
    // list constructor
    Union(const std::vector<std::shared_ptr<Shape>> &,
          const Color *color = nullptr,
          bool positive = true);
    // move constructor
    Union(const Union &&);
    // copy constructor
    //Union(const Union &);
    bool Contains(const Vector &) const;
    void GetParameters(std::vector<float> &, std::vector<int> &);
    std::shared_ptr<Shape> TransformPointer(const Transform &);
	void AddElement(const std::shared_ptr<Shape> &);
    void CalculateBound(float);
};

class Intersection : public CSG
{
public:
    // default
    Intersection(const Color *color = nullptr,
                 bool positive = true);
    // list constructor
    Intersection(const std::vector<std::shared_ptr<Shape>> &,
                 const Color *color = nullptr,
                 bool positive = true);
    // single element constructor
	Intersection(const std::shared_ptr<Shape> &,
                 const Color *color = nullptr,
                 bool positive = true);
	Intersection(Shape*,
                 const Color *color = nullptr,
                 bool positive = true);
    // move constructor
    Intersection(const Intersection &&);
    // copy constructor
    //Intersection(const Intersection &);
    bool Contains(const Vector &) const;
    void GetParameters(std::vector<float> &, std::vector<int> &);
    std::shared_ptr<Shape> TransformPointer(const Transform &);
    void AddElement(const std::shared_ptr<Shape> &);
    void CalculateBound(float);
};


#endif //__CSG_H__
