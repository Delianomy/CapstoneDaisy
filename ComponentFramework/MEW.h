#pragma once

#include "VMath.h"
#include "AssetManager.h"
#include <vector>
#include <algorithm>
#include <Ray.h>
#include "CollisionComponent.h"
using namespace MATH;


class MEW {
private:

#pragma region "Code I stole from sem 2"
    static bool RayAABCollisionDetected(const Ray& ray, const AABB& box) {
        if (ray.direction.x == 0 && ray.direction.y == 0 && ray.direction.z == 0) {
            return false;
        }

        return true;
    }

    //Checks if the intersection point that we found is valid
    static bool AABIntersectionCheck(Vec3 point, AABB box) {
        if ((point.x >= 0 && point.x <= box.rx) &&
            (point.y >= 0 && point.y <= box.ry) &&
            (point.z >= 0 && point.z <= box.rz)) {
            return true;
        }
        return false;
    }

    //Finding the postion of the ray that intersects with the box
    //t = rx - sx / vx
    static float AABCalculateIntersection(float boxVert, float start, float dir) {
        return (boxVert - start) / dir;
    }

    static Vec3 RayAABCollisionPoint(const Ray& ray, const AABB& box) {
        float testingVert; //Which r we are using to find the intersection point
        float t;
        Vec3 point; //point of intersection that we calculated

        if (!RayAABCollisionDetected(ray, box)) {
            return Vec3(NAN, NAN, NAN);
        }

        if (ray.direction.x != 0) {
            //Assigns the appropriate r value based on the ray's direction
            testingVert = (ray.direction.x > 0) ? 0 : box.rx;
            t = AABCalculateIntersection(testingVert, ray.start.x, ray.direction.x);
            point = ray.getPos(t);
            //Checks if the intersection point that we found is valid. If it does return it
            if (AABIntersectionCheck(point, box)) {
                return point;
            }
        }

        if (ray.direction.y != 0) {
            testingVert = (ray.direction.y > 0) ? 0 : box.ry;
            t = AABCalculateIntersection(testingVert, ray.start.y, ray.direction.y);
            point = ray.getPos(t);
            if (AABIntersectionCheck(point, box)) {
                return point;
            }
        }

        if (ray.direction.z != 0) {
            testingVert = (ray.direction.z > 0) ? 0 : box.rz;
            t = AABCalculateIntersection(testingVert, ray.start.z, ray.direction.z);
            point = ray.getPos(t);
            if (AABIntersectionCheck(point, box)) {
                return point;
            }
        }

        //In case any of the other checks fail, returns null
        return Vec3(NAN, NAN, NAN);
    }

#pragma endregion

public:

    static bool RayBoxIntersection(Ray ray, AABB box) {
        // ref: https://gdbooks.gitbooks.io/3dcollisions/content/Chapter3/raycast_aabb.html
        Vec3 point = RayAABCollisionPoint(ray, box);

        if (point.x != NAN) { return true; }
        
        return false;
    }

    //Printing a vector list
    static void PrintVecList(std::vector<Vec3>& list) {
        for (Vec3 &v : list) {
            v.print();
        }
    }


    static float GetMaxX(std::vector<Vec3>& list) {
        //Creating a list of floats with the x member value
        std::vector<float> f;
        for (Vec3& v : list) {
            f.push_back(v.x);
        }

        if (f.size() <= 0) { return 404.0000f; }

        float currentLargets = f[0];
        for (float fv : f) {
            if (fv >= currentLargets) {
                currentLargets = fv;
            }
        }

        return currentLargets;
    }

    static float GetMinX(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3& v : list) {
            f.push_back(v.x);
        }

        if (f.size() <= 0) { return 404.0000f; }

        float currentSmallest = f[0];
        for (float fv : f) {
            if (fv <= currentSmallest) {
                currentSmallest = fv;
            }
        }

        return currentSmallest;
    }


    static float GetMaxY(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3& v : list) {
            f.push_back(v.y);
        }

        if (f.size() <= 0) { return 404.0000f; }

        float currentLargets = f[0];
        for (float fv : f) {
            if (fv >= currentLargets) {
                currentLargets = fv;
            }
        }

        return currentLargets;
    }

    static float GetMinY(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3& v : list) {
            f.push_back(v.y);
        }

        if (f.size() <= 0) { return 404.0000f; }

        float currentSmallest = f[0];
        for (float fv : f) {
            if (fv <= currentSmallest) {
                currentSmallest = fv;
            }
        }

        return currentSmallest;
    }


    static float GetMaxZ(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3& v : list) {
            f.push_back(v.z);
        }

        if (f.size() <= 0) { return 404.0000f; }

        float currentLargets = f[0];
        for (float fv : f) {
            if (fv >= currentLargets) {
                currentLargets = fv;
            }
        }

        return currentLargets;
    }

    static float GetMinZ(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3& v : list) {
            f.push_back(v.z);
        }

        if (f.size() <= 0) { return 404.0000f; }

        float currentSmallest = f[0];
        for (float fv : f) {
            if (fv <= currentSmallest) {
                currentSmallest = fv;
            }
        }

        return currentSmallest;
    }


};