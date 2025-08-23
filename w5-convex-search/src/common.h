/**
@file
@brief This file contains common declarations.
*/

#ifndef COMMON_H_INCD
#define COMMON_H_INCD

#include <vector>

/**
@brief This class stores a point.

This class stores a point with x and y co-ordinates of type float.

*/

class Point {
  public:
    float x;
    float y;
};

typedef std::vector<Point> PointList;

#endif
