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
#include <Message.h>
#include <Messenger.h>
#include <String.h>
#include <StringView.h>
#include <View.h>
#include <PopUpMenu.h>
#include <Screen.h>
#include <String.h>
#include <ctime>
#include <Dragger.h>
#include <Window.h>
#include "../core/HebrewDate.h"
#include "../core/JewishCalendar.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ConverterModule header"


const uint32	CONVERT_TO_GREGORIAN= 'cTGr';
const uint32	CONVERT_TO_HEBREW	= 'cTHb';


class ConverterModuleView : public BBox {
public:
	ConverterModuleView(BRect frame);
	ConverterModuleView(BMessage* in);
	~ConverterModuleView() {};
	
	
	void AttachedToWindow();

private:
	BButton* fToGregorian;
	BButton* fToHebrew;
	BStringView* fGregorianDateStringView;
	BStringView* fHebrewDateStringView;
	GregorianDate	fGregorianDate;
	HebrewDate		fHebrewDate;
	
	BString fGregorianDateString;
	BString fHebrewDateString;
	JewishCalendar fJewishCalendar;
	
	void Init();
	void AddDragger();
};


#endif // _CONVERTER_MODULE_H_
