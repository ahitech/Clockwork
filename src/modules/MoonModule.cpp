
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
	
	DrawShadow(bounds);
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

BShape*		MoonModule::CreateShadowShape(BRect bounds)
{
	unsigned int currentDay = GetHebrewDate();
	unsigned int totalDays = GetTotalDaysInCurrentHebrewMonth();
	// When date is 15, the moon is full = no shadow at all
	if (currentDay == 0 || currentDay == 15 || totalDays == 0 ||
		currentDay > totalDays)
	{
		return NULL;
	}
	float minSize = (bounds.Width() < bounds.Height()) ?
		bounds.Width() : bounds.Height();
	float radius = minSize / 2.0f;
	BPoint northPole = BPoint((bounds.Width() + 20) / 2.0f, 10);
	BPoint southPole = BPoint((bounds.Width() + 20) / 2.0f, bounds.Height() + 10);
	float centerX = bounds.left + 10 + bounds.Width() / 2.0f ;
	float centerY = bounds.top + 10 + bounds.Height() / 2.0f;
	
	// How many steps are there on the moon's face, 15 or 14?
	float k = 0.0f;
	bool isWaxing = (currentDay < 15);
	if (isWaxing) {
		k = -1.0f + (2.0f / 15.0f) * currentDay;
	} else {
		unsigned int daysAfterFull = currentDay - 15;
		unsigned int secondHalfLength = totalDays - 15;
		k = 1.0f - (2.0f / (float)secondHalfLength) * daysAfterFull;
	}
	
	// Where the terminator crosses the equator?
	float terminatorX = centerX + (k * radius);
	
	const float kBezierCircle = 0.552f;
	float circleControlOffset = radius * kBezierCircle;
	
	// Internal arc (crossing the face of the moon)
	BShape *shadowShape = new BShape();
	shadowShape->MoveTo(northPole);
	BPoint terminatorControl1(terminatorX, centerY - circleControlOffset);
	BPoint terminatorControl2(terminatorX, centerY + circleControlOffset);
	BPoint bezierPoints[3] = { 	terminatorControl1,
								terminatorControl2,
								southPole 
							 };
	shadowShape->BezierTo(bezierPoints);
	
	// External arc (around the side of the moon)
	BPoint edgeControl1, edgeControl2;
	if (isWaxing) {
		edgeControl1 = BPoint(centerX - circleControlOffset,
							  centerY + circleControlOffset);
		edgeControl2 = BPoint(centerX - circleControlOffset,
							  centerY - circleControlOffset);
	} else {
		edgeControl1 = BPoint(centerX + circleControlOffset,
							  centerY + circleControlOffset);
		edgeControl2 = BPoint(centerX + circleControlOffset,
							  centerY - circleControlOffset);
	}
	BPoint edgePoints[3] = { edgeControl1,
							 edgeControl2,
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
