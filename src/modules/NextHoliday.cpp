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
#include "../../lib/hdate.h"

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

void NextHolidayModule::Init()
{
	SetViewColor(B_TRANSPARENT_COLOR);
	SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLabel(B_TRANSLATE("Next Hebrew Holiday"));

	SetFlags(Flags() | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE
		| B_PULSE_NEEDED);

	fToday = GregorianDate(std::time(nullptr));
	fSelectedOffset = 0;

	fPrevHolidayButton = new BButton("Prev Holiday", "<",
		new BMessage(PrevHolidayMessage));
	fPrevHolidayButton->SetTarget(this);

	fNextHolidayButton = new BButton("Next Holiday", ">",
		new BMessage(NextHolidayMessage));
	fNextHolidayButton->SetTarget(this);

	fFirstLine = new BStringView("First Line", "First Line");
	fSecondLine = new BStringView("Second Line", "Second Line");
	fThirdLine = new BStringView("Third Line", "Third Line");

	BRect content = Bounds();
	content.InsetBy(6, 6);
	content.top += 10; // Leave room for the BBox label.

	BView* contentView = new BView(content, "holiday content",
		B_FOLLOW_ALL, B_WILL_DRAW);
	
	contentView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    contentView->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
//    SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));

//	contentView->SetViewColor(255, 0, 0); // Temporary debug color.
//	contentView->SetLowColor(255, 0, 0);

	BGroupLayout* mainLayout = new BGroupLayout(B_HORIZONTAL, 8);
	mainLayout->SetInsets(8, 10, 10, 10);
	contentView->SetLayout(mainLayout);
	
	mainLayout->AddView(fPrevHolidayButton);
	
	BView* textView = new BView("holiday text", B_WILL_DRAW);
//	textView->SetViewColor(255, 255, 0); // Temporary debug color.
//	textView->SetLowColor(255, 255, 0);
	
	BGroupLayout* textLayout = new BGroupLayout(B_VERTICAL, 4);
	textLayout->SetInsets(0, 0, 0, 0);
	textView->SetLayout(textLayout);
	
	textLayout->AddView(fFirstLine);
	textLayout->AddView(fSecondLine);
	textLayout->AddView(fThirdLine);
	
	mainLayout->AddView(textView, 1.0f);
	
	mainLayout->AddView(fNextHolidayButton);

	AddChild(contentView);
	
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

void NextHolidayModule::UpdateCurrentHoliday()
{
	int holidayId = FindNextHolidayId(fToday);

	if (holidayId == 0) {
		fFirstLine->SetText(B_TRANSLATE("No holiday found"));
		return;
	}

//	fFirstLine->SetText(HolidayName(holidayId));

}

int NextHolidayModule::FindNextHolidayId(const GregorianDate& from) const
{
	fNextHolidayDate = from;
	std::time_t now = std::time(nullptr);
	struct tm tempTM;
	localtime_r(&now, &tempTM);
	time_t	timeSinceEpoch;
	int holiday = 0;
	hdate_struct tempHDate;
	
	do
	{
		tempTM.tm_mday++;
		//counter++;
		timeSinceEpoch = mktime(&tempTM);
		hdate_set_gdate(&tempHDate,
						tempTM.tm_mday,
						tempTM.tm_mon + 1,
						tempTM.tm_year + 1900);
		holiday = hdate_get_holyday (&tempHDate, 1);		// Diaspora
		
		printf("Holiday = %d\n", holiday);
		
	} while (holiday == 0);
		
		
		fFirstLine->SetText(holidayNames[holiday].second);
		char buffer[30];
		sprintf(buffer, "On %d.%d.%d", tempTM.tm_mday, tempTM.tm_mon+1,
			tempTM.tm_year+1900);
		fSecondLine->SetText(buffer);
		
	
	return holiday;
}

const char* NextHolidayModule::HolidayName(int holidayId) const
{
	for (int i = 0; i < 400; i++) {
		
		printf(hdate_string (HDATE_STRING_HOLIDAY, 
									i,
									0,
									HDATE_STRING_LOCAL));
		printf(" - ");
		printf(hdate_string (HDATE_STRING_PARASHA, 
									i,
									1,
									HDATE_STRING_LOCAL));
		printf("\n");
		fflush(stdout);
	}
	
	return B_TRANSLATE(hdate_string (HDATE_STRING_HOLIDAY, 
									holidayId,
									0,
									HDATE_STRING_LOCAL));
		
//	switch (holidayId) {
//		case 1:
//			return B_TRANSLATE("Yom Tov");
//
//		case 2:
//			return B_TRANSLATE("Yom Kippur Eve");
//
//		case 3:
//			return B_TRANSLATE("Hol ha-Moed");
//
//		case 4:
//			return B_TRANSLATE("Hanukkah / Purim");
//
//		case 5:
//			return B_TRANSLATE("Tzom");
//
//		case 6:
//			return B_TRANSLATE("Independence day");
//
//		case 7:
//			return B_TRANSLATE("Tu be-av");
//
//		case 8:
//			return B_TRANSLATE("Holocost memorial day");
//
//		case 9:
//			return B_TRANSLATE("National day");
//
//		default:
//			{
//				char buffer[20];
//				sprintf(buffer, "%d Unknown holiday", holidayId);
//				return B_TRANSLATE(buffer);
//			}
//	}
}

