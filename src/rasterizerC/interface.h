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
std::shared_ptr<Shape> Ring(float halfWidth = 1.0F, float halfHeight = 1.0F, 
                            float centerX = 0.0F, float centerY = 0.0F, 
                            float thickness = 0.01F, 
                            const Color * inputColorPointer = nullptr);
// Create a horizontal ellipse ring; UI-parameter-friendly
// TODO: use of template
std::shared_ptr<Shape> Ring(const Vector &, const Vector &, float,
                            const Color * inputColorPointer = nullptr);
std::shared_ptr<Shape> Star();
std::shared_ptr<Shape> Arrow();
#endif //__INTERFACE_H__