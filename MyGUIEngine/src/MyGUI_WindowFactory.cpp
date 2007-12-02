/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_WindowFactory.h"
#include "MyGUI_Window.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		WindowFactory::WindowFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("WindowAutoAlpha") = newDelegate(this, &WindowFactory::WindowAutoAlpha);
			manager.registerDelegate("WindowMinMax") = newDelegate(this, &WindowFactory::WindowMinMax);
			manager.registerDelegate("WindowToStick") = newDelegate(this, &WindowFactory::WindowToStick);
		}

		WindowFactory::~WindowFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("WindowAutoAlpha");
			manager.unregisterDelegate("WindowMinMax");
			manager.unregisterDelegate("WindowToStick");
		}

		const Ogre::String& WindowFactory::getType()
		{
			static Ogre::String type("Window");
			return type;
		}

		WidgetPtr WindowFactory::createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new Window(_left, _top, _width, _height, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void WindowFactory::WindowToStick(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setIsToStick(util::parseBool(_value));
		}

		void WindowFactory::WindowAutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setAutoAlpha(util::parseBool(_value));
		}

		void WindowFactory::WindowMinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setMinMax(util::parseIntRect(_value));
		}

	} // namespace factory
} // namespace MyGUI