/*
 * Copyright 2026, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the GPL license.
 */


#include "JewishCalendar.h"

#include <Catalog.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "JewishCalendar"

std::map<int, std::pair<enum HolidayType, BString>> holidayNames = {
	{0,		{NO_HOLIDAY, 		BString(B_TRANSLATE("No holiday"))}},
	{1, 	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Rosh HaShana first day"))}},
	{2, 	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Rosh HaShana second day"))}},
	{3, 	{TZOM, 				BString(B_TRANSLATE("Tzom Gedaliah"))}},
	{4, 	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Yom Kippur"))}},
	{5,		{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Sukkot"))}},
	{6, 	{HOL_HAMOED, 		BString(B_TRANSLATE("Hol HaMoed Sukkot"))}},
	{7, 	{RELIGIOUS_MINOR, 	BString(B_TRANSLATE("Hoshana Raba"))}},
	{8, 	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Simchat Torah"))}},
	{9, 	{RELIGIOUS_MINOR, 	BString(B_TRANSLATE("Hanukkah"))}},
	{10,	{TZOM, 				BString(B_TRANSLATE("Asara BeTevet"))}},
	{11,	{RELIGIOUS_MINOR, 	BString(B_TRANSLATE("Tu BiShvat"))}},
	{12,	{TZOM, 				BString(B_TRANSLATE("Taanit Ester"))}},
	{13,	{RELIGIOUS_MINOR, 	BString(B_TRANSLATE("Purim"))}},
	{14,	{RELIGIOUS_MINOR, 	BString(B_TRANSLATE("Shushan Purim"))}},
	{15,	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Pesach"))}},
	{16,	{HOL_HAMOED,		BString(B_TRANSLATE("Hol HaMoed Pesach"))}},
	{17,	{CIVIL,				BString(B_TRANSLATE("Yom HaAtzmaut"))}},
	{18,	{RELIGIOUS_MINOR,	BString(B_TRANSLATE("Lag BaOmer"))}},
	{19,	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Erev Shavuot"))}},
	{20,	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Shavuot"))}},
	{21,	{TZOM,				BString(B_TRANSLATE("Tzom Tammuz"))}},
	{22,	{TZOM,				BString(B_TRANSLATE("Tish'a BeAv"))}},
	{23,	{RELIGIOUS_MINOR,	BString(B_TRANSLATE("Tu BeAv"))}},
	{24,	{MINOR,				BString(B_TRANSLATE("Yom HaShoah"))}},
	{25,	{MINOR,				BString(B_TRANSLATE("Yom HaZikaron"))}},
	{26,	{CIVIL,				BString(B_TRANSLATE("Yom Yerushalayim"))}},
	{27,	{RELIGIOUS_MINOR,	BString(B_TRANSLATE("Shmini Atzeret"))}},
	{28,	{RELIGIOUS_MINOR,	BString(B_TRANSLATE("Shevi'i shel Pesach"))}},
	{29,	{RELIGIOUS_MINOR,	BString(B_TRANSLATE("Acharon shel Pesach"))}},
	{30,	{RELIGIOUS_MINOR,	BString(B_TRANSLATE("Shavuot second day"))}},
	{31,	{RELIGIOUS_MINOR,	BString(B_TRANSLATE("Sukkot second day"))}},
	{32,	{RELIGIOUS_MINOR,	BString(B_TRANSLATE("Pesach second day"))}},
	{33,	{CIVIL,				BString(B_TRANSLATE("Family Day"))}},
	{34,	{MINOR,				BString(B_TRANSLATE("Memorial day for fallen whose place of burial is unknown"))}},
	{35,	{MINOR,				BString(B_TRANSLATE("Yitzhak Rabin memorial day"))}},
	{36,	{MINOR,				BString(B_TRANSLATE("Zeev Zhabotinsky day"))}}, 
	{37,	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Erev Yom Kippur"))}},
	{38,	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Erev Pesach"))}},
	{39,	{RELIGIOUS_MAJOR, 	BString(B_TRANSLATE("Erev Sukkot"))}}
};


JewishMonth	JewishMonths[15] = {
	{0, BString(B_TRANSLATE("No month"))},
	{1, BString(B_TRANSLATE("Tishrei"))},
	{2, BString(B_TRANSLATE("Cheshvan"))},
	{3, BString(B_TRANSLATE("Kislev"))},
	{4, BString(B_TRANSLATE("Tevet"))},
	{5, BString(B_TRANSLATE("Shvat"))},
	{6, BString(B_TRANSLATE("Adar"))},
	{7, BString(B_TRANSLATE("Nisan"))},
	{8, BString(B_TRANSLATE("Iyar"))},
	{9,	BString(B_TRANSLATE("Sivan"))},
	{10,BString(B_TRANSLATE("Tammuz"))},
	{11,BString(B_TRANSLATE("Av"))},
	{12,BString(B_TRANSLATE("Elul"))},
	{13,BString(B_TRANSLATE("Adar I"))},
	{14,BString(B_TRANSLATE("Adar II"))}
};

BString GregorianMonthNames[13] = 
	{	BString(B_TRANSLATE("No month"))
		,BString(B_TRANSLATE("January"))
		,BString(B_TRANSLATE("February"))
		,BString(B_TRANSLATE("March"))
		,BString(B_TRANSLATE("April"))
		,BString(B_TRANSLATE("May"))
		,BString(B_TRANSLATE("June"))
		,BString(B_TRANSLATE("July"))
		,BString(B_TRANSLATE("August"))
		,BString(B_TRANSLATE("September"))
		,BString(B_TRANSLATE("October"))
		,BString(B_TRANSLATE("November"))
		,BString(B_TRANSLATE("December"))
	};

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

BString		JewishCalendar::HebrewDateToString (const HebrewDate& in) const
{
	// I suppose the Hebrew Date struct is already normalized
	BString toReturn;
	char buffer[50];
	sprintf(buffer, "%d of %s, %d",
		in.Day(), JewishMonths[in.Month()].name.String(), in.Year());
	toReturn.SetTo(buffer);
	
	return toReturn;
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
	GregorianDate toReturn;
	toReturn.year = hdate_get_gyear (&hs);
	toReturn.month = hdate_get_gmonth(&hs);
	toReturn.day = hdate_get_gday(&hs);
	fflush(stderr);
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


/**
 *	\brief		Returns ID of the day as defined in the libhdate.
 *	\param[in]	date	Gregorian date for the translation.
 *	\returns	0 if the date is not holiday, 1 .. 37 otherwise.
 */
int JewishCalendar::HolidayId(const GregorianDate& date) const
{
	hdate_struct tempHDate;
	hdate_set_gdate(&tempHDate,
					date.day,
					date.month,
					date.year);
	int holiday = hdate_get_holyday (&tempHDate, _diaspora);
	return holiday;
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
