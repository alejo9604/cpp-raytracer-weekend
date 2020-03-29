//
// Created by Santiago Alvarez Vargas on 2020-02-28.
//

#ifndef RAYTRACE_SWITCH_SPHERE_H
#define RAYTRACE_SWITCH_SPHERE_H

#include "hitable.h"

class sphere : public hitable
{
public:
    sphere() {}

    sphere(vec3 cen, float r, material *mat) : center(cen), radius(r), mat_ptr(mat) {};

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;

    vec3 center;
    float radius;
    material *mat_ptr;
};

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
    // need to resolve the following quadratic equation to find t:
    //t*t*dot(B,​ B)​ + 2*t*dot(B,A​-C​) + dot(A-C,A​-C​) - R*R = 0
    // 0 means no solution, any other value represents the amount of solutions (max 2)
    // note: the 2 and 4 were removed as they were getting canceled in the equation
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(r.direction(), oc); // 2 *
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c; // - 4 * a * c
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif //RAYTRACE_SWITCH_SPHERE_H
