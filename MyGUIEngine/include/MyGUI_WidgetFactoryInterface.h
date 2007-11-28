/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_FACTORY_INTERFACE_H__
#define __MYGUI_WIDGET_FACTORY_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport WidgetFactoryInterface
	{
	public:
		virtual const Ogre::String& getType() = 0;
		virtual WidgetPtr createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name) = 0;
	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_FACTORY_INTERFACE_H__