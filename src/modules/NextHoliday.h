#pragma once

#include <Box.h>
#include <Deskbar.h>
#include <Message.h>
#include <View.h>
#include <PopUpMenu.h>
#include <Screen.h>
#include <String.h>
#include <ctime>
#include <Dragger.h>
#include <Window.h>
#include "../core/HebrewDate.h"
#include "../core/ResizableBox.h"

class NextHolidayModule : public BBox {
public:
    NextHolidayModule(BRect frame);
    NextHolidayModule(BMessage* in);
    ~NextHolidayModule();
    
    virtual BArchivable* Instantiate(BMessage* in);
    virtual status_t Archive(BMessage* out, bool deep = true) const;
    virtual void AttachedToWindow() override;
    
    // TODO: Finish this function
    virtual void Pulse() {};

private:
	void Init();
	void AddDragger();

};
