#ifndef __PV_HELPER_H__
#define __PV_HELPER_H__
#include <memory>
#include <vector>
#include "csg.h"
#include "scene.h"
#include "interface.h"

static std::vector<Vector> targetList;

// (circle Int circle) U lineseg U lineseg U lineseg
std::shared_ptr<CSG> SightMark(const Vector &, bool, Color * InputColorPointer = nullptr);

void AddTarget(float, float);
void CreateSightMarks(Scene &);
// make sure the scene contains only sightmarks
void RotateSightMarks(Scene &);
// Change all sight marks to lockon mode
void LockSightMarks(Scene &);

#endif //__PV_HELPER_H__