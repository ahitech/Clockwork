/*
 * Copyright 2026, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL license.
 */
#ifndef __JEWISH_CALENDAR_H__
#define __JEWISH_CALENDAR_H__

#include "hdate.h"
#include "HebrewDate.h"

#include <ctime>
#include <SupportDefs.h>

/**
 *	\brief		Struct that represents Gregorian date
 *	\details	Under the hood the normalization is performed
 *				using struct tm, mktime and localtime_r.
 */
struct GregorianDate {
	uint year;	// Full year, e. g. 2026
	uint month;	// 1 = January, 12 = December
	uint day;	// 1 .. 31
};

	GregorianDate GregorianDateFromTm(const struct tm&);
	struct tm TMFromGregorianDate(const GregorianDate& in);
	GregorianDate AddDays(const GregorianDate& in, int days);
	int DaysBetween(const GregorianDate& in1, const GregorianDate &in2);

/**
 *	\brief
 */

struct HolidayFilter {
	bool includeMajor,
	bool includeMinor,
	bool includeFasts,
	bool includeModernIsraeli,
	bool includeRoshHodesh,
	bool includeHolHaMoed
};



class JewishCalendar {
public:
	JewishCalendar (bool diaspora = true);
	
	HebrewDate HebrewDateFromGregorian(const GregorianDate& date) const;
	bool IsHoliday(const GregorianDate &gregorianDate,
			const HolidayFilter& filter);
			
	
			

private:
	bool _diaspora;
};


#endif // __JEWISH_CALENDAR_H__
