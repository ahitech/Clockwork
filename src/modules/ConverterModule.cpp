/*
 * Copyright 2026, Alexey Burshtein <ahitech@gmail.com>
 */


#include "ConverterModule.h"
#include <GridLayout.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ConverterModule"

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
	
	fGregorianDateStringView = new BStringView("Gregorian date view",
		"Gregorian Date View");
	fHebrewDateStringView = new BStringView("Hebrew date view",
		"Hebrew Date View");
	fToGregorian = new BButton("Convert to Gregorian", "↑",
		new BMessage(CONVERT_TO_GREGORIAN));
	fToHebrew = new BButton("Convert to Hebrew", "↓",
		new BMessage(CONVERT_TO_HEBREW));
		
	BGridLayout* mainLayout = new BGridLayout();
	mainLayout->SetInsets(8, 10, 10, 10);
	
	mainLayout->AddView(fToGregorian, 0, 0, 0, 1);
	mainLayout->AddView(fGregorianDateStringView, 1, 0);
	mainLayout->AddView(fHebrewDateStringView, 1, 1);
	mainLayout->AddView(fToHebrew, 2, 0, 2, 1);
	
	BRect content = Bounds();
	content.InsetBy(6, 6);
	content.top += 10; // Leave room for the BBox label.

	BView* contentView = new BView(content, "converter content",
		B_FOLLOW_ALL, B_WILL_DRAW);
	
	printf("Content view created!\n");
	
	contentView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    contentView->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	contentView->SetLayout(mainLayout);
	this->AddChild(contentView);	
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
}
