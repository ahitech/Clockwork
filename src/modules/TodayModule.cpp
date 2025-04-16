#include "TodayModule.h"
#include <Message.h>
#include <Font.h>
#include <Locale.h>
#include <View.h>
#include <StringView.h>
#include <ctime>

TodayModuleView::TodayModuleView()
    : BView("TodayModuleView", B_WILL_DRAW),
      fToday(std::time(nullptr), true)
{
    SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    UpdateContent();
    BRect frame = this->Frame();
    
    if (1) {
    	frame.OffsetTo(B_ORIGIN);
		frame.top = frame.bottom - 7;
		frame.left = frame.right - 7;
		fDragger = new BDragger(frame, this, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
    	AddChild(fDragger);
    }
}

TodayModuleView::TodayModuleView(BMessage* archive)
    : BView(archive),
      fToday(std::time(nullptr), true)
{
    SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    UpdateContent();
}

TodayModuleView::~TodayModuleView() {}

void TodayModuleView::MouseDown(BPoint where)
{
	if (!Bounds().Contains(where))
		return;

	int32 buttons = 0;
	if (Window() != NULL && Window()->CurrentMessage() != NULL)
		Window()->CurrentMessage()->FindInt32("buttons", &buttons);

	if ((buttons & B_SECONDARY_MOUSE_BUTTON) == 0)
		return;
		
	BPopUpMenu *menu = new BPopUpMenu(B_EMPTY_STRING, false, false);
	menu->SetFont(be_plain_font);
	
	WorkspacesWindow *window = dynamic_cast<WorkspacesWindow*>(Window());
	if (window != NULL) {
		// inside Clockwork app
	} else {
		// Replicated
		BMenuItem* item;

		menu->AddSeparatorItem();

		// check which way
		BDragger *dragger = dynamic_cast<BDragger*>(ChildAt(0));
		if (dragger) {
			// replicant
			menu->AddItem(item = new BMenuItem(B_TRANSLATE("Remove replicant"),
				new BMessage(B_TRASH_TARGET)));
			item->SetTarget(dragger);
		} else {
			// Deskbar item
			menu->AddItem(item = new BMenuItem(B_TRANSLATE("Remove replicant"),
				new BMessage(kMsgToggleLiveInDeskbar)));
			item->SetTarget(this);
		}
	}
	
	ConvertToScreen(where);
	menu->Go(where, true, true, true);
}

void
TodayModuleView::MouseMoved(BPoint where, uint32 transit,
	const BMessage* dragMessage)
{
	TodayModuleView* window = dynamic_cast<TodayModuleView*>(Window());
	if (window == NULL) //  || !window->IsAutoRaising())
		return;

	// Auto-Raise

	where = ConvertToScreen(where);
//	BScreen screen(window);
//	BRect screenFrame = screen.Frame();
	BRect windowFrame = window->Frame();
	float tabHeight = 5; // window->GetTabHeight();
	float borderWidth = 5; // window->GetBorderWidth();

		// Stretch frame to also accept mouse moves over the window borders
		windowFrame.InsetBy(-borderWidth, -(tabHeight + borderWidth));

		if (windowFrame.Contains(where))
			if (Window()) {
				Window()->Activate();
			}
	}
}

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

void TodayModuleView::MessageReceived(BMessage* message) {
	switch (message->what) {
		case B_ABOUT_REQUESTED:
			// _AboutRequested();
			break;
		case kMsgToggleLiveInDeskbar:
		{
			// only actually used from the replicant itself
			// since HasItem() locks up we just remove directly.
			BDeskbar deskbar;
			// we shouldn't do this here actually, but it works for now...
			deskbar.RemoveItem(kDeskbarItemName);
			break;
		}
		default:
			BView::MessageReceived(message);
			break;
	};
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
