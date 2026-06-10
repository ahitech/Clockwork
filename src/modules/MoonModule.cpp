
#include <Catalog.h>
#include <Entry.h>
#include <Rect.h>
#include <TranslationUtils.h>

#include <stdio.h>

#include "MoonModule.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Moon Module"

MoonModule::MoonModule(BRect frame) :
	BBox(frame,
		 "Moon View",
		 B_FOLLOW_H_CENTER | B_FOLLOW_V_CENTER,
		 B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS ),
	fOutBox(nullptr),
	fMoonPicture(nullptr)
{
	fMoonPicture = LoadMoonPicture("Moon.png");
	this->SetLabel(B_TRANSLATE("Current moon phase"));	
	DrawMoonPicture();
	
}

MoonModule::~MoonModule() {
	if (fMoonPicture != nullptr) {
	}
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


void MoonModule::Draw(BRect updateRect) {
	if (fMoonPicture) DrawMoonPicture();
}

void MoonModule::DrawMoonPicture()
{
	if (fMoonPicture == nullptr) {
		fprintf(stderr,
				"Error: picture doesn't exist!\n");
		return;
	}
	BRect bounds = this->Bounds(); 
	bounds.InsetBy(10, 10);
	float minSide = (bounds.Width() > bounds.Height()) ?
					bounds.Height() : bounds.Width();
	float xOffset = 10 + (bounds.Width() - minSide) / 2.0f;
	float yOffset = 10 + (bounds.Height() - minSide) / 2.0f;
	BRect targetSquare(	xOffset, yOffset,
						xOffset + minSide, yOffset + minSide);
	DrawBitmap(fMoonPicture, fMoonPicture->Bounds(), targetSquare);
}
