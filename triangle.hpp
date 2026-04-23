#ifndef __triangle__
#define __triangle__

#include "vector3.hpp"
#include <vector>

using namespace std;

struct Triangle{
    Vector3 p0, p1, p2;

    Triangle();

    Vector3 (&shader)(Vector3 position, Vector3 normal, Vector3 uv, Vector3 light);
};

#endif