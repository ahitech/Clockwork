
#ifndef __MOON_MODULE_H__
#define __MOON_MODULE_H__

#include <Bitmap.h>
#include <Box.h>
#include <View.h>
#include <Window.h>


class MoonModule : public BView {
	public:
		virtual MoonModule();
		~MoonModule();
	
		virtual void MessageReceived(BMessage* in) {};
		virtual void ResizeTo(float width, float height) {};
	
	private:
		BBitmap* fMoonPicture;
		
	
		virtual BBox* 		CreateBBox();
		virtual BBitmap*	LoadMoonPicture(const char* filePath);
};

#endif // __MOON_MODULE_H__

