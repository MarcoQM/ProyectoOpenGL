#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

namespace util
{
    template <class T>
    inline bool existDataInVector(std::vector<T> &v, T &data)
    {
        return find(v.begin(), v.end(), data) != v.end();
    }

}

#endif // UTIL_H