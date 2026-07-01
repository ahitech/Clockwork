/*
 * Copyright 2026, Alexey Burshtein <ahitech@gmail.com>
 */


#include "ConverterModule.h"
#include <GridLayout.h>
#include <MenuItem.h>
#include <stdio.h>
#include <string.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ConverterModule"

//	-----==< NumbersTextControl >==-----

NumbersTextControl::NumbersTextControl( uint allowedDigits,
						const char* name,
						const char* label,
						const char* data,
						BMessage* message = nullptr) :
		BTextControl(name, label, data, message),
		fAllowedDigits(allowedDigits),
		fSanitizing(false),
		fToSendUponChange(nullptr)
{
	buffer[0] = '\0';
};

NumbersTextControl::NumbersTextControl(BMessage* in)
	:	BTextControl(in),
		fSanitizing(false),
		fToSendUponChange(nullptr)
{
	if (in->FindInt32("Allowed Digits", (int32*)(&fAllowedDigits)) != B_OK)
		fAllowedDigits = 4;
	if (in->FindString("String to replace", &buffer) != B_OK)
		buffer[0] = '\0';
}


status_t NumbersTextControl::Archive(BMessage* out, bool deep = true)
{
	status_t toReturn = BTextControl::Archive(out, deep);
	if (toReturn != B_OK) {
		return toReturn;
	}
	toReturn = out->AddInt32("Allowed Digits", static_cast<int32>(fAllowedDigits));
	if (toReturn != B_OK) {
		return toReturn;
	}
	toReturn = out->AddString("String to replace", buffer);
	return toReturn;
}

void NumbersTextControl::SetStringToReplace(const char* string)
{
	strncpy(buffer, string, 50);
}

void NumbersTextControl::MessageReceived(BMessage* in) {
	switch (in->what)
	{
		case (TEXT_CHANGED):
		{
			SanitizeText();
			if (fToSendUponChange && strlen(buffer) > 0) {
				int num = std::stoi(Text());
				fToSendUponChange->ReplaceInt32(buffer, num);
				fTarget->MessageReceived(fToSendUponChange);
			}
			break;
		}
		default:
			BTextControl::MessageReceived(in);
			break;
	}
}

void NumbersTextControl::KeyDown(const char* bytes, int32 numBytes)
{
	if (numBytes >= 2) { return; }	// All Unicode goes to hell
	char in = *bytes;
	if (in >= '0' && in <= '9') {
		BString newText(this->Text());
		int32 length = newText.CountChars();
		if (length >= fAllowedDigits)
		{
			// Remove the most significant character from the string
			newText.Remove(0, (length - fAllowedDigits) - 1);
		}
		newText.Append(in, 1);
		this->SetText(newText.String());
		return;
	}
	if (in == ' ' 	// Clear the whole string on SPACE or ESC
		|| in == 27)
	{
		this->SetText("");
		return;
	} 
	if (in < ' ' // Characters from 0 to 31, service chars
		|| in == 127)	// Delete
	{
		BTextControl::KeyDown(&in, 1);
		return;
	}
	return;
}

void NumbersTextControl::AttachedToWindow()
{
	BView* parent = Parent();
	if ( Parent() )
      SetViewColor(Parent()->ViewColor());
    BTextControl::AttachedToWindow();
    SetModificationMessage(new BMessage(TEXT_CHANGED));
	SetTarget(this);
}

void NumbersTextControl::SanitizeText()
{
	if (fSanitizing)
		return;		// Only one sanitization at a time

	fSanitizing = true;

	BString source(Text());
	BString clean;

	for (int32 i = 0; i < source.CountChars(); i++) {
		BString ch;
		source.CopyCharsInto(ch, i, 1);

		if (ch.Length() == 1 && ch[0] >= '0' && ch[0] <= '9')
			clean << ch;
	}

	while ((uint)clean.CountChars() > fAllowedDigits)
		clean.Remove(0, 1);

	if (clean != Text())
		SetText(clean.String());

	fSanitizing = false;
}

//	-----==< ConverterModuleView >==-----

ConverterModuleView::ConverterModuleView(BRect frame) :
	BBox(frame,
		 B_TRANSLATE("Converter"),
		 B_FOLLOW_H_CENTER | B_FOLLOW_V_CENTER,
		 B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS | B_PULSE_NEEDED )
{
	Init();
	AddDragger();
}

void ConverterModuleView::Init()
{
	SetFlags(Flags() | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE
		| B_PULSE_NEEDED);
	SetLabel(B_TRANSLATE("Converter"));

	fGregorianDate = GregorianDate(std::time(nullptr));
	fHebrewDate = fJewishCalendar.ToHebrewDate(fGregorianDate);
	
	fGregorianStringView = new BStringView("Gregorian",
		B_TRANSLATE("Gregorian:"));
	fHebrewStringView = new BStringView("Hebrew",
		B_TRANSLATE("Hebrew:"));

	fToGregorian = new BButton("Convert to Gregorian", "↑",
		new BMessage(CONVERT_TO_GREGORIAN));
	fToHebrew = new BButton("Convert to Hebrew", "↓",
		new BMessage(CONVERT_TO_HEBREW));

	BRect content = Bounds();
	content.InsetBy(6, 6);
	content.top += 10; // Leave room for the BBox label.

	fContentView = new BView(content, "converter content",
		B_FOLLOW_ALL, B_WILL_DRAW | B_SUPPORTS_LAYOUT);
	
	BGridLayout* mainLayout = new BGridLayout();
	mainLayout->SetInsets(8, 10, 10, 10);
	fContentView->SetLayout(mainLayout);
	
	fGDays = new BPopUpMenu("Gregorian days");
	fHDays = new BPopUpMenu("Hebrew days");
	fGMonths = new BPopUpMenu("Gregorian months");
	fHMonths = new BPopUpMenu("Hebrew months");
	
	InitializeDatesToToday();
	InitializeDateFields();
	
	mainLayout->AddView(fToGregorian, 0, 0, 1, 2);
	mainLayout->AddView(fGregorianStringView, 1, 0);
	mainLayout->AddView(fGYear, 2, 0);
	mainLayout->AddView(fHYear, 2, 1);
	mainLayout->AddView(fGMonth, 3, 0);
	mainLayout->AddView(fHMonth, 3, 1);
	mainLayout->AddView(fGDay, 4, 0);
	mainLayout->AddView(fHDay, 4, 1);
	mainLayout->AddView(fHebrewStringView, 1, 1);
	mainLayout->AddView(fToHebrew, 5, 0, 1, 2);
	
	fContentView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    fContentView->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	fContentView->InvalidateLayout();
	
	this->AddChild(fContentView);
}

void ConverterModuleView::AddDragger() {
	BRect rect(this->Bounds());
	rect.left = rect.right - 7;
	rect.top = rect.bottom - 7;
	BDragger* dragger = new BDragger (rect, this,
			B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM,
			B_WILL_DRAW);
	this->AddChild(dragger);
}

void ConverterModuleView::AttachedToWindow()
{
	BView* parent = Parent();
	if ( Parent() )
      SetViewColor(Parent()->ViewColor());
	BBox::AttachedToWindow();
	if (Window() != NULL) {
		Window()->SetPulseRate(60 * 1000000LL);	// Once a minute
	}
	fToGregorian->SetTarget(this);
	fToHebrew->SetTarget(this);
}

void ConverterModuleView::MessageReceived(BMessage* in) {
	switch (in->what) {
		case CONVERT_TO_GREGORIAN:
			printf("Going to convert to Gregorian!\n");
			break;
		case CONVERT_TO_HEBREW:
			printf("Going to convert to Hebrew!\n");
			break;
		case GREGORIAN_YEAR_CHG:
		{
			int32 newYear;
			in->FindInt32("year", &newYear);
			if (newYear / 1000.0f < 1)	// Only 3 or less digits in year
			{
				char buffer[10];
				newYear += 1000;
				sprintf(buffer, "%04d", newYear);
				fGYear.SetText(buffer);
			}
			fGregorianDate.year = newYear;
			BuildGregorianMonthsMenu();
			BuildGregorianDaysMenu();
			break;
		}
		case HEBREW_YEAR_CHG:
		{
			int32 newYear;
			in->FindInt32("year", &newYear);
			while (newYear / 1000.0f < 5)	// Years long in the past
			{
				newYear += 1000;
			}
			char buffer[10];
			sprintf(buffer, "%04d", newYear);
			fHYear.SetText(buffer);
			
			fHebrewDate.SetYear(newYear);
			BuildHebrewMonthsMenu(newYear);
			break;
		}
		case GREGORIAN_MONTH_CHG:
			BuildGregorianDaysMenus(uint gregorianYear, uint gregorianMonth);
		default:
			BBox::MessageReceived(in);
	}
}

void ConverterModuleView::InitializeDatesToToday()
{
	fGregorianDate = GregorianDate(std::time(nullptr));
	fHebrewDate = fJewishCalendar.ToHebrewDate(fGregorianDate);
	InitializeDateFields();
	this->Invalidate();
}

void ConverterModuleView::InitializeDateFields()
{
	char buffer[50];
	BMessage* toSend;
	sprintf(buffer, "%4d", fHebrewDate.Year());
	fHYear = new NumbersTextControl(4, "Hebrew year", B_TRANSLATE("Year"), buffer);
	toSend = new BMessage(HEBREW_YEAR_CHG);
	toSend->AddInt32("Year", fHebrewDate.Year());
	fHYear->SetMessage(toSend);
	fHYear->SetTarget(this);
	sprintf(buffer, "%4d", fGregorianDate.year);
	fGYear = new NumbersTextControl(4, "Gregorian year", B_TRANSLATE("Year"), buffer);
	toSend = new BMessage(GREGORIAN_YEAR_CHG);
	toSend->AddInt32("Year", fGregorianDate.year);
	fHYear->SetMessage(toSend);
	fHYear->SetTarget(this);
	BuildMonthsMenus(static_cast<uint>(fHebrewDate.Year()));
	fHMonth = new BMenuField("Hebrew month", B_TRANSLATE("Month"), fHMonths);
	fGMonth = new BMenuField("Gregorian month", B_TRANSLATE("Month"), fGMonths);
	fHMonth->Menu()->FindItem(JewishMonths[fHebrewDate.Month()].name.String())->SetMarked(true);
	fGMonth->Menu()->FindItem(GregorianMonthNames[fGregorianDate.month].String())->SetMarked(true);
	BuildHebrewDaysMenus(static_cast<uint>(fHebrewDate.Year()),
						 static_cast<uint>(fHebrewDate.Month()));
	BuildGregorianDaysMenus(static_cast<uint>(fGregorianDate.year),
							static_cast<uint>(fGregorianDate.month));
	fHDay = new BMenuField("Hebrew day", B_TRANSLATE("Day"), fHDays);
	fGDay = new BMenuField("Gregorian day", B_TRANSLATE("Day"), fGDays);
	sprintf(buffer, "%d", fGregorianDate.day);
	fGDays->FindItem(buffer)->SetMarked(true);
	sprintf(buffer, "%d", fHebrewDate.Day());
	fHDays->FindItem(buffer)->SetMarked(true);
}

void ConverterModuleView::BuildHebrewMonthsMenu(uint hebrewYear)
{
	BMenuItem* monthItem;
	ClearMenu(fHMonths); ClearMenu(fGMonths);

	// Hebrew months menu
	for (int i = 1; i <= 14; i++) {
		if (fJewishCalendar.IsValidHebrewMonth(hebrewYear, i)) {
			monthItem = new BMenuItem(JewishMonths[i].name.String(), nullptr);
			fHMonths->AddItem(monthItem);
		}
	}
}

/**
 *	\brief		Finds the number corresponding to selected item in a menu
 *	\param[in]	menu	The menu to search for the item
 *	\param[in]	string	The string according to which searching
 *	\returns	The number associated with the selected item
 *				-1 in case of any error
 */
int ConverterModuleView::FindCurrentlySelectedItem(BPopUpMenu* menu,
			const char* string)
{
	int toReturn = -1;	// No item selected
	BMenuItem* menuItem = menu->FindMarked();
	if (nullptr == menuItem) { return toReturn; }
	BMessage* message = menuItem->Message();
	if (B_OK != message->FindInt8(string, static_cast<int8*>(&toReturn))) { return -1; }
	return toReturn;	
}

void ConverterModuleView::BuildGregorianMonthsMenu() {
	BMenuItem* monthItem;
	BMessage* message;
	// Save currently selected item
	int selectedItem = FindCurrentlySelectedItem(fGMonths, "Month");

	// Basically, here I would clear the menu and recreate it,
	// but since in gregorian calendar all years have the same months,
	// I'm going to reuse current menu.
	//	ClearMenu(fGMonths);
	
	// Gregorian menu
	for (int i=1; i<=12; i++) {
		message = new BMessage(GREGORIAN_MONTH_CHG);
		message->AddInt8("Month", i);
		monthItem = new BMenuItem(GregorianMonthNames[i].String(), message);
		fGMonths->AddItem(monthItem);
	}
}

void ConverterModuleView::BuildHebrewDaysMenus(uint hebrewYear, uint hebrewMonth)
{	
	BMenuItem* dayItem;
	ClearMenu(fHDays);
	int limit = fJewishCalendar.DaysInHebrewMonth(hebrewYear, hebrewMonth);
	char buffer[10];
	for (int i = 1; i <= limit; i++) {
		sprintf (buffer, "%2d", i);
		dayItem = new BMenuItem(buffer, nullptr);
		fHDays->AddItem(dayItem);
	}
}

void ConverterModuleView::BuildGregorianDaysMenus(uint year, uint month)
{
	BMenuItem* dayItem;
	ClearMenu(fGDays);
	int limit = fJewishCalendar.DaysInGregorianMonth(year, month);
	char buffer[10];
	for (int i = 1; i <= limit; i++) {
		sprintf (buffer, "%2d", i);
		dayItem = new BMenuItem(buffer, nullptr);
		fGDays->AddItem(dayItem);
	}
}

void ConverterModuleView::ClearMenu(BMenu* in)
{
	if (in == nullptr) return;
	BMenuItem* item = static_cast<BMenuItem*>(0x1000);
	while (item != nullptr) {
		item = in->RemoveItem(0);
		free(item);
	}
}
