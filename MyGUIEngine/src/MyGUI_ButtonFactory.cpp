/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_ButtonFactory.h"
#include "MyGUI_Button.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ButtonFactory::ButtonFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("ButtonPressed") = newDelegate(this, &ButtonFactory::ButtonPressed);
		}

		ButtonFactory::~ButtonFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.unregisterDelegate("ButtonPressed");
		}

		const Ogre::String& ButtonFactory::getType()
		{
			return Button::getType();
		}

		WidgetPtr ButtonFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new Button(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void ButtonFactory::ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(ButtonPtr, _widget);
			static_cast<ButtonPtr>(_widget)->setButtonPressed(util::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI