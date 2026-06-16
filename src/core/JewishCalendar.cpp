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

HebrewDate JewishCalendar::HebrewDateFromGregorian(const GregorianDate& in)
{
	hdate_struct hs;
	hdate_set_gdate (&hs, in.day, in.month, in.year);
	HebrewDate toReturn(hs.hd_year, hs.hd_mon, hs.hd_day);
	return toReturn;
}
	
	
	
}
