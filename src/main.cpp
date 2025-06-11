#include <Application.h>
#include <LocaleRoster.h>
#include <Window.h>
#include <Rect.h>
#include <stdio.h>
#include "modules/TodayModule.h"

class ClockworkApp : public BApplication {
public:
    ClockworkApp() : BApplication("application/x-vnd.clockwork-app") {}

    void ReadyToRun() override {
    	
    	// Задать предпочтительный язык
    	BLocaleRoster* be_locale_roster = BLocaleRoster::Default();
    	be_locale_roster->SetPreferredLanguages("ru");
    	
        BRect frame(100, 100, 300, 200);
        BWindow* window = new BWindow(frame, "Clockwork — Today", B_TITLED_WINDOW,
        	B_QUIT_ON_WINDOW_CLOSE | B_AUTO_UPDATE_SIZE_LIMITS);

        BRect viewFrame = window->Bounds();
        fprintf(stderr, "Window bounds: %.1f x %.1f", viewFrame.Width(), viewFrame.Height());

        TodayModuleView* view = new TodayModuleView(viewFrame);
        view->ResizeToPreferred(); // Optional
        view->MoveTo(0, 0);
        view->ResizeTo(viewFrame.Width(), viewFrame.Height());

        window->AddChild(view);
        window->Show();
    }
};

int main() {
    ClockworkApp app;
    app.Run();
    return 0;
}
