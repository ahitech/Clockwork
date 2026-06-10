

#include <Entry.h>
#include <Rect.h>
#include <TranslationUtils.h>

#include <stdio.h>

#include "MoonModule.h"

MoonModule::MoonModule() :
	BView("Moon View", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS )
{
	BBox* outBox = CreateBBox();
	fMoonPicture = LoadMoonPicture("Moon.png");
	
		
}

MoonModule::~MoonModule() {
	if (fMoonPicture != nullptr) {
	}
}

BBox* MoonModule::CreateBBox() {
	BRect frame = this->Frame();
	frame.InsetBy(5, 5);
	BBox* outBox = new BBox(frame, "Out Box", B_FOLLOW_ALL_SIDES);
	return outBox;
}

BBitmap* MoonModule::LoadMoonPicture(const char* filePath)
{
	BEntry entry(filePath);
	if (!entry.Exists()) {
		fprintf(stderr,
				"Error: file with moon pic (%s) is not found!\n",
				filePath);
		return NULL;
	}
	BBitmap* bitmap = BTranslationUtils::GetBitmap(filePath);
	if (nullptr == bitmap) {
		fprintf(stderr,
				"Error: could not decode the image!\n");
	}
	return bitmap;
}
