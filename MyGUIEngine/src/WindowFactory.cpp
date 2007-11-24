
#include "WindowFactory.h"
#include "Window.h"
#include "SkinManager.h"
#include "WidgetManager.h"

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

		WidgetPtr WindowFactory::createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new Window(_left, _top, _width, _height, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void WindowFactory::WindowToStick(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setIsToStick(util::parseBool(_value));
		}

		void WindowFactory::WindowAutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setAutoAlpha(util::parseBool(_value));
		}

		void WindowFactory::WindowMinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setMinMax(util::parseIntRect(_value));
		}

	} // namespace factory
} // namespace MyGUI