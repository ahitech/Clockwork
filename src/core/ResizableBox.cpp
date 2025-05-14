/*
 * Copyright 2025, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "ResizableBox.h"

#include <stdio.h>

#include <Application.h>
#include <Window.h>

#pragma region // -----==< Implementation of the struct Cursors >==-----
Cursors::Cursors()
{
	defaultCursor 	= new BCursor(B_CURSOR_ID_SYSTEM_DEFAULT);
	draggable		= new BCursor(B_CURSOR_ID_GRAB);
	dragging	 	= new BCursor(B_CURSOR_ID_GRABBING);
	draggingN 		= new BCursor(B_CURSOR_ID_RESIZE_NORTH);
	draggingS 		= new BCursor(B_CURSOR_ID_RESIZE_SOUTH);
	draggingW 		= new BCursor(B_CURSOR_ID_RESIZE_WEST);
	draggingE 		= new BCursor(B_CURSOR_ID_RESIZE_EAST);
	draggingNW 		= new BCursor(B_CURSOR_ID_RESIZE_NORTH_WEST);
	draggingNE 		= new BCursor(B_CURSOR_ID_RESIZE_NORTH_EAST);
	draggingSW 		= new BCursor(B_CURSOR_ID_RESIZE_SOUTH_WEST);
	draggingSE 		= new BCursor(B_CURSOR_ID_RESIZE_SOUTH_EAST);
}

Cursors::~Cursors()
{
	if (defaultCursor) 		delete defaultCursor;
	if (draggable) 			delete draggable;
	if (dragging) 			delete dragging;
	if (draggingN)			delete draggingN;
	if (draggingS)			delete draggingS;
	if (draggingW)			delete draggingW;
	if (draggingE)			delete draggingE;
	if (draggingNW)			delete draggingNW;
	if (draggingNE)			delete draggingNE;
	if (draggingSW)			delete draggingSW;
	if (draggingSE)			delete draggingSE;
}
#pragma endregion

#pragma region;	// -----==< Public functions of ResizableBox >==-----
ResizableBox::ResizableBox(BRect frame,
		const char* name,
		uint32 resizingMode,
		uint32 flags,
		border_style border) :
	BBox(frame, name, resizingMode, flags, border),
	fBorderDraggingMode(false)
{
	
}

ResizableBox::ResizableBox(const char* name,
		uint32 flags,
		border_style border,
		BView* child) :
	BBox(name, flags, border, child),
	fBorderDraggingMode(false)
{
	
}

ResizableBox::ResizableBox(border_style border, BView* child) :
	BBox(border, child),
	fBorderDraggingMode(false)
{
	
}

ResizableBox::~ResizableBox()
{
	
}

void ResizableBox::MessageReceived(BMessage *in)
{
	if (in) {
		switch (in->what) {
			
			default:
				BBox::MessageReceived(in);
		};
	}	
}


void ResizableBox::MouseMoved(BPoint where, uint32 code, const BMessage* dragMessage)
{
	BRect bounds = Bounds();
	float borderWidth = this->TopBorderOffset();
	
	if (code == B_ENTERED_VIEW || code == B_INSIDE_VIEW)
	{
		be_app->SetCursor(_WhichCursorSuits(_IsCursorNearTheBorder(where)));
	}
	else if (code == B_EXITED_VIEW) {
			be_app->SetCursor(cursors.defaultCursor);
	};
	
	BBox::MouseMoved(where, code, dragMessage);									
}

void ResizableBox::MouseDown(BPoint where)
{
	int32 clickCount = 0;
	uint32 buttons = 0;
	MakeFocus(true);
	if (Window() != NULL && Window()->CurrentMessage() != NULL) {
		clickCount = Window()->CurrentMessage()->FindInt32("clicks");
		buttons = Window()->CurrentMessage()->FindInt32("buttons");
	}
	
}

void ResizableBox::MouseUp(BPoint where)
{
	
}
#pragma endregion

#pragma region	// -----==< Protected functions of ResizableBox >==----- 
enum Border ResizableBox::_IsCursorNearTheBorder(BPoint in)
{
	bool N = false, S = false, E = false, W = false;
	BRect bounds = Bounds();
	float threshold = this->TopBorderOffset();
	enum Border toReturn = FAR_FROM_BORDER;
	
	if (in.x <= threshold) { W = true; toReturn = LEFT; }
	if (bounds.Width() - in.x <= threshold) { E = true; toReturn = RIGHT; }
	if (in.y <= threshold) { N = true; toReturn = TOP; }
	if (bounds.Height() - in.y <= threshold) { S = true; toReturn = BOTTOM; }
	
	if (N & W) { toReturn = TOP_LEFT; }
	if (N & E) { toReturn = TOP_RIGHT; }
	if (S & W) { toReturn = BOTTOM_LEFT; }
	if (S & E) { toReturn = BOTTOM_RIGHT; }
	
	return toReturn;
}

BCursor* ResizableBox::_WhichCursorSuits(enum Border in)
{
	BCursor* toReturn = NULL;
	
	if (!fBorderDraggingMode && in != FAR_FROM_BORDER)
	{
		toReturn = cursors.draggable;
	}
	else
	{
		switch (in)
		{
			case TOP:
				toReturn = cursors.draggingN; break;
			case BOTTOM:
				toReturn = cursors.draggingS; break;
			case LEFT:
				toReturn = cursors.draggingW; break;
			case RIGHT:
				toReturn = cursors.draggingE; break;
			case TOP_LEFT:
				toReturn = cursors.draggingNW; break;
			case TOP_RIGHT:
				toReturn = cursors.draggingNE; break;
			case BOTTOM_LEFT:
				toReturn = cursors.draggingSW; break;
			case BOTTOM_RIGHT:
				toReturn = cursors.draggingSE; break;
			case FAR_FROM_BORDER:		// Intentional fall-through
			default:
				toReturn = cursors.defaultCursor; break;
		};
	}
	
	return toReturn;
}
#pragma endregion
