#ifndef GEOM_H
#define GEOM_H

#include <vector>

namespace sherkunov
{
    struct Point
    {
        int x;
        int y;
    };

    struct Polygon
    {
        std::vector< Point > points;
    };
}

#endif
