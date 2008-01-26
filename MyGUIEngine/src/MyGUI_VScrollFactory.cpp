/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_VScrollFactory.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		VScrollFactory::VScrollFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("Scroll_Range") = newDelegate(this, &VScrollFactory::Scroll_Range);
			manager.registerDelegate("Scroll_Position") = newDelegate(this, &VScrollFactory::Scroll_Position);
			manager.registerDelegate("Scroll_Page") = newDelegate(this, &VScrollFactory::Scroll_Page);
		}

		VScrollFactory::~VScrollFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("Scroll_Range");
			manager.unregisterDelegate("Scroll_Position");
			manager.unregisterDelegate("Scroll_Page");
		}

		const Ogre::String& VScrollFactory::getType()
		{
			return VScroll::_getType();
		}

		WidgetPtr VScrollFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new VScroll(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void VScrollFactory::Scroll_Range(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(VScrollPtr, _widget);
			static_cast<VScrollPtr>(_widget)->setScrollRange(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_Position(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(VScrollPtr, _widget);
			static_cast<VScrollPtr>(_widget)->setScrollPosition(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_Page(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(VScrollPtr, _widget);
			static_cast<VScrollPtr>(_widget)->setScrollPage(utility::parseSizeT(_value));
		}

	} // namespace factory

} // namespace MyGUI
