#ifndef CGRAPH_MYFUNCTION_H
#define CGRAPH_MYFUNCTION_H

#include <string>
inline int add(int i, int j) {
    return i + j;
}

static float minusBy5(float i) {
    return i - 5.0f;
}


class MyFunction {
public:
    std::string pow2(std::string& str) const {
        int result = 1;
        int pow = power_;
        while (pow--) {
            result *= atoi(str.c_str());
        }
        return "multiply result is : " + std::to_string(result);
    }

    static int divide(int i, int j) {
        if (0 == j) {
            return 0;
        }

        return i / j;
    }

private:
    int power_ = 2;
};

#endif