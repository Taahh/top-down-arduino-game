#ifndef TUPLE_H
#define TUPLE_H
template <typename T, typename K>
struct Tuple
{
    T left;
    K right;
};

#endif

#ifndef PIXEL_H
#define PIXEL_H
struct Pixel
{
    int x;
    int y;
    uint16_t color;
};
#endif

#ifndef UTIL_FUNCS_H
#define UTIL_FUNCS_H
#include <math.h>
int distance(short x1, short y1, short x2, short y2) {
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}


#endif