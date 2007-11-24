
#include "HScrollFactory.h"
#include "HScroll.h"
#include "SkinManager.h"
#include "WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		HScrollFactory::HScrollFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
		}

		HScrollFactory::~HScrollFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
		}

		const Ogre::String& HScrollFactory::getType()
		{
			static Ogre::String type("HScroll");
			return type;
		}

		WidgetPtr HScrollFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new HScroll(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

	} // namespace factory
} // namespace MyGUI