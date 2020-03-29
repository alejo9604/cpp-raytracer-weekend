//
// Created by Santiago Alvarez Vargas on 2020-02-28.
//

#ifndef RAYTRACE_SWITCH_RAY_H
#define RAYTRACE_SWITCH_RAY_H


#include "vec3.h"

class ray
{
public:
    ray() {}

    ray(const vec3 &a, const vec3 &b)
    {
        A = a;
        B = b;
    }

    vec3 origin() const { return A; }

    vec3 direction() const { return B; }

    vec3 point_at_parameter(float t) const { return A + (t * B); }

    vec3 A;
    vec3 B;
};


#endif //RAYTRACE_SWITCH_RAY_H
