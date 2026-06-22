#pragma once

#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <Deskbar.h>
#include <Message.h>
#include <Messenger.h>
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

const uint32	PrevHolidayMessage	= 'pHDM';
const uint32	NextHolidayMessage 	= 'nHDM';
const uint32	ToggleDateLanguage	= 'tDLg';

enum class Direction {
	PREVIOUS = -1
	,NEXT = 1
};


class ClickableStringView : public BStringView {
	public:
		ClickableStringView (const char *a, const char *b, BHandler* in) :
			BStringView(a, b),
			target(in) { };
		virtual ~ClickableStringView() {};
		virtual void MouseDown(BPoint where);
		virtual void MouseMoved(BPoint where, uint32 transit,
			const BMessage* dragMessage);
	private:
		BHandler* target;
};

class NextHolidayModule : public BBox {
public:
    NextHolidayModule(BRect frame);
    NextHolidayModule(BMessage* in);
    virtual ~NextHolidayModule();
    
    static BArchivable* Instantiate(BMessage* in);
    virtual status_t Archive(BMessage* out, bool deep = true) const;
    virtual void AttachedToWindow() override;
    virtual void Pulse();
    virtual void MessageReceived (BMessage* in);
    
    // TODO:
    void UpdateCurrentHoliday(Direction);
    

private:
	void Init();
	void AddDragger();
	int FindNextHolidayId(const GregorianDate& from, Direction) const;
	void ToggleThirdLine();
	
	GregorianDate fToday;
	GregorianDate	fNextHolidayDate;
	int fSelectedOffset;	// 0 - nearest holiday, 1 - next, etc.
	JewishCalendar fJewishCalendar;
	rgb_color fViewColor;
	
	BButton* fPrevHolidayButton;
	BButton* fNextHolidayButton;
	
	BStringView* fFirstLine;
	BStringView* fSecondLine;
	ClickableStringView* fThirdLine;
	BStringView* fFourthLine;
	BCheckBox*	 fUpdateOnMidnight;
	bool fShownGregorianDate;
	Direction fDirection;
};

