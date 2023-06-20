/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2002 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.	It may not be distributed
 * under any circumstances.
 */

#ifndef PI_FORMS_LISTENERS
#define PI_FORMS_LISTENERS

//
// Listeners for platform independent forms (piforms)
//

#include "modules/widgets/OpWidget.h"

class DocumentManager;
class HTML_Element;
class ES_Thread;

/**
 * A listener for form widget events.
 */
class WidgetListener : public OpWidgetListener
{
public:
	/**
	 * Constructor.
	 *
	 * @param doc_man The document manager.
	 * @param helm The HTML element the widget is created from.
	 */
	WidgetListener(DocumentManager* doc_man, HTML_Element* helm) : doc_man(doc_man)
		, helm(helm)
	{
	}

	/**
	 * Should be called when the widget is clicked.
	 *
	 * @param object The widget that is clicked.
	 * @param id an id.
	 */
	virtual void OnClick(OpWidget *object, UINT32 id = 0);

	/**
	 * Is called in addition to OnClick if the widget has invented a click of
	 * its own. For instance in response to a keypress or other action.
	 */
	virtual void OnGeneratedClick(OpWidget *widget, UINT32 id = 0);

	/**
	 * Should be called when the data in the widget is changed.
	 *
	 * @param object The widget that is changed.
	 * @param changed_by_mouse TRUE if the change is because of a
	 * mouse operation.
	 */
	virtual void OnChange(OpWidget *object, BOOL changed_by_mouse);

	/**
	 * Should be called when the data has changed and the user has left
	 * the widget signalling that the change is finished.
	 *
	 * @param widget The widget that is changed.
	 */
	virtual void OnChangeWhenLostFocus(OpWidget *widget);

	/**
	 * Called when the widget is selected.
	 *
	 * @param object The widget that is selected.
	 */
	virtual void OnSelect(OpWidget *object);

	/**
	 * Called when the widget is scrolled.
	 *
	 * @param widget The widget that is scrolled.
	 * @param old_val The old scroll value.
	 * @param new_val The new scroll value.
	 * @param caused_by_input TRUE if caused by user input.
	 * @see OpWidget for more details.
	 */
	virtual void OnScroll(OpWidget *widget, INT32 old_val, INT32 new_val, BOOL caused_by_input);

	/**
	 * Called when a resize is requested by the user.
	 *
	 * @param widget The OpWidget that was resized.
	 * @param width The new width of the OpWidget.
	 * @param height The new height of the OpWidget.
	 */
	virtual void OnResizeRequest(OpWidget *widget, INT32 width, INT32 height);

	/**
	 * Process a generated click. If thread is not NULL, then it was
	 * generated by a script, and any events should be synthetic
	 * and interrupt thread.
	 *
	 * @param widget The widget that should have a "fake" click.
	 *
	 * @param thread The thread causing this or NULL.
	 */
	void HandleGeneratedClick(OpWidget* widget, ES_Thread* thread);
protected:

	/**
	 * The document manager supplied to the constructor.
	 */
	DocumentManager* doc_man;

	/**
	 * The HTML_Element supplied to the constructor.
	 */
	HTML_Element* helm;
};

/**
 * A listener for radio button events.
 */
class RadioButtonListener : public WidgetListener
{
public:
	/**
	 * Constructor for the listener.
	 *
	 * @param doc_man The document manager.
	 * @param helm The HTML element the widget is created from.
	 */
	RadioButtonListener(DocumentManager* doc_man, HTML_Element* helm);
	virtual void OnClick(OpWidget *object, UINT32 id = 0);
};

/**
 * A listener for selection list events.
 */
class SelectionObjectListener : public WidgetListener
{
public:
	/**
	 * Constructor for the listener.
	 *
	 * @param doc_man The document manager.
	 * @param helm The HTML element the widget is created from.
	 */
	SelectionObjectListener(DocumentManager* doc_man, HTML_Element* helm);

	/**
	 * Should be called when the widget is clicked or gets used by a mouse
	 * some other way.
	 *
	 * @param widget The widget that is used.
	 * @param pos
	 * @param x The x position of the click.
	 * @param y The y position of the click.
	 * @param button The mouse button used for the click.
	 * @param down
	 * @param nclicks which click in a sequence of clicks this is.
	 */
	virtual void OnMouseEvent(OpWidget *widget,
							  INT32 pos,
							  INT32 x,
							  INT32 y,
							  MouseButton button,
							  BOOL down,
							  UINT8 nclicks);
};

#endif // PI_FORMS_LISTENERS
