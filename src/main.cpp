#include <Application.h>
#include <GroupLayout.h>
#include <LocaleRoster.h>
#include <LayoutItem.h>
#include <Window.h>
#include <Rect.h>
#include <stdio.h>

#include "modules/TodayModule.h"
#include "modules/MoonModule.h"
#include "modules/NextHoliday.h"
#include "modules/ConverterModule.h"

class ClockworkApp : public BApplication {
public:
    ClockworkApp() : BApplication("application/x-vnd.clockwork-app") {}

    void ReadyToRun() override {
    	
    	// Задать предпочтительный язык
    	BLocaleRoster* be_locale_roster = BLocaleRoster::Default();
//    	be_locale_roster->SetPreferredLanguages("ru");
    	
        BRect frame(100, 100, 470, 700);
        BWindow* window = new BWindow(frame, "Clockwork — Today", B_TITLED_WINDOW,
        	B_QUIT_ON_WINDOW_CLOSE | B_AUTO_UPDATE_SIZE_LIMITS);

        BRect viewFrame = (window->Bounds());
        
		BView* mainView = new BView(viewFrame, "main view",
			B_FOLLOW_ALL_SIDES,
			B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE);
		mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		window->AddChild(mainView);

		BGroupLayout* layout = new BGroupLayout(B_VERTICAL);
		layout->SetInsets(10, 10, 20, 10);
		mainView->SetLayout(layout);

        TodayModuleView* todayView = new TodayModuleView(BRect(0, 0, 300, 100));
        BLayoutItem* layoutItem = layout->AddView(todayView);
        layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP));
        

		NextHolidayModule* view = new NextHolidayModule(BRect(0, 100, 300, 300));
        
        layoutItem = layout->AddView(view);
        layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP));
        
        MoonModule* moonModule = new MoonModule(viewFrame);
        moonModule->MoveTo(0, viewFrame.Height());
        moonModule->ResizeTo(viewFrame.Width(), viewFrame.Height());
		
		layoutItem = layout->AddView(moonModule);
		layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_BOTTOM));
		
		ConverterModuleView* converterModule = new ConverterModuleView(viewFrame);
		layoutItem = layout->AddView(converterModule);
		layoutItem->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_BOTTOM));
		
        
        window->Show();
    }
};

int main() {
    ClockworkApp app;
    app.Run();
    return 0;
}
