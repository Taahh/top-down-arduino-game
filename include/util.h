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