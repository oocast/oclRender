#include <cmath>
#include "csg.h"

CSG::
CSG(const Color *color, bool positive) :
Shape(color, positive)
{    
}

CSG::
CSG(const std::vector<std::shared_ptr<Shape>> &elements,
    const Color *color,
    bool positive) :
Shape(color, positive), elements(elements)
{
}

void CSG::
AddElement(const std::shared_ptr<Shape> &element)
{
    elements.push_back(element);
    if (elements.size() == 1)
    {
        // the first element of this CSG
        bound = element->bound;
    }
}

Union::
Union(const Color *color, bool positive):
CSG(color, positive)
{
}

Union::
Union(const std::vector<std::shared_ptr<Shape>> &elements,
      const Color *color,
      bool positive) :
CSG(elements, color, positive)
{
    std::vector<Vector> boundVertices;
    for (int i = 0; i < elements.size(); i++)
    {
        boundVertices.push_back(elements[i]->bound.low);
        boundVertices.push_back(elements[i]->bound.high);
    }
    bound = AABox::FromVectors(&boundVertices[0], boundVertices.size());
}

bool Union::
Contains(const Vector &point) const
{
    bool result = false;
    for (int i = 0; i < elements.size(); i++)
    {
        result += elements[i]->Contains(point);
    }
    result = (result == positive);
    return result;
}

void Union::
GetParameters(std::vector<float> &parameters, std::vector<int> &structures)
{
    // Since Union should be the top level
    // Structures passed in should be {1, 1, 1, 1}
    structures[0] = (int)positive;
    structures[1] = elements.size();
    for (int i = 0; i < elements.size(); i++)
    {
        elements[i]->GetParameters(parameters, structures);
    }
}

void Union::
AddElement(const std::shared_ptr<Shape> &element)
{
    CSG::AddElement(element);
    // update bound
    std::vector<Vector> boundVertices({ bound.low, bound.high, element->bound.low, element->bound.high });
    bound = AABox::FromVectors(&boundVertices[0], 4);
}
/*
Union::
Union(const Shape *v1, const Shape *v2, const Color *color) :
CSG(v1, v2, color)
{
  Vector vs[4] = { v1->bound.low, v1->bound.high,
    v2->bound.low, v2->bound.high };
  bound = AABox::from_vectors(vs, 4);
}

bool Union::
contains(const Vector &p) const
{
  return v1->contains(p) || v2->contains(p);
}
*/

/*
double Union::
signed_distance_bound(const Vector &p) const
{
  double b1, b2;
  b1 = v1->signed_distance_bound(p);
  b2 = v2->signed_distance_bound(p);
  return fmax(b1, b2);
}
*/

/*
Union Union::
transform(const Transform &t)
{
  //return Union(v1->transform(t))
}
*/

Intersection::
Intersection(const Color *color, bool positive) :
CSG(color, positive)
{
}

Intersection::
Intersection(const std::vector<std::shared_ptr<Shape>> &elements,
             const Color *color,
             bool positive) :
CSG(elements, color, positive)
{
}

Intersection::
Intersection(const std::shared_ptr<Shape> &element,
             const Color *color,
             bool positive) :
CSG(std::vector<std::shared_ptr<Shape>>(1, element), color, positive)
{
    bound = element->bound;
}

Intersection::
Intersection(Shape* element,
						 const Color *color = nullptr,
						 bool positive = true):
CSG(std::vector<std::shared_ptr<Shape>>(1, std::shared_ptr<Shape>(element)), color, positive)
{
	bound = element->bound;
}

bool Intersection::
Contains(const Vector &point) const
{
    bool result = true;
    for (int i = 0; i < elements.size(); i++)
    {
        result *= elements[i]->Contains(point);
    }
    result = (result == positive);
    return result;
}

void Intersection::
GetParameters(std::vector<float> &parameters, std::vector<int> &structures)
{
    // Since Union should be the 2nd top level
    if (structures.size() == 4) {
        // It's the first Intersection under a Union
        // Assume Intersection size is always at least 1
        structures[2] = (int)positive;
        structures[3] = elements.size();
    }
    else
    {
        structures.push_back((int)positive);
        structures.push_back(elements.size());
    }
    for (int i = 0; i < elements.size(); i++)
    {
        elements[i]->GetParameters(parameters, structures);
    }
}

void Intersection::
AddElement(const std::shared_ptr<Shape> &element)
{
    CSG::AddElement(element);
    bound = bound.Intersection(element->bound);
}
