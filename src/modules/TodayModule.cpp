#include "TodayModule.h"

#include <Catalog.h>
#include <MenuItem.h>
#include <Message.h>
#include <Font.h>
#include <Locale.h>
#include <View.h>
#include <StringView.h>
#include <ctime>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <unordered_map>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TodayModuleView"


std::unordered_map<std::string, std::string> months =
	{	{"", 			B_TRANSLATE("") },
		{"Sivan", 		B_TRANSLATE("Sivan") },
		{"Tishrei", 	B_TRANSLATE("Tishrei") },
		{"Cheshvan",	B_TRANSLATE("Cheshvan") },
		{"Kislev",		B_TRANSLATE("Kislev") },
		{"Tevet",		B_TRANSLATE("Tevet") },
		{"Shevat",		B_TRANSLATE("Shevat") },
		{"Adar",		B_TRANSLATE("Adar") },
		{"Nisan",		B_TRANSLATE("Nisan") },
		{"Iyar",		B_TRANSLATE("Iyar") },
		{"Sivan",		B_TRANSLATE("Sivan") },
		{"Tammuz",		B_TRANSLATE("Tammuz") },
		{"Av",			B_TRANSLATE("Av") },
		{"Elul",		B_TRANSLATE("Elul") },
		{"Adar I",		B_TRANSLATE("Adar I") },
		{"Adar II",		B_TRANSLATE("Adar II") }
	};


const char* sivan = B_TRANSLATE("Sivan");



TodayModuleView::TodayModuleView(BRect frame)
    : BView(frame, 
    		"TodayModuleView", 
    		B_FOLLOW_H_CENTER | B_FOLLOW_V_CENTER,
    		B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_PULSE_NEEDED),
      fToday(std::time(nullptr), true)
{
    SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    UpdateContent();
    
    BRect resizableBoxFrame = frame;
    resizableBoxFrame.InsetBy(20, 20);

	fBox = new ResizableBox(resizableBoxFrame, "Box");
	
	fBox->SetLabel("Try it");
	
//	AddChild(fBox);
    
    if (1) {
    	frame.OffsetTo(B_ORIGIN);
		frame.top = frame.bottom - 7;
		frame.left = frame.right - 7;
		fDragger = new BDragger(frame, this, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
    	AddChild(fDragger);
    }
    
    BStringView *stringView = new BStringView(BRect(10, 20, 100, 35),
    	"stringView", B_TRANSLATE("String"));
    	
    AddChild(stringView);
}

TodayModuleView::TodayModuleView(BMessage* archive)
    : BView(archive),
      fToday(std::time(nullptr), true),
      fBox(NULL)
{
    SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
    SetFlags(Flags() | B_DRAW_ON_CHILDREN);
    UpdateContent();
}

TodayModuleView::~TodayModuleView() {
	if (fBox) { fBox->RemoveSelf(); delete fBox; }
}

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
	
	BWindow* window = Window();
	if (window != NULL) {
		// inside Clockwork app
		BMenuItem* item;
		BDragger *dragger = NULL;
		int i = 0;
		while (dragger == NULL) {
			dragger = dynamic_cast<BDragger*>(ChildAt(i++));
		}
		menu->AddItem(item = new BMenuItem(B_TRANSLATE("Remove replicant"),
				new BMessage(B_TRASH_TARGET)));
			item->SetTarget(dragger);
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
	{
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
    status_t status = BView::Archive(into, deep);
    if (status == B_OK) {
    	status = into->AddString("class", "TodayModuleView");
    }
    if (B_OK == status) {
    	status = into->AddRect("bounds", Bounds());
    }
    if (B_OK == status) {
    	status = into->AddString("add_on", "application/x-vnd.clockwork-app");
    }
    return status;
}

BArchivable* TodayModuleView::Instantiate(BMessage* from) {
    if (validate_instantiation(from, "TodayModuleView")) {
    	fprintf (stderr, "Instantiate: creating app from archive\n");
        return new TodayModuleView(from);
    }
    return nullptr;
}

void TodayModuleView::UpdateContent() {
	this->fToday = HebrewDate(std::time(nullptr), true);
    fDateString = fToday.ToStringShort().c_str();
    fParsha = fToday.ToParsha().c_str();
    fHoliday = fToday.ToHolidayName().c_str();
}


