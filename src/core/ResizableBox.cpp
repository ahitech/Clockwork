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
	borderDraggingMode(false)
{
	
}



ResizableBox::ResizableBox(const char* name,
		uint32 flags,
		border_style border,
		BView* child) :
	BBox(name, flags, border, child),
	borderDraggingMode(false)
{
	
}



ResizableBox::ResizableBox(border_style border, BView* child) :
	BBox(border, child),
	borderDraggingMode(false)
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
	switch (code)
	{
		B_ENTERED_VIEW:		// We need to change the cursor
				
		
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
