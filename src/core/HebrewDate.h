/*
 * Copyright 2025, Alex Hitech <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL v2 license.
 */
#pragma once

#include <string>
#include <ctime>

class HebrewDate {
public:
    HebrewDate(int hYear, int hMonth, int hDay);
    HebrewDate(std::time_t gTime, bool diaspora = true);

    int Day() const;
    int Month() const;
    int Year() const;
    
    std::string HebrewMonthName() const;

    std::string ToStringShort() const;
    std::string ToParsha() const;
    std::string ToHolidayName() const;

private:
    int hYear_;
    int hMonth_;
    int hDay_;
    bool diaspora_;
};

