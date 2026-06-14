
#ifndef __MOON_MODULE_H__
#define __MOON_MODULE_H__

#include <Bitmap.h>
#include <Box.h>
#include <Shape.h>
#include <View.h>
#include <Window.h>


class MoonModule : public BBox {
	public:
		MoonModule(BRect frame);
		~MoonModule();
	
		virtual void MessageReceived(BMessage* in) {};
		virtual void ResizeTo(float width, float height) {};
		virtual void AttachedToWindow() override;
		virtual void Draw(BRect updateRect) override;
	
	private:
		BBitmap*	fMoonPicture;
		BBox*		fOutBox;
		
		BBitmap*	LoadMoonPicture(const char* filePath);
		void 		DrawMoonPicture();
		unsigned int	GetHebrewDate();
		unsigned int	GetTotalDaysInCurrentHebrewMonth();
		
		BShape*		CreateShadowShape(BRect bounds);
		void		DrawShadow(BRect bounds);
};

#endif // __MOON_MODULE_H__

