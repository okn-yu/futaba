/*
 * Created by okn-yu on 2022/05/05.
 */

#ifndef PRACTICEPATHTRACING_SPHERE_H
#define PRACTICEPATHTRACING_SPHERE_H

#include <cmath>
#include <memory>
#include <utility>
#include "futaba/core/config.h"
#include "futaba/core/ray.h"
#include "futaba/core/vec3.h"
#include "futaba/render/hit.h"


class Sphere {
public:
    Vec3 center;
    float radius;
    Sphere(const Vec3 &_center, float _radius) : center(_center), radius(_radius) {};

    bool is_hittable(Ray &ray, HitRecord &hit_record) const {
        float b = dot(ray.direction, ray.origin - center);
        float c = (ray.origin - center).squared_length() - radius * radius;
        float D = b * b - c;
        float t;

        if (D < 0) {
            return false;
        } else {
            // t1 <= t2
            float t1 = -b - std::sqrt(D);
            float t2 = -b + std::sqrt(D);

            // HIT_DISTANCE_MAX < t1 < t2
            if (t1 > HIT_DISTANCE_MAX)
                return false;
            // t1 < t2 < HIT_DISTANCE_MIN
            if (t2 < HIT_DISTANCE_MIN)
                return false;
            // t1 < HIT_DISTANCE_MIN, HIT_DISTANCE_MAX < t2
            if (t1 < HIT_DISTANCE_MIN & HIT_DISTANCE_MAX < t2)
                return false;

            // 衝突の結果t1もしくはt2が採用される
            // t1が採用されるのは下の場合のみ
            // HIT_DISTANCE_MIN < t1　< HIT_DISTANCE_MAX,  t2とHIT_DISTANCE_MAXの関係は任意
            if (t1 > HIT_DISTANCE_MIN)
                t = t1;
                // t2が採用されるのは以下の場合のみ
                // t1 < HIT_DISTANCE_MIN < t2　< HIT_DISTANCE_MAX
            else
                t = t2;

            hit_record.t = t;
            hit_record.hit_object = this;
            hit_record.hit_pos = ray(t);
            hit_record.hit_normal = unit_vec(hit_record.hit_pos - center);

            return true;
        }
    }
};

#endif //PRACTICEPATHTRACING_SPHERE_H