#include "NextHoliday.h"

#include <Catalog.h>
#include <MenuItem.h>
#include <Message.h>
#include <Font.h>
#include <Locale.h>
#include <View.h>
#include <StringView.h>
#include <ctime>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <unordered_map>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "NextHolidayModule"

NextHolidayModule::NextHolidayModule(BRect frame) :
	BBox(frame,
		 "Next Holiday",
		 B_FOLLOW_H_CENTER | B_FOLLOW_V_CENTER,
		 B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS | B_PULSE_NEEDED )
{
	Init();
	AddDragger();
}

NextHolidayModule::NextHolidayModule(BMessage* in) :
	BBox(in)
{
	SetFlags(Flags() | B_PULSE_NEEDED);
	Init();
}

NextHolidayModule::~NextHolidayModule() {
	
}

BArchivable* NextHolidayModule::Instantiate (BMessage* in) {
	if (validate_instantiation(in, "NextHolidayModule")) {
		return new NextHolidayModule(in);
	}
	return NULL;
}


status_t NextHolidayModule::Archive(BMessage* out, bool deep) const
{
	status_t toReturn = BBox::Archive(out, deep);
	if (toReturn != B_OK) { return toReturn; }
	out->AddString("add_on", "application/x-vnd.clockwork-app");
	out->AddString("class", "NextHolidayModule");
	return B_OK;
}

void NextHolidayModule::AddDragger() {
	BRect rect(this->Bounds());
	rect.left = rect.right - 7;
	rect.top = rect.bottom - 7;
	BDragger* dragger = new BDragger (rect, this,
			B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM,
			B_WILL_DRAW);
	this->AddChild(dragger);
}

void NextHolidayModule::Init() {
	SetViewColor(B_TRANSPARENT_COLOR);
	SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	this->SetLabel(B_TRANSLATE("Next Hebrew Holiday"));
	
	
	
	
//	BGroupLayout* layout = new BLayoutBuilder::Group<>(B_VERTICAL)
	
}

void NextHolidayModule::AttachedToWindow()
{
	BView* parent = Parent();
	if ( Parent() )
      SetViewColor(Parent()->ViewColor());
	BBox::AttachedToWindow();
	if (Window() != NULL) {
		Window()->SetPulseRate(60 * 1000000LL);	// Once a minute
	}
}

