/*
 * Copyright 2025, Alex Hitech <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL v2 license.
 */
#include "HebrewDate.h"


#include "HebrewDate.h"
#include "hdate.h"
#include <sstream>
#include <cstdlib>
#include <Catalog.h>
#include <LocaleRoster.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "HebrewDate"

HebrewDate::HebrewDate(int hYear, int hMonth, int hDay)
    : hYear_(hYear), hMonth_(hMonth), hDay_(hDay), diaspora_(true) {}

HebrewDate::HebrewDate(std::time_t gTime, bool diaspora)
    : diaspora_(diaspora) {
    struct tm *lt = localtime(&gTime);
    hdate_struct h;
    hdate_set_gdate(&h, lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
    hYear_ = h.hd_year;
    hMonth_ = h.hd_mon;
    hDay_ = h.hd_day;
}

int HebrewDate::Day() const { return hDay_; }
int HebrewDate::Month() const { return hMonth_; }
int HebrewDate::Year() const { return hYear_; }

std::string HebrewDate::ToStringShort() const {
    char* month_name = hdate_get_hebrew_month_string(hMonth_, HDATE_SHORT_FLAG);
    std::ostringstream oss;
    oss << hDay_ << " " << (month_name ? B_TRANSLATE(month_name) : "") << " " << hYear_;
    return oss.str();
}

std::string HebrewDate::ToParsha() const {
    hdate_struct h;
    hdate_set_hdate(&h, hDay_, hMonth_, hYear_);
    int parasha_code = hdate_get_parasha(&h, diaspora_ ? HDATE_DIASPORA_FLAG : HDATE_ISRAEL_FLAG);
    if (parasha_code <= 0) return "";
    char* name = hdate_get_parasha_string(parasha_code, HDATE_SHORT_FLAG);
    return name ? B_TRANSLATE(name) : "";
}

std::string HebrewDate::ToHolidayName() const {
    hdate_struct h;
    hdate_set_hdate(&h, hDay_, hMonth_, hYear_);
    int holiday_code = hdate_get_holyday(&h, diaspora_ ? HDATE_DIASPORA_FLAG : HDATE_ISRAEL_FLAG);
    if (holiday_code <= 0) return "";
    char* name = hdate_get_holyday_string(holiday_code, HDATE_SHORT_FLAG);
    return name ? B_TRANSLATE(name) : "";
}
