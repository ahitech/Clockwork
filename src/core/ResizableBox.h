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
#include <Cursor.h>
#include <SupportDefs.h>

/** 
 * @brief	This enum answers the question whether the cursor is near the border
 *			of the ResizableBox.
 * @details	The value is set by the function that checks the position of the cursor.
 *			It is used later for deciding how the cursor which is probably hovering
 *			over the border should be changed, if at all.
 */
enum Border {
	FAR_FROM_BORDER = 0,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

/**
 * @brief	This structure prepares all cursors that can be used by the ResizableBox.
 * @details	The usage is purely for clarity of the code and for optimizing the runtime
 *			latency by a fraction of msec. End user would probably not feel the
 *			difference between dynamically allocating all resources at the beginning
 *			of the program's run or when it's already running.
 */
struct Cursors {
	BCursor* defaultCursor;
	BCursor* draggable;
	BCursor* dragging;		//!< It is advised not to use it, as it gives no
							//	 sense of direction of resizing
	BCursor* draggingN;
	BCursor* draggingS;
	BCursor* draggingW;
	BCursor* draggingE;
	BCursor* draggingNW;
	BCursor* draggingNE;
	BCursor* draggingSW;
	BCursor* draggingSE;
	
	Cursors();
	virtual ~Cursors();
};
	

/**
 * @brief	Resizable BBox class.
 * @detals	The main idea is to create a BBox with visible borders that
 *			can be resized by dragging these borders. That will allow resizing
 *			the view inside the BBox. Then the ResizableBox can be turned into
 *			a replicant, which, in turn, makes possible creating replicants with
 *			size independent of the size of the other views or windows of the program.
 */
class _EXPORT ResizableBox :
	public BBox
{
public:
	// Constructors
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
	ResizableBox(BMessage* from);

	virtual ~ResizableBox();
	
	// Reimplemented functions
	virtual void MouseMoved(BPoint where, uint32 code, const BMessage* dragMessage);
	virtual void MessageReceived(BMessage* message);
	virtual void MouseDown(BPoint where);
	virtual void MouseUp(BPoint where);
	
	virtual BArchivable* Instantiate(BMessage* archive);
	virtual status_t	 Archive(BMessage* into, bool deep) const;

protected:
	bool fBorderDraggingMode;
	Cursors* cursors; 
	
	virtual enum Border _IsCursorNearTheBorder(BPoint in);
	virtual BCursor* _WhichCursorSuits(enum Border in);
	virtual void _AddDragger();

};



#endif // RESIZABLE_BOX_H
