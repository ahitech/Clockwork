#pragma once

#include <Box.h>
#include <Deskbar.h>
#include <View.h>
#include <PopUpMenu.h>
#include <Screen.h>
#include <String.h>
#include <StringView.h>
#include <ctime>
#include <Dragger.h>
#include <Window.h>
#include "../core/HebrewDate.h"
#include "../core/JewishCalendar.h"

const uint32 kMsgToggleLiveInDeskbar = 'LIDB';
static const char* kDeskbarItemName = "clockwork";

class TodayModuleView : public BBox {
public:
    TodayModuleView(BRect);
    TodayModuleView(BMessage* archive);
    virtual ~TodayModuleView() {};

    void AttachedToWindow() override;
    status_t Archive(BMessage* into, bool deep = true) const override;
    static BArchivable* Instantiate(BMessage* from);
    void Pulse() override;
    
private:
    GregorianDate fToday;
    BString fDateString;
    BStringView* fTodayStringView;
    
    void UpdateContent();
    void Init();
    void AddDragger();
    JewishCalendar fJewishCalendar;
};
