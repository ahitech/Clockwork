/*
 * Copyright 2026, Alexey Burshtein <ahitech@gmail.com>
 */
#ifndef _CONVERTER_MODULE_H_
#define _CONVERTER_MODULE_H_

#include <Box.h>
#include <Button.h>
#include <Catalog.h>
#include <CheckBox.h>
#include <Deskbar.h>
#include <Handler.h>
#include <Menu.h>
#include <PopUpMenu.h>
#include <MenuField.h>
#include <Message.h>
#include <Messenger.h>
#include <String.h>
#include <StringView.h>
#include <View.h>
#include <PopUpMenu.h>
#include <Screen.h>
#include <String.h>
#include <TextControl.h>
#include <ctime>
#include <Dragger.h>
#include <Window.h>
#include "../core/HebrewDate.h"
#include "../core/JewishCalendar.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ConverterModule header"


const uint32	CONVERT_TO_GREGORIAN= 'cTGr';
const uint32	CONVERT_TO_HEBREW	= 'cTHb';
const uint32	TEXT_CHANGED		= 'cHgd';
const uint32	GREGORIAN_YEAR_CHG	= 'GYCH';
const uint32	HEBREW_YEAR_CHG		= 'HYCH';
const uint32	GREGORIAN_MONTH_CHG = 'GMCH';
const uint32	HEBREW_MONTH_CHG	= 'HMCH';

class NumbersTextControl : public BTextControl {
public:
	NumbersTextControl( uint allowedDigits,
						const char* name,
						const char* label,
						const char* data,
						BMessage* message = nullptr);
	NumbersTextControl (BMessage *in);
	virtual ~NumbersTextControl() {};
	virtual status_t Archive(BMessage* out, bool deep = true);
	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage* in);
	virtual status_t SetMessage(BMessage* message) {
		fToSetUponChange = message; return B_OK;
	}
	void SetStringToReplace(const char* );
	void SetHandler (BHandler* in) {
		fTarget = in;
	}
	virtual void KeyDown(const char* bytes, int32 numBytes);
private:
	char buffer[50];
	uint fAllowedDigits;
	bool fSanitizing;
	BMessage* fToSendUponChange;
	BHandler* fTarget;
	void SanitizeText();
};



class ConverterModuleView : public BBox {
public:
	ConverterModuleView(BRect frame);
	ConverterModuleView(BMessage* in);
	~ConverterModuleView() {};
	
	void MessageReceived(BMessage* in);
	void AttachedToWindow();

private:
	BButton* fToGregorian;
	BButton* fToHebrew;
	BView*	 fContentView;
	BStringView* fGregorianStringView;
	BStringView* fHebrewStringView;
	GregorianDate	fGregorianDate;
	HebrewDate		fHebrewDate;
	
	NumbersTextControl*	fHYear;
	NumbersTextControl* fGYear;
	BMenuField*		fHMonth;
	BPopUpMenu*		fHMonths;
	BMenuField*		fGMonth;
	BPopUpMenu*		fGMonths;
	BMenuField*		fGDay;
	BPopUpMenu*		fGDays;
	BMenuField*		fHDay;
	BPopUpMenu*		fHDays;
	
	BString fGregorianDateString;
	BString fHebrewDateString;
	JewishCalendar fJewishCalendar;
	
	void Init();
	void InitializeDateFields();
	void AddDragger();
	void InitializeDatesToToday();
	void BuildHebrewMonthsMenu(uint hebrewYear);
	void BuildGregorianMonthsMenu();
	void BuildHebrewDaysMenus(uint hebrewYear, uint hebrewMonth);
	void BuildGregorianDaysMenus(uint gregorianYear, uint gregorianMonth);
	void ClearMenu(BMenu* );
	int FindCurrentlySelectedItem(BPopUpMenu*, const char* string);
};


#endif // _CONVERTER_MODULE_H_
