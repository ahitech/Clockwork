/*
 * Copyright 2025, Alex Hitech <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL v2 license.
 */
#include "../src/core/HebrewDate.h"
#include <cassert>
#include <iostream>

int main() {
    HebrewDate hd1(5785, 1, 1);
    assert(hd1.Day() == 1);
    assert(hd1.Month() == 1);
    assert(hd1.Year() == 5785);
    std::cout << "Fixed date: " << hd1.ToStringShort() << std::endl;

    std::time_t now = std::time(nullptr);
    HebrewDate hd2(now, true); // true = diaspora
    std::cout << "Current Hebrew Date: " << hd2.ToStringShort() << std::endl;
    std::cout << "Parsha: " << hd2.ToParsha() << std::endl;
    std::cout << "Holiday: " << hd2.ToHolidayName() << std::endl;

    return 0;
}

