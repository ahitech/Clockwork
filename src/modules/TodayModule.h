#pragma once

#include <View.h>
#include <String.h>
#include <ctime>
#include "../core/HebrewDate.h"

class TodayModuleView : public BView {
public:
    TodayModuleView();
    TodayModuleView(BMessage* archive);
    virtual ~TodayModuleView();

    void AttachedToWindow() override;
    void Draw(BRect updateRect) override;
    status_t Archive(BMessage* into, bool deep = true) const override;
    static BArchivable* Instantiate(BMessage* from);

private:
    HebrewDate fToday;
    BString fDateString;
    BString fParsha;
    BString fHoliday;

    void UpdateContent();
};
