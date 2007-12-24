/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_WidgetFactory.h"
#include "MyGUI_Widget.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		WidgetFactory::WidgetFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("Widget_Caption") = newDelegate(this, &WidgetFactory::Widget_Caption);
			manager.registerDelegate("Widget_Move") = newDelegate(this, &WidgetFactory::Widget_Move);
			manager.registerDelegate("Widget_Size") = newDelegate(this, &WidgetFactory::Widget_Size);
			manager.registerDelegate("Widget_Show") = newDelegate(this, &WidgetFactory::Widget_Show);
			manager.registerDelegate("Widget_Colour") = newDelegate(this, &WidgetFactory::Widget_Colour);
			manager.registerDelegate("Widget_FontName") = newDelegate(this, &WidgetFactory::Widget_FontName);
			manager.registerDelegate("Widget_FontHeight") = newDelegate(this, &WidgetFactory::Widget_FontHeight);
			manager.registerDelegate("Widget_Alpha") = newDelegate(this, &WidgetFactory::Widget_Alpha);
			manager.registerDelegate("Widget_State") = newDelegate(this, &WidgetFactory::Widget_State);
			manager.registerDelegate("Widget_NeedKey") = newDelegate(this, &WidgetFactory::Widget_NeedKey);
			manager.registerDelegate("Widget_AlignText") = newDelegate(this, &WidgetFactory::Widget_AlignText);
		}

		WidgetFactory::~WidgetFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("Widget_Caption");
			manager.unregisterDelegate("Widget_Move");
			manager.unregisterDelegate("Widget_Size");
			manager.unregisterDelegate("Widget_Show");
			manager.unregisterDelegate("Widget_Colour");
			manager.unregisterDelegate("Widget_FontName");
			manager.unregisterDelegate("Widget_FontHeight");
			manager.unregisterDelegate("Widget_Alpha");
			manager.unregisterDelegate("Widget_State");
			manager.unregisterDelegate("Widget_NeedKey");
			manager.unregisterDelegate("Widget_AlignText");
		}

		const Ogre::String& WidgetFactory::getType()
		{
			return Widget::getType();
		}

		WidgetPtr WidgetFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new Widget(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void WidgetFactory::Widget_Caption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setCaption(_value);
		}

		void WidgetFactory::Widget_Move(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() == 4) _widget->setPosition(util::parseInt(vec[0]), util::parseInt(vec[1]), util::parseInt(vec[2]), util::parseInt(vec[3]) );
			else if (vec.size() == 2) _widget->setPosition( util::parseInt(vec[0]), util::parseInt(vec[1]) );
		}

		void WidgetFactory::Widget_Size(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() == 2) _widget->setSize( util::parseInt(vec[0]), util::parseInt(vec[1]) );
		}

		void WidgetFactory::Widget_Show(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			if (util::parseBool(_value)) _widget->show();
			else _widget->hide();
		}

		void WidgetFactory::Widget_Colour(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			const std::vector<std::string> & vec = util::split(_value);
			if (vec.size() > 2) _widget->setColour(Ogre::ColourValue(util::parseFloat(vec[0]), util::parseFloat(vec[1]), util::parseFloat(vec[2])) );
		}

		void WidgetFactory::Widget_FontName(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setFontName(_value);
		}

		void WidgetFactory::Widget_FontHeight(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setFontHeight(util::parseFloat(_value));
		}

		void WidgetFactory::Widget_Alpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setAlpha(util::parseFloat(_value));
		}

		void WidgetFactory::Widget_State(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setState(_value);
		}

		void WidgetFactory::Widget_NeedKey(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setNeedKeyFocus(util::parseBool(_value));
		}

		void WidgetFactory::Widget_AlignText(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			_widget->setTextAlign(SkinManager::getInstance().parseAlign(_value));
		}

	} // namespace factory
} // namespace MyGUI