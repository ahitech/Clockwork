#include "TodayModule.h"

#include <Catalog.h>
#include <MenuItem.h>
#include <Message.h>
#include <Font.h>
#include <LayoutItem.h>
#include <Locale.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <LayoutBuilder.h>
#include <View.h>
#include <StringView.h>
#include <ctime>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <unordered_map>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TodayModuleView"


TodayModuleView::TodayModuleView(BRect frame) :
	BBox(frame,
		 B_TRANSLATE("Today"),
		 B_FOLLOW_H_CENTER | B_FOLLOW_V_CENTER,
		 B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS | B_PULSE_NEEDED ),
	contentView(nullptr)
{
	Init();
	AddDragger();
}

TodayModuleView::TodayModuleView(BMessage* in) :
	BBox(in)
{
	SetFlags(Flags() | B_PULSE_NEEDED);
	Init();
	contentView->SetViewColor(B_TRANSPARENT_COLOR);
	contentView->SetLowColor(B_TRANSPARENT_COLOR);
	contentView->SetHighColor(B_TRANSPARENT_COLOR);
	fTodayStringView->SetViewColor(B_TRANSPARENT_COLOR);
	fTodayStringView->SetLowColor(B_TRANSPARENT_COLOR);
}

void TodayModuleView::AddDragger() {
	BRect rect(this->Bounds());
	rect.left = rect.right - 7;
	rect.top = rect.bottom - 7;
	BDragger* dragger = new BDragger (rect, this,
			B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM,
			B_WILL_DRAW);
	this->AddChild(dragger);
}

void TodayModuleView::Init() {
	fToday = GregorianDate(std::time(nullptr));
	this->SetLabel(B_TRANSLATE("Today"));
	
	BRect content = Bounds();
	content.InsetBy(6, 6);
	content.top += 10; // Leave room for the BBox label.

	contentView = new BView(content, "today content",
		B_FOLLOW_ALL, B_WILL_DRAW);
	
	
	contentView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    contentView->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    this->AddChild(contentView);

	BGroupLayout* mainLayout = new BGroupLayout(B_HORIZONTAL, 8);
	mainLayout->SetInsets(8, 10, 10, 10);
	contentView->SetLayout(mainLayout);
	
	fTodayStringView = new BStringView("Today", "Today");
	fTodayStringView->SetViewColor(B_TRANSPARENT_COLOR);
	fTodayStringView->SetLowColor(B_TRANSPARENT_COLOR);
	BLayoutItem* layoutItem = mainLayout->AddView(fTodayStringView);
	layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP));
	
	UpdateContent();
}

void TodayModuleView::AttachedToWindow()
{
	BView* parent = Parent();
	if ( Parent() )
      SetViewColor(Parent()->ViewColor());
	BBox::AttachedToWindow();
	if (Window() != NULL) {
		Window()->SetPulseRate(60 * 1000000LL);	// Once a minute
	}
}

void TodayModuleView::Pulse()
{
	GregorianDate today(std::time(nullptr));

	if (today.year != fToday.year
			|| today.month != fToday.month
			|| today.day != fToday.day)
	{
		fToday = today;
		UpdateContent();
		Invalidate();
	}
}

void TodayModuleView::UpdateContent() {
	char buffer[100];
	HebrewDate Hdate =
			fJewishCalendar.ToHebrewDate(fToday);
	sprintf(buffer, B_TRANSLATE("%d of %s, %d"),
			Hdate.Day(), JewishMonths[Hdate.Month()].name.String(),
			Hdate.Year());
	fTodayStringView->SetText(buffer);
	Invalidate();
}

status_t TodayModuleView::Archive(BMessage* into, bool deep = true) const
{
	status_t toReturn = BBox::Archive(into, deep);	
	if (toReturn != B_OK) {
		fprintf(stderr, "BBox::Archive -> %s\n", strerror(toReturn));
		return toReturn;
	}
	into->AddString("add_on", "application/x-vnd.clockwork-app");
	into->AddString("class", "TodayModuleView");
	return B_OK;	
}

BArchivable* TodayModuleView::Instantiate (BMessage* in) {
	if (validate_instantiation(in, "TodayModuleView")) {
		return new TodayModuleView(in);
	}
	return NULL;
}
