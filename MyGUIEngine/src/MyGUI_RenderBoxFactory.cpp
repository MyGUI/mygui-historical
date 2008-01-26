/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#include "MyGUI_RenderBoxFactory.h"
#include "MyGUI_RenderBox.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		RenderBoxFactory::RenderBoxFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("RenderBox_Mesh") = newDelegate(this, &RenderBoxFactory::RenderBox_Mesh);
			manager.registerDelegate("RenderBox_AutorotationSpeed") = newDelegate(this, &RenderBoxFactory::RenderBox_AutorotationSpeed);
			manager.registerDelegate("RenderBox_BackgroungColour") = newDelegate(this, &RenderBoxFactory::RenderBox_BackgroungColour);
			manager.registerDelegate("RenderBox_RotationAngle") = newDelegate(this, &RenderBoxFactory::RenderBox_RotationAngle);
			manager.registerDelegate("RenderBox_MouseRotation") = newDelegate(this, &RenderBoxFactory::RenderBox_MouseRotation);
		}

		RenderBoxFactory::~RenderBoxFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("RenderBox_Mesh");
			manager.unregisterDelegate("RenderBox_AutorotationSpeed");
			manager.unregisterDelegate("RenderBox_BackgroungColour");
			manager.unregisterDelegate("RenderBox_RotationAngle");
			manager.unregisterDelegate("RenderBox_MouseRotation");
		}

		const Ogre::String& RenderBoxFactory::getType()
		{
			return RenderBox::_getType();
		}

		WidgetPtr RenderBoxFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new RenderBox(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void RenderBoxFactory::RenderBox_Mesh(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(RenderBoxPtr, _widget);
			static_cast<RenderBoxPtr>(_widget)->injectObject(_value);
		}

		void RenderBoxFactory::RenderBox_AutorotationSpeed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(RenderBoxPtr, _widget);
			static_cast<RenderBoxPtr>(_widget)->setAutorotationSpeed(utility::parseInt(_value));
		}

		void RenderBoxFactory::RenderBox_BackgroungColour(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(RenderBoxPtr, _widget);
			static_cast<RenderBoxPtr>(_widget)->setBackgroungColour(utility::parseColour(_value));
		}

		void RenderBoxFactory::RenderBox_RotationAngle(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(RenderBoxPtr, _widget);
			static_cast<RenderBoxPtr>(_widget)->setRotationAngle(Ogre::Degree(utility::parseInt(_value)));
		}

		void RenderBoxFactory::RenderBox_MouseRotation(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(RenderBoxPtr, _widget);
			static_cast<RenderBoxPtr>(_widget)->setMouseRotation(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI
