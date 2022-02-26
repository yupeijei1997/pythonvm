#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "object/hiObject.hpp"
#include "object/hiInteger.hpp"


class Universe {
public:
    static HiInteger* HiTrue;
    static HiInteger* HiFalse;

    static HiObject* HiNone;

public:
    static void genesis();
    static void destory();
};

#endif