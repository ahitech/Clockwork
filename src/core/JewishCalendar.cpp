/*
 * Copyright 2026, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


#include "JewishCalendar.h"

#include <Catalog.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "JewishCalendar"

GregorianDate GregorianDateFromTm(const struct tm& t)
{
	return {
		static_cast<uint>(t.tm_year + 1900),
		static_cast<uint>(t.tm_mon + 1),
		static_cast<uint>(t.tm_mday)
	};
}

struct tm TMFromGregorianDate(const GregorianDate& in) {
	struct tm t = {};
	t.tm_year = in.year - 1900;
	t.tm_mon  = in.month - 1;
	t.tm_mday = in.day;
	t.tm_hour = 12;	// Avoid problems with DST
	return t;
}

GregorianDate AddDays(const GregorianDate& in, int days) {
	struct tm t = TMFromGregorianDate(in);
	t.tm_mday += days;
	time_t time = mktime(&t);
	struct tm normalized;
	localtime_r (&time, &normalized);
	return GregorianDateFromTm(normalized);
}

int DaysBetween(const GregorianDate& in1, const GregorianDate &in2) {
	struct tm a = TMFromGregorianDate(in1);
	struct tm b = TMFromGregorianDate(in2);	
	time_t ta = mktime(&a), tb = mktime(&b);
	double seconds = difftime(tb, ta);
	return (int)(seconds / (24*60*60));
}

JewishCalendar::JewishCalendar(bool diaspora) :
	_diaspora(diaspora)
{
}

HebrewDate JewishCalendar::ToHebrewDate(const GregorianDate& in) const
{
	hdate_struct hs;
	hdate_set_gdate (&hs, in.day, in.month, in.year);
	HebrewDate toReturn(hs.hd_year, hs.hd_mon, hs.hd_day);
	return toReturn;
}
	
GregorianDate	JewishCalendar::ToGregorianDate(const HebrewDate& in) const
{
	hdate_struct hs;
	hdate_set_hdate(&hs, in.Day(), in.Month(), in.Year());
	GregorianDate toReturn(hs.gd_year, hs.gd_mon, hs.gd_day);
	return toReturn;
}


HebrewDate JewishCalendar::Today() const
{
	return HebrewDate(std::time(nullptr));
}

GregorianDate JewishCalendar::AddDays(const GregorianDate& in, int days) const
{
	struct tm tmDate = {};
	tmDate.tm_year = in.year - 1900;
	tmDate.tm_mon  = in.month - 1;
	tmDate.tm_mday = in.day + days;

	// Noon is better than midnight when DST is involved.
	tmDate.tm_hour = 12;
	tmDate.tm_isdst = -1;

	time_t normalized = mktime(&tmDate);

	return GregorianDate(normalized);
}


int JewishCalendar::HolidayId(const GregorianDate& date) const
{
	hdate_struct hdate;
	hdate_set_gdate(&hdate, date.day, date.month, date.year);
	return hdate_get_holyday_type(hdate_get_holyday(&hdate, _diaspora ? 1 : 0));
}

int JewishCalendar::HolidayType(const GregorianDate& date) const
{
	const int holidayId = HolidayId(date);

	// In libhdate, zero usually means "no holiday".
	// Returning zero here keeps the same convention for holiday type.
	if (holidayId == 0)
		return 0;

	// This returns libhdate's holiday type classification.
	// The meaning of the numeric type is still libhdate-specific.
	return hdate_get_holyday_type(holidayId);
}

int JewishCalendar::DaysInHebrewMonth(uint year, uint month) const
{
	if (year == 0 || month == 0 || month > 14)
		return 0;

	hdate_struct currentMonth;
	hdate_set_hdate(&currentMonth, 1, month, year);

	if ((uint)hdate_get_hyear(&currentMonth) != year
			|| (uint)hdate_get_hmonth(&currentMonth) != month
			|| hdate_get_hday(&currentMonth) != 1) {
		return 0;
	}

	uint nextYear = year;
	uint nextMonth = month + 1;

	if (nextMonth > 14) {
		nextMonth = 1;
		nextYear++;
	}

	hdate_struct nextMonthDate;
	hdate_set_hdate(&nextMonthDate, 1, nextMonth, nextYear);

	if ((uint)hdate_get_hyear(&nextMonthDate) != nextYear
			|| (uint)hdate_get_hmonth(&nextMonthDate) != nextMonth
			|| hdate_get_hday(&nextMonthDate) != 1) {
		return 0;
	}

	const int days = hdate_get_julian(&nextMonthDate)
		- hdate_get_julian(&currentMonth);

	return days;
}
