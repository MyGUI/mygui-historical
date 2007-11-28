/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WINDOW_FACTORY_H__
#define __MYGUI_WINDOW_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport WindowFactory : public WidgetFactoryInterface
		{
		public:
			WindowFactory();
			~WindowFactory();

			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);

			void WindowToStick(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void WindowAutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void WindowMinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_WINDOW_FACTORY_H__