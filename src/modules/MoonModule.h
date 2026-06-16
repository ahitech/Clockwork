
#ifndef __MOON_MODULE_H__
#define __MOON_MODULE_H__

#include <Bitmap.h>
#include <Box.h>
#include <Dragger.h>
#include <Shape.h>
#include <View.h>
#include <Window.h>


class MoonModule : public BBox {
	public:
		MoonModule(BRect frame);
		MoonModule(BMessage* in);
		~MoonModule();
	
		virtual void MessageReceived(BMessage* in) {};
		static  BArchivable* Instantiate(BMessage* in);
		virtual void AttachedToWindow() override;
		virtual void Draw(BRect updateRect) override;
		virtual status_t Archive(BMessage* out, bool deep = true) const;
		virtual void Pulse();
	
	private:
		BBitmap*	fMoonPicture;
		uint		day;
		uint		monthDays;
		
		BBitmap*	LoadMoonPicture(const char* filePath);
		void 		DrawMoonPicture();
		unsigned int	GetHebrewDate();
		unsigned int	GetTotalDaysInCurrentHebrewMonth();
		
		BShape*		CreateShadowShape(BRect bounds);
		void		DrawShadow(BRect bounds);
		
		void AddDragger();
		void Init();
};

#endif // __MOON_MODULE_H__

