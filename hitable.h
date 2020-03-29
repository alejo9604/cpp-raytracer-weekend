//
// Created by Santiago Alvarez Vargas on 2020-02-28.
//

#ifndef RAYTRACE_SWITCH_HITTABLE_H
#define RAYTRACE_SWITCH_HITTABLE_H

class material;

struct hit_record
{
    float t;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
};

class hitable
{
public:
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

#endif //RAYTRACE_SWITCH_HITTABLE_H
