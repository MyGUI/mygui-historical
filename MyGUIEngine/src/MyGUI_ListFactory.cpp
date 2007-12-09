/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_ListFactory.h"
#include "MyGUI_List.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ListFactory::ListFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("ListAddString") = newDelegate(this, &ListFactory::ListAddString);
		}

		ListFactory::~ListFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.unregisterDelegate("ListAddString");
		}

		const Ogre::String& ListFactory::getType()
		{
			return List::getType();
		}

		WidgetPtr ListFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new List(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void ListFactory::ListAddString(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(ListPtr, _widget);
			static_cast<ListPtr>(_widget)->addItemString(_value);
		}

	} // namespace factory
} // namespace MyGUI