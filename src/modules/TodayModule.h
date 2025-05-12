#pragma once

#include <Deskbar.h>
#include <View.h>
#include <PopUpMenu.h>
#include <Screen.h>
#include <String.h>
#include <ctime>
#include <Dragger.h>
#include <Window.h>
#include "../core/HebrewDate.h"
#include "../core/ResizableBox.h"

const uint32 kMsgToggleLiveInDeskbar = 'LIDB';
static const char* kDeskbarItemName = "clockwork";

class TodayModuleView : public BView {
public:
    TodayModuleView(BRect);
    TodayModuleView(BMessage* archive);
    virtual ~TodayModuleView();

    void AttachedToWindow() override;
    void Draw(BRect updateRect) override;
    status_t Archive(BMessage* into, bool deep = true) const override;
    static BArchivable* Instantiate(BMessage* from);
    
    void MouseDown(BPoint where);
    void MessageReceived(BMessage *message);
    void MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage);

private:
    HebrewDate fToday;
    BString fDateString;
    BString fParsha;
    BString fHoliday;
	BDragger* fDragger = NULL;
	ResizableBox* fBox;
	
    void UpdateContent();
};
