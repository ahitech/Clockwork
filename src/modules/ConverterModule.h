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
#include <Menu.h>
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
	
	virtual void KeyDown(const char* bytes, int32 numBytes);
private:
	uint fAllowedDigits;
	bool fSanitizing;
	
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
	
	BTextControl* 	fHYear;
	BTextControl* 	fGYear;
	BMenuField*		fHMonth;
	BMenu*			fHMonths;
	BMenuField*		fGMonth;
	BMenu*			fGMonths;
	BMenuField*		fHDay;
	BMenuField*		fGDay;
	
	
	BString fGregorianDateString;
	BString fHebrewDateString;
	JewishCalendar fJewishCalendar;
	
	void Init();
	void AddDragger();
	void InitializeDatesToToday();
};


#endif // _CONVERTER_MODULE_H_
