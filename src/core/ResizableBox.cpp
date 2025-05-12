/*
 * Copyright 2025, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "ResizableBox.h"

#include <stdio.h>

#include <Application.h>
#include <Window.h>

ResizableBox::ResizableBox(BRect frame,
		const char* name,
		uint32 resizingMode,
		uint32 flags,
		border_style border) :
	BBox(frame, name, resizingMode, flags, border),
	fBorderDraggingMode(false)
{
	_InitCursors();
}

ResizableBox::ResizableBox(const char* name,
		uint32 flags,
		border_style border,
		BView* child) :
	BBox(name, flags, border, child),
	fBorderDraggingMode(false)
{
	_InitCursors();
}

ResizableBox::ResizableBox(border_style border, BView* child) :
	BBox(border, child),
	fBorderDraggingMode(false)
{
	_InitCursors();
}

ResizableBox::~ResizableBox()
{
	if (fCursorDefault) delete fCursorDefault;
	if (fCursorGrabReady) delete fCursorGrabReady;
	if (fCursorGrabbing) delete fCursorGrabbing;
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
			if ((where.x <= borderWidth)					||
				(bounds.Width() - where.x <= borderWidth)	||
				(where.y <= borderWidth)					||
				(bounds.Height() - where.y <= borderWidth))
			{	
				be_app->SetCursor(fCursorGrabReady);
			}
			else
			{
				be_app->SetCursor(fCursorDefault);
			}
	}
	else if (code == B_EXITED_VIEW) {
			be_app->SetCursor(fCursorDefault);
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

void ResizableBox::_InitCursors()
{
	fCursorDefault = new BCursor(B_CURSOR_ID_SYSTEM_DEFAULT);
	fCursorGrabReady = new BCursor(B_CURSOR_ID_GRAB);
	fCursorGrabbing = new BCursor(B_CURSOR_ID_GRABBING);
}
