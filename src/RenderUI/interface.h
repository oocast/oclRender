#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include <memory>
#include "poly.h"
#include "ellipse.h"
#include "csg.h"

// Create a horizontal rectangular frame
std::shared_ptr<Shape> Frame(const Vector &, const Vector &, float, 
                             const Color * inputColorPointer = nullptr);
// Create a horizontal ellipse ring
std::shared_ptr<CSG> Ring(float halfWidth = 1.0F, float halfHeight = 1.0F, 
                            float centerX = 0.0F, float centerY = 0.0F, 
                            float thickness = 0.01F, 
                            const Color * inputColorPointer = nullptr);
// Create a horizontal ellipse ring; UI-parameter-friendly
// TODO: use of template
std::shared_ptr<CSG> Ring(const Vector &, const Vector &, float,
                            const Color * inputColorPointer = nullptr);
std::shared_ptr<Shape> Star();
// the int: direction: 0right, 1up, 2left, 3down
std::shared_ptr<Shape> Arrow(const Vector &, const Vector &,
                             int, const Color * inputColorPointer = nullptr,
                             float lengthRatio = 0.5F, float widthRatio = 0.5F);
// Brush tool
std::shared_ptr<CSG> BrushInit(const Vector &, const Vector &, float,
                               Vector &, const Color * inputColorPointer = nullptr);
std::shared_ptr<CSG> BrushAppend(const Vector &, std::shared_ptr<CSG>, float, Vector &);

std::shared_ptr<CSG> BezierCurve(const Vector &, const Vector &, const Vector &, const float);
std::shared_ptr<CSG> BezierCurve(const std::vector<Vector> &, const float);
std::shared_ptr<CSG> EllipseRing(const float, const float, const float, const float, const float, const float, const float);
std::shared_ptr<CSG> ParabolaRing(const float, const float, const float, const float, const float, const float, const float);
#endif //__INTERFACE_H__
