/*
 * Copyright 2025, Alexey Burshtein <ahitech@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 
/**
 * @file	Resizable BBox class implementation.
 * @author	Alexey "Hitech" Burshtein
 */
#ifndef RESIZABLE_BOX_H
#define RESIZABLE_BOX_H

#include <Box.h>
#include <SupportDefs.h>

/**
 * @brief	Resizable BBox class.
 * @detals	The main idea is to create a BBox with visible borders that
 *			can be resized by dragging these borders. That will allow resizing
 *			the view inside the BBox. Then the ResizableBox can be turned into
 *			a replicant, which, in turn, makes possible creating replicants with
 *			size independent of the size of the other views or windows of the program.
 */
class ResizableBox :
	public BBox
{
public:
	ResizableBox(BRect frame,
		const char* name,
		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
		uint32 flags = B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE_JUMP,
		border_style border = B_FANCY_BORDER);
	ResizableBox(const char* name,
		uint32 flags = B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE_JUMP,
		border_style border = B_FANCY_BORDER,
		BView* child = NULL);
	ResizableBox(border_style border, BView* child = NULL);
		
	virtual ~ResizableBox() = default;
	
	virtual void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage);

private:

};


#endif // RESIZABLE_BOX_H
