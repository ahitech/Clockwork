/*
 * Copyright 2026, Alexey Burshtein <ahitech@gmail.com>
 */


#include "ConverterModule.h"
#include <GridLayout.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ConverterModule"

//	-----==< NumbersTextControl >==-----

NumbersTextControl::NumbersTextControl( uint allowedDigits,
						const char* name,
						const char* label,
						const char* data,
						BMessage* message = nullptr) :
		BTextControl(name, label, data, message),
		fAllowedDigits(allowedDigits)
{

};


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

	fHYear = new BTextControl("Hebrew year", B_TRANSLATE("Year"), "", nullptr);
	fGYear = new BTextControl("Gregorian year", B_TRANSLATE("Year"), "", nullptr);

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
	
	mainLayout->AddView(fToGregorian, 0, 0, 1, 2);
	mainLayout->AddView(fGregorianStringView, 1, 0);
	mainLayout->AddView(fHebrewStringView, 1, 1);
	mainLayout->AddView(fToHebrew, 2, 0, 1, 2);
	
	fContentView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    fContentView->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	fContentView->InvalidateLayout();
	
	this->AddChild(fContentView);
	InitializeDatesToToday();
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
		default:
			BBox::MessageReceived(in);
	}
}

void ConverterModuleView::InitializeDatesToToday()
{
	fGregorianDate = GregorianDate(std::time(nullptr));
//	fGregorianDateStringView->SetText(fGregorianDate.ToString().String());
	fHebrewDate = fJewishCalendar.ToHebrewDate(fGregorianDate);
//	fHebrewDateStringView->SetText(fJewishCalendar.HebrewDateToString(fHebrewDate).String());
	this->Invalidate();
}
