#pragma once

#include "VMath.h"
#include "AssetManager.h"
#include <vector>
#include <algorithm>
#include <Ray.h>
#include "CollisionComponent.h"
#include <Sphere.h>
#include <cmath> 
using namespace MATH;


class MEW {
private:

#pragma region "Code I Stole From Sem 2, Thanks Rashida"
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

        //If the vector is NAN (Not a number) then no intersection exists
        if (std::isnan(point.x)) { 
            return false; 
        }
        
        return true;
    }

    static bool RaySphereIntersection(Ray ray, Sphere sphere) {
        //Please work
        //https://kylehalladay.com/blog/tutorial/math/2013/12/24/Ray-Sphere-Intersection.html
        //Thanks btw

        Vec3 rayOrigin = ray.start;
        Vec3 rayDirection = ray.direction - ray.start;

        Vec3 L = sphere.center - rayOrigin;

        float tc = VMath::dot(L, rayDirection);
        if (tc < 0.0) return false;

        float d = sqrt(VMath::dot(L,L) - (tc * tc));
        if (d > sphere.r * sphere.r) { return false; }

        //solve for t1c
        float t1c = sqrt((sphere.r * sphere.r) - (d * d));

        //solve for intersection points
        float t1 = tc - t1c;
        float t2 = tc + t1c;

        return true;
    }

    /// Apperently Umer's ray has Start as the origin and 'Direction' as the end
    static bool RayAABBIntersection(Ray ray, AABB box) {
        Vec3 rayDir = ray.direction - ray.start;

        Vec3 boxMin = box.center - Vec3(box.rx, box.ry, box.rz);
        Vec3 boxMax = box.center + Vec3(box.rx, box.ry, box.rz);

        //TX
        float tx1 = (boxMin.x - ray.start.x) / rayDir.x;
        float tx2 = (boxMax.x - ray.start.x) / rayDir.x;
        if (rayDir.x == 0.0f) {
            // Check if ray.start.x is within the bounds of the box on the X axis
            if (ray.start.x < boxMin.x || ray.start.x > boxMax.x) {
                return false; // No intersection if the ray is parallel and not in range
            }
            // If the ray is parallel to the X axis, treat as infinite range on that axis
            tx1 = -std::numeric_limits<float>::infinity();
            tx2 = std::numeric_limits<float>::infinity();
        }
        else if (tx1 > tx2) {
            std::swap(tx1, tx2);
        }
        float tNearX = std::min(tx1, tx2);
        float tFarX = std::max(tx1, tx2);

        //TY
        float ty1 = (boxMin.y - ray.start.y) / rayDir.y;
        float ty2 = (boxMax.y - ray.start.y) / rayDir.y;
        if (rayDir.y == 0.0f) {
            // Check if ray.start.y is within the bounds of the box on the Y axis
            if (ray.start.y < boxMin.y || ray.start.y > boxMax.y) {
                return false; // No intersection if the ray is parallel and not in range
            }
            ty1 = -std::numeric_limits<float>::infinity();
            ty2 = std::numeric_limits<float>::infinity();
        }
        else if (ty1 > ty2) {
            std::swap(ty1, ty2);
        }
        float tNearY = std::min(ty1, ty2);
        float tFarY = std::max(ty1, ty2);


        //TZ
        float tz1 = (boxMin.z - ray.start.z) / rayDir.z;
        float tz2 = (boxMax.z - ray.start.z) / rayDir.z;
        if (rayDir.z == 0.0f) {
            // Check if ray.start.z is within the bounds of the box on the Z axis
            if (ray.start.z < boxMin.z || ray.start.z > boxMax.z) {
                return false; // No intersection if the ray is parallel and not in range
            }
            tz1 = -std::numeric_limits<float>::infinity();
            tz2 = std::numeric_limits<float>::infinity();
        }
        else if (tz1 > tz2) {
            std::swap(tz1, tz2);
        }
        float tNearZ = std::min(tz1, tz2);
        float tFarZ = std::max(tz1, tz2);
      

        //Finding the biggest TNear value
        float tmin = std::max(std::max(tNearX, tNearY), tNearZ);

        //Finding the smallest Tmax value
        float tmax = std::min(std::min(tFarX, tFarY), tFarZ);

        if (tmin > tmax) { 
            return false; 
        }

        //Checking if the tmin are within the bounds
        if (tmin > 1) {
            return false;
        }
        return true;
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