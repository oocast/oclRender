#ifndef __PV_HELPER2_H__
#define __PV_HELPER2_H__
#include <memory>
#include <vector>
#include <fstream>
#include "csg.h"
#include "scene.h"
#include "interface.h"

static std::vector<Vector> targetList;
static std::vector<int> timeList;

//void CreateScene(Scene & sceneSightMark, Scene & sceneTargetMarks);
void AddAllTargets(std::ifstream &);
void MainFunc(Scene &, Scene &, int);

#endif //__PV_HELPER2_H__
