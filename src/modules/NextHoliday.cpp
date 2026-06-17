#include "NextHoliday.h"

#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
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
	
	SetFlags(Flags() | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE
		| B_PULSE_NEEDED);

	fToday = GregorianDate(std::time(nullptr));
	fSelectedOffset = 0;
	BRect rect(0, 0, 25, 40);
	
	fPrevHolidayButton = new BButton(rect,
		"Prev Holiday",
		"<",
		new BMessage(PrevHolidayMessage));
	fPrevHolidayButton->SetTarget(this);
		
	fNextHolidayButton = new BButton(rect,
		"Next Holiday",
		">",
		new BMessage(NextHolidayMessage));
	fNextHolidayButton->SetTarget(this);
	
	fFirstLine = new BStringView(rect,
		"First Line",
		"First Line");
	fSecondLine = new BStringView(rect,
		"Second Line",
		"Second Line");
	fThirdLine = new BStringView(rect,
		"Third Line",
		"Third Line");
	
	BGroupLayout *boxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
		.SetInsets(10, 10, 10, 10)
		.Add(fPrevHolidayButton)
		.Add(BGroupLayoutBuilder(B_VERTICAL, 10)
			.Add(fFirstLine)
			.Add(fSecondLine)
			.Add(fThirdLine)
		)
		.Add(fNextHolidayButton)
	;

	this->AddChild(boxLayout->View());

	UpdateCurrentHoliday();
}
	
	
//	BGroupLayout* layout = new BLayoutBuilder::Group<>(B_VERTICAL)


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

void NextHolidayModule::Pulse()
{
	GregorianDate today(std::time(nullptr));

	if (today.year != fToday.year
			|| today.month != fToday.month
			|| today.day != fToday.day) {
		fToday = today;
		fSelectedOffset = 0;
		UpdateCurrentHoliday();
		Invalidate();
	}
}
