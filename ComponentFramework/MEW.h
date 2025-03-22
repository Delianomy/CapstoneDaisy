#pragma once

#include "VMath.h"
#include "AssetManager.h"
#include <vector>
#include <algorithm>
using namespace MATH;


class MEW {
public:
    void DrawWireSphere(std::shared_ptr<AssetManager> assetManager, Vec3 pos, float r){
    
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