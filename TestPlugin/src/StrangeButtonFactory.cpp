
#include "StrangeButtonFactory.h"
#include "StrangeButton.h"
#include "SkinManager.h"
#include "WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		StrangeButtonFactory::StrangeButtonFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("StrangeButtonPressed") = newDelegate(this, &StrangeButtonFactory::ButtonPressed);
		}

		StrangeButtonFactory::~StrangeButtonFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("StrangeButtonPressed");
		}

		// ���������� ���������� �������
		const Ogre::String& StrangeButtonFactory::getType()
		{
			static Ogre::String type("StrangeButton");
			return type;
		}

		WidgetPtr StrangeButtonFactory::createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new StrangeButton(_left, _top, _width, _height, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		// ������ ��� ��������
		void StrangeButtonFactory::ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(StrangeButtonPtr, _widget);
			static_cast<StrangeButtonPtr>(_widget)->setButtonPressed(util::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI