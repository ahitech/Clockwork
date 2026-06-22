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
	fShownGregorianDate = true;		// We start with Gregorian date
	SetViewColor(B_TRANSPARENT_COLOR);
	SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLabel(B_TRANSLATE("Next Hebrew Holiday"));
	fDirection = Direction::NEXT;

	SetFlags(Flags() | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE
		| B_PULSE_NEEDED);

	fToday = GregorianDate(std::time(nullptr));
	fSelectedOffset = 0;

	fPrevHolidayButton = new BButton("Prev Holiday", "<",
		new BMessage(PrevHolidayMessage));
	
	// We start with the "previous" button disabled
	fPrevHolidayButton->SetEnabled(false);

	fNextHolidayButton = new BButton("Next Holiday", ">",
		new BMessage(NextHolidayMessage));
	

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
	
	UpdateCurrentHoliday(fToday, fDirection);
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
	fNextHolidayButton->SetTarget(this);
	fPrevHolidayButton->SetTarget(this);
	UpdateThirdLine();
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
			UpdateCurrentHoliday(fToday, Direction::NEXT);
		}
		Invalidate();
	}
}

void NextHolidayModule::UpdateCurrentHoliday(GregorianDate fFrom, Direction dir)
{
	int holidayId = FindNextHolidayId(fFrom, dir);

	if (holidayId == 0) {
		fFirstLine->SetText(B_TRANSLATE("No holiday found"));
		return;
	}
}

int NextHolidayModule::FindNextHolidayId(const GregorianDate& from, Direction direction) const
{	
	struct tm tempTM = TMFromGregorianDate(from);
	time_t	timeSinceEpoch;
	int holiday = 0;
	static int counter = 0;
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
		counter += dateChange;

		timeSinceEpoch = mktime(&tempTM);
		hdate_set_gdate(&tempHDate,
						tempTM.tm_mday,
						tempTM.tm_mon + 1,
						tempTM.tm_year + 1900);
		holiday = hdate_get_holyday (&tempHDate, 1);		// Diaspora
		
		printf("Holiday = %d\n", holiday);
		
	} while (holiday == 0);
	
	HebrewDate Hdate(tempHDate.hd_year, tempHDate.hd_mon, tempHDate.hd_day);
	
	
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
	UpdateThirdLine();
	return holiday;
}		
		
void NextHolidayModule::UpdateThirdLine()
{
	char buffer[100];
	if (fShownGregorianDate)	// Gregorian date is shown
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
		
		
		sprintf(buffer, B_TRANSLATE("On %d of %s, %d"),
			fNextHolidayDate.day, toDisplay.String(),
			fNextHolidayDate.year),
			
		fThirdLine->SetText(buffer);
		fShownGregorianDate = true;
	} else {
		// Hebrew date is shown
		HebrewDate Hdate =
			fJewishCalendar.ToHebrewDate(fNextHolidayDate);
		sprintf(buffer, B_TRANSLATE("On %d of %s, %d"),
			Hdate.Day(), JewishMonths[Hdate.Month()].name.String(),
			Hdate.Year());
		fThirdLine->SetText(buffer);
		fShownGregorianDate = false;
	}
}

void NextHolidayModule::MessageReceived(BMessage* in) {
	switch (in->what) {
		case ToggleDateLanguage:
			fShownGregorianDate = (fShownGregorianDate) ? false : true;
			UpdateThirdLine();
			break;
		case NextHolidayMessage:
			UpdateCurrentHoliday(fNextHolidayDate, Direction::NEXT);
			fPrevHolidayButton->SetEnabled(true);
			break;
		case PrevHolidayMessage:
			UpdateCurrentHoliday(fNextHolidayDate, Direction::PREVIOUS);
			break;
		default:
			BBox::MessageReceived(in);
	}
}
