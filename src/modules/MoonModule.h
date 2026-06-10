
#ifndef __MOON_MODULE_H__
#define __MOON_MODULE_H__

#include <Bitmap.h>
#include <Box.h>
#include <View.h>
#include <Window.h>


class MoonModule : public BBox {
	public:
		MoonModule(BRect frame);
		~MoonModule();
	
		virtual void MessageReceived(BMessage* in) {};
		virtual void ResizeTo(float width, float height) {};
		virtual void AttachedToWindow() {};
		virtual void Draw(BRect updateRect);
	
	private:
		BBitmap*	fMoonPicture;
		BBox*		fOutBox;
		
		virtual BBitmap*	LoadMoonPicture(const char* filePath);
		virtual void 		DrawMoonPicture();
};

#endif // __MOON_MODULE_H__

