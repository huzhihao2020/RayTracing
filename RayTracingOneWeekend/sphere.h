#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m) 
            : center(cen), radius(r), mat_ptr(m) {}
        
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        shared_ptr<material> mat_ptr;
        point3 center;
        double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 ca = r.origin() - center;
    auto a = r.direction().length_squared();
    auto b_half = dot(r.direction(), ca);
    auto c = ca.length_squared() - radius*radius;

    auto discriminant = b_half*b_half - a*c;
    if(discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // find the nearest point the lies in the acceptable range
    auto root = (-b_half-sqrtd)/a;
    if(root<t_min || t_max<root) {
        root = (-b_half+sqrtd)/a;
        if(root<t_min || t_max<root) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    // out_ward_normal 是sphere center指向intersection 的向量
    vec3 outward_normal = (rec.p - center)/radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr; 

    return true;
}

#endif

