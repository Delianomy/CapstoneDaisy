#pragma once

#include "VMath.h"
#include <vector>
#include <algorithm>
using namespace MATH;


class MEW {
public:
    //Printing a vector list
    static void PrintVecList(std::vector<Vec3> list) {
        for (Vec3 v : list) {
            v.print();
        }
    }


    static float GetMaxX(std::vector<Vec3> list) {
        //Creating a list of floats with the x member value
        std::vector<float> f;
        for (Vec3& v : list) {
            f.push_back(v.x);
        }

        std::reverse(f.begin(), f.end());

        return f[0];
    }

    static float GetMinX(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3 &v : list) {
            f.push_back(v.x);
        }

        std::sort(f.begin(), f.end());

        return f[0];
    }


    static float GetMaxY(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3 &v : list) {
            f.push_back(v.y);
        }

        std::reverse(f.begin(), f.end());

        return f[0];
    }

    static float GetMinY(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3 &v : list) {
            f.push_back(v.y);
        }

        std::sort(f.begin(), f.end());

        return f[0];
    }


    static float GetMaxZ(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3 &v : list) {
            f.push_back(v.z);
        }

        std::reverse(f.begin(), f.end());

        return f[0];
    }

    static float GetMinZ(std::vector<Vec3>& list) {
        std::vector<float> f;
        for (Vec3 &v : list) {
            f.push_back(v.z);
        }

        std::sort(f.begin(), f.end());

        return f[0];
    }
};