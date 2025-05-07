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
	BBox(frame, name, resizingMode, flags, border)
{
	
}



ResizableBox::ResizableBox(const char* name,
		uint32 flags,
		border_style border,
		BView* child) :
	BBox(name, flags, border, child)
{
	
}



ResizableBox::ResizableBox(border_style border, BView* child) :
		BBox(border, child)
{
	
}


void ResizableBox::MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) {
	BBox::MouseMoved(point, transit, dragMessage);									
}
