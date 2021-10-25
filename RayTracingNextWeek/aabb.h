#ifndef _AABB_H
#define _AABB_H

#include "rtweekend.h"

// Axis -Aligned Bounding Box
class aabb {
    public:
        aabb(){}
        aabb(const point3 &a, const point3 &b): minimum(a), maximum(b) {}
        point3 min() {return minimum;}
        point3 max() {return maximum;}

        // bool hit(const ray& r, double t_min, double t_max) cosnt {
        //     for(int i=0; i<3; i++) {
        //         auto t0 = fmin((minimum[i]-r.origin()[i])/r.direction()[i],
        //                         (maximum[i]-r.origin()[i])/r.direction()[i]);
        //         auto t1 = fmax((minimum[i]-r.origin()[i])/r.direction()[i],
        //                         (maximum[i]-r.origin()[i])/r.direction()[i]);
        //         t_min = fmax(t0, t_min);
        //         t_max = fmin(t1, t_max);
        //         if(tmax <= t_min) return false;
        //     }
        //     return true;
        // }

        // Optimized aabb method
        inline bool hit(const ray& r, double t_min, double t_max) {
            for(int i=0; i<3; i++) {
                auto invD = 1.0f / r.direction()[i];
                auto t0 = min()[i] - r.origin()[i] * invD;
                auto t1 = max()[i] - r.origin()[i] * invD;

                if(invD < 0.0f)
                    std::swap(t0, t1);
                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 > t_max ? t1 : t_max;
                if(t_min >= t_max)
                    return false;
            }
            return true;
        }
    private:
        point3 minimum; //两个slab的位置
        point3 maximum;
};

aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return aabb(small,big);
}

#endif//_AABB_H