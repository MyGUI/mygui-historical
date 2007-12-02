/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_BUTTON_FACTORY_H__
#define __MYGUI_BUTTON_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport ButtonFactory : public WidgetFactoryInterface
		{
		public:
			ButtonFactory::ButtonFactory();
			ButtonFactory::~ButtonFactory();

			// ���������� ���������� �������
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name);

			// ������ ��� ��������
			void ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_BUTTON_FACTORY_H__