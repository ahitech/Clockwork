
#include <Catalog.h>
#include <Entry.h>
#include <Rect.h>
#include <TranslationUtils.h>

#include <math.h>
#include <stdio.h>

#include "MoonModule.h"
#include "../core/HebrewDate.h"

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
	
	GetHebrewDate();
	GetTotalDaysInCurrentHebrewMonth();
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
	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_OVERLAY);
	DrawBitmap(fMoonPicture, fMoonPicture->Bounds(), targetSquare);
	SetDrawingMode(B_OP_COPY);
	
	DrawShadow(targetSquare);
}

void MoonModule::AttachedToWindow()
{
	BView* parent = Parent();
	if (parent)
	{
		SetViewColor (B_TRANSPARENT_COLOR);
	}
	BView::AttachedToWindow();	
}

unsigned int	MoonModule::GetHebrewDate()
{
	HebrewDate hDate(std::time(nullptr), true);
	return static_cast<unsigned int>(hDate.Day());
}

unsigned int	MoonModule::GetTotalDaysInCurrentHebrewMonth()
{
	HebrewDate hDate(std::time(nullptr), true);
	unsigned int toReturn = hDate.TotalDaysInThisMonth();
	return toReturn;
}

BShape*
MoonModule::CreateShadowShape(BRect bounds)
{
	unsigned int currentDay = GetHebrewDate();
	unsigned int totalDays = GetTotalDaysInCurrentHebrewMonth();

	if (currentDay == 0 || currentDay == 15 || totalDays == 0
			|| currentDay > totalDays) {
		return NULL;
	}

	float minSize = (bounds.Width() < bounds.Height())
		? bounds.Width() : bounds.Height();

	float radius = minSize / 2.0f;

	float centerX = bounds.left + bounds.Width() / 2.0f;
	float centerY = bounds.top + bounds.Height() / 2.0f;

	BPoint northPole(centerX, centerY - radius);
	BPoint southPole(centerX, centerY + radius);

	bool isWaxing = currentDay < 15;

	float k;
	if (isWaxing) {
		k = 1.0f - (2.0f * (currentDay - 1) / 14.0f);
	} else {
		unsigned int daysAfterFull = currentDay - 15;
		unsigned int secondHalfLength = totalDays - 15;
		k = 1.0f - (2.0f * daysAfterFull / (float)secondHalfLength);
	}

	const float kHalfCircleBezier = 4.0f / 3.0f;

	float terminatorControlX = centerX + k * radius * kHalfCircleBezier;

	BShape* shadowShape = new BShape();

	shadowShape->MoveTo(northPole);

	BPoint terminatorPoints[3] = {
		BPoint(terminatorControlX, northPole.y),
		BPoint(terminatorControlX, southPole.y),
		southPole
	};
	shadowShape->BezierTo(terminatorPoints);

	float edgeControlX = isWaxing
		? centerX - radius * kHalfCircleBezier
		: centerX + radius * kHalfCircleBezier;

	BPoint edgePoints[3] = {
		BPoint(edgeControlX, southPole.y),
		BPoint(edgeControlX, northPole.y),
		northPole
	};
	shadowShape->BezierTo(edgePoints);

	shadowShape->Close();

	return shadowShape;
}

void MoonModule::DrawShadow(BRect bounds) {
	BShape* shadowShape = CreateShadowShape(bounds);
	if (shadowShape == NULL) { return; }
	
	drawing_mode oldMode = this->DrawingMode();
	rgb_color	oldColor = this->HighColor();
	
	this->SetDrawingMode(B_OP_ALPHA);
	this->SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_OVERLAY);
	
	this->SetHighColor(0, 0, 0, 160);
	this->FillShape(shadowShape);
	
	delete shadowShape;
	this->SetDrawingMode(oldMode);
	this->SetHighColor(oldColor);
}
