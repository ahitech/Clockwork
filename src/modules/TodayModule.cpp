#include "TodayModule.h"
#include <Message.h>
#include <Font.h>
#include <View.h>
#include <StringView.h>
#include <ctime>

TodayModuleView::TodayModuleView()
    : BView("TodayModuleView", B_WILL_DRAW),
      fToday(std::time(nullptr), true)
{
    SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    UpdateContent();
}

TodayModuleView::TodayModuleView(BMessage* archive)
    : BView(archive),
      fToday(std::time(nullptr), true)
{
    SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    UpdateContent();
}

TodayModuleView::~TodayModuleView() {}

void TodayModuleView::AttachedToWindow() {
    BView::AttachedToWindow();
    SetFont(be_plain_font);
}

void TodayModuleView::Draw(BRect updateRect) {
    BView::Draw(updateRect);
    
    float y = 15.0f;
    DrawString(fDateString.String(), BPoint(10, y));
    y += 15.0f;

    if (!fHoliday.IsEmpty()) {
        BString line("Holiday: ");
        line += fHoliday;
        DrawString(line.String(), BPoint(10, y));
        y += 15.0f;
    }

    if (!fParsha.IsEmpty()) {
        BString line("Parsha: ");
        line += fParsha;
        DrawString(line.String(), BPoint(10, y));
    }
}

status_t TodayModuleView::Archive(BMessage* into, bool deep) const {
    BView::Archive(into, deep);
    into->AddString("class", "TodayModuleView");
    return B_OK;
}

BArchivable* TodayModuleView::Instantiate(BMessage* from) {
    if (validate_instantiation(from, "TodayModuleView"))
        return new TodayModuleView(from);
    return nullptr;
}

void TodayModuleView::UpdateContent() {
    fDateString = fToday.ToStringShort().c_str();
    fParsha = fToday.ToParsha().c_str();
    fHoliday = fToday.ToHolidayName().c_str();
}
