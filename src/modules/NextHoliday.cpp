#include "NextHoliday.h"

#include <AppDefs.h>
#include <Application.h>
#include <Cursor.h>
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

void ClickableStringView::MouseDown(BPoint where) {
	fprintf(stderr, "Clicked at %.2f, %.2f\n", where.x, where.y);
	fflush(stderr);
	if (Window() != nullptr && target != nullptr) {
		BMessage* toSend = new BMessage(ToggleDateLanguage);
		Window()->PostMessage(toSend, target);
	}
}

void ClickableStringView::MouseMoved(BPoint where, uint32 transit,
	const BMessage* dragMessage)
{
	switch (transit) {
		case B_ENTERED_VIEW:
			be_app->SetCursor(new BCursor(B_CURSOR_ID_HELP));
			break;

		case B_EXITED_VIEW:
			be_app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
			break;

		default:
			break;
	}

	BStringView::MouseMoved(where, transit, dragMessage);
}


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
	fShownGregorianDate = false;		// We start with Gregorian date
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
	fFirstLine->SetAlignment(B_ALIGN_CENTER);
	fSecondLine = new BStringView("Second Line", "Second Line");
	fSecondLine->SetAlignment(B_ALIGN_CENTER);
	fThirdLine = new ClickableStringView("Third Line", "Type: x", this);
	fThirdLine->SetAlignment(B_ALIGN_CENTER);
	fFourthLine = new BStringView("Fourth Line", "In x days");
	fFourthLine->SetAlignment(B_ALIGN_CENTER);
	fUpdateOnMidnight = new BCheckBox("Recalculate at midnight",
			B_TRANSLATE("Recalculate at midnight"),
			NULL);

	BRect content = Bounds();
	content.InsetBy(6, 6);
	content.top += 10; // Leave room for the BBox label.

	BView* contentView = new BView(content, "holiday content",
		B_FOLLOW_ALL, B_WILL_DRAW);
	
	contentView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    contentView->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BGroupLayout* mainLayout = new BGroupLayout(B_HORIZONTAL, 8);
	mainLayout->SetInsets(8, 10, 10, 10);
	contentView->SetLayout(mainLayout);
	
	mainLayout->AddView(fPrevHolidayButton);
	
	BView* textView = new BView("holiday text", B_WILL_DRAW);
	
	BGroupLayout* textLayout = new BGroupLayout(B_VERTICAL, 4);
	textLayout->SetInsets(0, 0, 0, 0);
	textView->SetLayout(textLayout);
	
	textLayout->AddView(fFirstLine);
	textLayout->AddView(fSecondLine);
	textLayout->AddView(fThirdLine);
	textLayout->AddView(fFourthLine);
	textLayout->AddView(fUpdateOnMidnight);
	
	mainLayout->AddView(textView, 1.0f);
	
	mainLayout->AddView(fNextHolidayButton);

	AddChild(contentView);
	
	UpdateCurrentHoliday(Direction::NEXT);
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

void NextHolidayModule::Pulse()
{
	GregorianDate today(std::time(nullptr));

	if (today.year != fToday.year
			|| today.month != fToday.month
			|| today.day != fToday.day) {
		fToday = today;
		fSelectedOffset = 0;
		if (fUpdateOnMidnight->Value()) {
			UpdateCurrentHoliday(Direction::NEXT);
		}
		Invalidate();
	}
}

void NextHolidayModule::UpdateCurrentHoliday(Direction dir)
{
	int holidayId = FindNextHolidayId(fNextHolidayDate, dir);

	if (holidayId == 0) {
		fFirstLine->SetText(B_TRANSLATE("No holiday found"));
		return;
	}
}

int NextHolidayModule::FindNextHolidayId(const GregorianDate& from, Direction direction) const
{
	fNextHolidayDate = from;
	std::time_t now = std::time(nullptr);
	struct tm tempTM;
	localtime_r(&now, &tempTM);
	time_t	timeSinceEpoch;
	int holiday = 0;
	int counter = 0;
	char buffer[30];
	hdate_struct tempHDate;
	
	do
	{
		int dateChange = 0;
		if (direction == Direction::PREVIOUS) {
			dateChange = -1;
		} else if (direction == Direction::NEXT) {
			dateChange = 1;
		}
		tempTM.tm_mday += dateChange;
		counter++;
		timeSinceEpoch = mktime(&tempTM);
		hdate_set_gdate(&tempHDate,
						tempTM.tm_mday,
						tempTM.tm_mon + 1,
						tempTM.tm_year + 1900);
		holiday = hdate_get_holyday (&tempHDate, 1);		// Diaspora
		
		printf("Holiday = %d\n", holiday);
		
	} while (holiday == 0);
	
	HebrewDate Hdate(tempHDate.hd_day, tempHDate.hd_mon, tempHDate.hd_year);
	
	
	fNextHolidayDate = fJewishCalendar.ToGregorianDate(Hdate);
		
	switch (holidayNames[holiday].first) {
		case RELIGIOUS_MAJOR:
			sprintf(buffer, B_TRANSLATE("Judaic major"));
			break;
		case RELIGIOUS_MINOR:
			sprintf(buffer, B_TRANSLATE("Judaic minor"));
			break;
		case TZOM:
			sprintf(buffer, B_TRANSLATE("Religious Fast"));
			break;
		case HOL_HAMOED:
			sprintf(buffer, B_TRANSLATE("Chol HaMoed"));
			break;
		case MINOR:
			sprintf(buffer, B_TRANSLATE("Rememberance day"));
			break;
		case CIVIL:
			sprintf(buffer, B_TRANSLATE("Civil holiday"));
			break;
		default:
			sprintf(buffer, B_TRANSLATE("Not a holiday"));
			break;
	};
	fSecondLine->SetText(buffer);
			

	fFirstLine->SetText(holidayNames[holiday].second);
	
	sprintf(buffer, "On %d.%d.%d", tempTM.tm_mday, tempTM.tm_mon+1,
		tempTM.tm_year+1900);
	fThirdLine->SetText(buffer);
	
	switch (counter) {
		case 0: 
			fFourthLine->SetText(B_TRANSLATE("Tonight"));
			break;
		case 1:
			fFourthLine->SetText(B_TRANSLATE("Tomorrow"));
			break;
		default:
			sprintf(buffer, B_TRANSLATE("In %d days"), counter);
			fFourthLine->SetText(buffer);
			break;
	};	

	return holiday;
}		
		
void NextHolidayModule::UpdateThirdLine()
{
	char buffer[100];
	if (!fShownGregorianDate)	// Hebrew date is shown
	{
		BString toDisplay;
		switch (fNextHolidayDate.month) {
			case 1:
				toDisplay << B_TRANSLATE("January");
				break;
			case 2:
				toDisplay << B_TRANSLATE("February");
				break;
			case 3:
				toDisplay << B_TRANSLATE("March");
				break;
			case 4:
				toDisplay << B_TRANSLATE("April");
				break;
			case 5:
				toDisplay << B_TRANSLATE("May");
				break;
			case 6:
				toDisplay << B_TRANSLATE("June");
				break;
			case 7:
				toDisplay << B_TRANSLATE("July");
				break;
			case 8:
				toDisplay << B_TRANSLATE("August");
				break;
			case 9:
				toDisplay << B_TRANSLATE("September");
				break;
			case 10:
				toDisplay << B_TRANSLATE("October");
				break;
			case 11:
				toDisplay << B_TRANSLATE("November");
				break;
			case 12:
				toDisplay << B_TRANSLATE("December");
				break;
			default:
				toDisplay << B_TRANSLATE("No month");
				break;
		};
		
		
		sprintf(buffer, B_TRANSLATE("On %s %dth %d"),
			toDisplay.String(), fNextHolidayDate.day,
			fNextHolidayDate.year),
			
		fThirdLine->SetText(buffer);
		fShownGregorianDate = true;
	} else {
		// Gregorian date is shown
		HebrewDate Hdate =
			fJewishCalendar.ToHebrewDate(fNextHolidayDate);
		sprintf(buffer, B_TRANSLATE("On %d of %s, %d"),
			Hdate.Day(), JewishMonths[Hdate.Month()].name.String(),
			Hdate.Year());
		fThirdLine->SetText(buffer);
		fShownGregorianDate = false;
	}
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

void NextHolidayModule::MessageReceived(BMessage* in) {
	switch (in->what) {
		case ToggleDateLanguage:
			fprintf(stdout, "Message Received!\n");
			fflush(stdout);
		default:
			BBox::MessageReceived(in);
	}
}
