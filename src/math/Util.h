#pragma once

#include <cmath>

class Util {
public:
    template<typename T>
    static T Lerp(T a0, T a1, double w) {
        return a0*(1.0 - w) + a1*w;
    }

    static double Wrap(double what, double to = 1.0) {
        double r = std::remainder(what, to);
        return r < 0 ? r + to : r;
    }
};
