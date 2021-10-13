#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"

#include "hittable.h"

class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material> m) 
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m) {};
        
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        point3 center(double time) const;

    public:
        shared_ptr<material> mat_ptr;
        point3 center0, center1;
        double time0, time1;
        double radius;
};

point3 moving_sphere::center(double time) const {
    return center0 + (time-time0) / (time1 - time0) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 ca = r.origin() - center(r.time());
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
    vec3 outward_normal = (rec.p - center(r.time()))/radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr; 

    return true;
}

#endif