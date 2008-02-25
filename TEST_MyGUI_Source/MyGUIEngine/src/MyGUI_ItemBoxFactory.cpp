/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_ItemBoxFactory.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ItemBoxFactory::ItemBoxFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			//manager.registerDelegate("ItemBox_AddString") = newDelegate(this, &ItemBoxFactory::ItemBox_AddString);
		}

		ItemBoxFactory::~ItemBoxFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			//manager.unregisterDelegate("ItemBox_AddString");
		}

		const Ogre::String& ItemBoxFactory::getType()
		{
			return ItemBox::_getType();
		}

		WidgetPtr ItemBoxFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new ItemBox(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		/*void ItemBoxFactory::ItemBox_AddString(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ItemBoxPtr, _widget, _key);
			static_cast<ItemBoxPtr>(_widget)->addItemString(_value);
		}*/

	} // namespace factory
} // namespace MyGUI
