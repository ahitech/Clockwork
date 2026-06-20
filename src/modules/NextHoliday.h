#pragma once

#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <Deskbar.h>
#include <Message.h>
#include <StringView.h>
#include <View.h>
#include <PopUpMenu.h>
#include <Screen.h>
#include <String.h>
#include <ctime>
#include <Dragger.h>
#include <Window.h>
#include "../core/HebrewDate.h"
#include "../core/ResizableBox.h"
#include "../core/JewishCalendar.h"

const uint32	PrevHolidayMessage	= "pHDM";
const uint32	NextHolidayMessage 	= "nHDM";

enum class Direction {
	PREVIOUS = -1
	,NEXT = 1
};

class NextHolidayModule : public BBox {
public:
    NextHolidayModule(BRect frame);
    NextHolidayModule(BMessage* in);
    ~NextHolidayModule();
    
    static BArchivable* Instantiate(BMessage* in);
    virtual status_t Archive(BMessage* out, bool deep = true) const;
    virtual void AttachedToWindow() override;
    virtual void Pulse();
    
    // TODO:
    void UpdateCurrentHoliday(Direction);
    void MouseDown(BPoint where) override {};
    

private:
	void Init();
	void AddDragger();
	int FindNextHolidayId(const GregorianDate& from, Direction) const;
	const char* HolidayName(int holidayId) const;
	
	GregorianDate fToday;
	GregorianDate	fNextHolidayDate;
	int fSelectedOffset;	// 0 - nearest holiday, 1 - next, etc.
	JewishCalendar fJewishCalendar;
	rgb_color fViewColor;
	
	BButton* fPrevHolidayButton;
	BButton* fNextHolidayButton;
	
	BStringView* fFirstLine;
	BStringView* fSecondLine;
	BStringView* fThirdLine;
	BStringView* fFourthLine;
	BCheckBox*	 fUpdateOnMidnight;
};

