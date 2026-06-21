/*
 * Copyright 2026, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL license.
 */
#ifndef __JEWISH_CALENDAR_H__
#define __JEWISH_CALENDAR_H__

#include "hdate.h"
#include "HebrewDate.h"

#include <ctime>
#include <map>
#include <utility>
#include <String.h>
#include <Catalog.h>
#include <SupportDefs.h>


/**
 *	\brief		Struct that represents Gregorian date
 *	\details	Under the hood the normalization is performed
 *				using struct tm, mktime and localtime_r.
 *	\note		HebrewDate struct is defined in HebrewDate.h
 */
struct GregorianDate {
	uint year;	// Full year, e. g. 2026
	uint month;	// 1 = January, 12 = December
	uint day;	// 1 .. 31

	GregorianDate() = default;

	GregorianDate(uint y, uint m, uint d)
		: year(y), month(m), day(d)
	{ }

	explicit GregorianDate(const struct tm& t)
		: year(t.tm_year + 1900),
		  month(t.tm_mon + 1),
		  day(t.tm_mday)
	{ }
	
	explicit GregorianDate(std::time_t t)
	{
		struct tm result;
		localtime_r(&t, &result);
	
		year  = result.tm_year + 1900;
		month = result.tm_mon + 1;
		day   = result.tm_mday;
	}
};

	GregorianDate GregorianDateFromTm(const struct tm&);
	struct tm TMFromGregorianDate(const GregorianDate& in);
	GregorianDate AddDays(const GregorianDate& in, int days);
	int DaysBetween(const GregorianDate& in1, const GregorianDate &in2);

/**
 *	\brief
 */
enum HolidayType {
	 NO_HOLIDAY = 0
	,TZOM
	,RELIGIOUS_MAJOR
	,RELIGIOUS_MINOR
	,HOL_HAMOED
	,CIVIL
	,MINOR	
};
 
extern std::map<int, std::pair<enum HolidayType, BString>> holidayNames;

struct JewishMonth {
	uint number;
	BString name;
};

extern JewishMonth JewishMonths[14];


class JewishCalendar {
public:
	JewishCalendar (bool diaspora = true);

	HebrewDate Today() const;
	HebrewDate ToHebrewDate(const GregorianDate& date) const;
	BString		HebrewDateToString(const HebrewDate& ) const;
	GregorianDate	ToGregorianDate(const HebrewDate& date) const;
	GregorianDate 	AddDays(const GregorianDate& in,
										  int days) const;
	int HolidayId(const GregorianDate& date) const;		
	int HolidayType(const GregorianDate& date) const;
	int DaysInHebrewMonth(uint year, uint month) const;

private:
	bool _diaspora;
};


#endif // __JEWISH_CALENDAR_H__
