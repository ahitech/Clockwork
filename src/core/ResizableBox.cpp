/*
 * Copyright 2025, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "ResizableBox.h"

ResizableBox::ResizableBox(BRect frame,
		const char* name,
		uint32 resizingMode,
		uint32 flags,
		border_style border) :
	BBox(frame, name, resizingMode, flags, border),
	fBorderDraggingMode(false)
{
	_LoadCursors();
}



ResizableBox::ResizableBox(const char* name,
		uint32 flags,
		border_style border,
		BView* child) :
	BBox(name, flags, border, child),
	fBorderDraggingMode(false)
{
	_LoadCursors();
}



ResizableBox::ResizableBox(border_style border, BView* child) :
	BBox(border, child),
	fBorderDraggingMode(false)
{
	_LoadCursors()
}

ResizableBox::~ResizableBox()
{
	if (fGrabCursor) delete fGrabCursor;
	if (fGrabbingCursor) delete fGrabbingCursor;
	if (fDefaultCursor) delete fDefaultCursor;
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
	float frameOffset = this->TopBorderOffset();
	BRect bounds = this->Bounds();
	switch (code)
	{
		B_ENTERED_VIEW:		// We need to change the cursor
		B_INSIDE_VIEW:
			if (	(where.x - bounds.LeftTop().x <= frameOffset) 		||
					(bounds.RightBottom().x - where.x <= frameOffset)	||
					(where.y - bounds.LeftTop().y <= frameOffset)		||
					(bounds.RightBottom().y - where.y <= frameOffset) )
			{
				be_app()->SetCursor(fGrabCursor);
			}
			else
			{
				be_app()->SetCursor(bDefaultCursor);
			}
			break;
		B_EXITED_VIEW:
			be_app()->SetCursor(bDefautCursor);
			break;		
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


void ResizableBox::_LoadCursors()
{
	fGrabCursor = new BCursor(B_CURSOR_ID_GRAB);
	fGrabbingCursor = new BCursor(B_CURSOR_ID_GRABBING);
	fDefaultCursor = new BCursor(B_CURSOR_ID_SYSTEM_DEFAULT);
}
