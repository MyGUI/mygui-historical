
#include "BasisManager.h"

#include "Gui.h"
#include "LayerManager.h"
#include "PointerManager.h"
#include "SkinManager.h"
#include "WidgetManager.h"
#include "LayoutManager.h"
#include "ParserManager.h"
#include "MyGUI_FontManager.h"
#include "Edit.h"
//#include "Window.h"
//#include "Button.h"
//#include "HScroll.h"
//#include "VScroll.h"
//#include "List.h"
#include "utility.h"

void OptionsState::enter(bool bIsChangeState)
{

	MyGUI::Gui::getInstance().initialise(BasisManager::getInstance().mWindow);

	MyGUI::LayerManager::getInstance().load("main.layer");
	MyGUI::PointerManager::getInstance().load("main.pointer");
	MyGUI::PointerManager::getInstance().show();

	// ��������� ����
	MyGUI::SkinManager::getInstance().load("main.skin");
	// ��������� ������
	//MyGUI::LayoutManager::getInstance().load("mygui.layout");

	MyGUI::FontManager::getInstance().load("MyGUI.font");


	//MyGUI::ButtonPtr button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonAutoAlpha"));
	//button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed1);

	/*button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonManualAlpha"));
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed2);*/


/*	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::WidgetManager::getInstance().findWidget("windowListDemo"));
	window->show(false);
	//MyGUI::ParserManager::getInstance().parce(window, "Caption", util::utf8<void>("������"));

	button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonLeft"));
	button->setCaption("<<");
	button->setUserData("button", 1);
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed3);

	button = static_cast<MyGUI::ButtonPtr>(MyGUI::WidgetManager::getInstance().findWidget("buttonRight"));
	button->setCaption(">>");
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed3);

	list1 = static_cast<MyGUI::ListPtr>(MyGUI::WidgetManager::getInstance().findWidget("list1"));
	list2 = static_cast<MyGUI::ListPtr>(MyGUI::WidgetManager::getInstance().findWidget("list2"));*/

	MyGUI::ButtonPtr button = static_cast<MyGUI::ButtonPtr>(MyGUI::Gui::getInstance().createWidget("Button", "Button", 10, 10, 200, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	button->setCaption(L"#00FF00������\t=)");

	MyGUI::EditPtr edit = static_cast<MyGUI::EditPtr>(MyGUI::Gui::getInstance().createWidget("Edit", "Edit", 200, 50, 300, 70, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	edit->setCaption(L"�� ������ ��� � ������");

	/*Ogre::FontPtr mpFont = Ogre::FontManager::getSingleton().getByName("MyGUI_font");
	if (!mpFont.isNull()) {
		const Ogre::MaterialPtr & material = mpFont->getMaterial();
		Ogre::Pass * pass = material->getTechnique(0)->getPass(0);
		pass->createTextureUnitState("char_select.png", 1);
		pass->getTextureUnitState(1)->setColourOperationEx(LBX_SOURCE1, LBS_CURRENT, LBS_CURRENT);
		pass->getTextureUnitState(1)->setAlphaOperation(LBX_BLEND_TEXTURE_ALPHA, LBS_TEXTURE, LBS_CURRENT);
	}*/

/*	Ogre::OverlayManager & manager = Ogre::OverlayManager::getSingleton();
	Ogre::Overlay * overlay = manager.create("test1");
	overlay->setZOrder(100);
	overlay->show();
	Ogre::OverlayElement * element = manager.createOverlayElement("Panel", "test2");
	element->setMaterialName("test");
	element->setPosition(0.1, 0.1);
	element->setDimensions(0.5, 0.5);
	overlay->add2D((Ogre::OverlayContainer*)element);*/

}
//===================================================================================
bool OptionsState::mouseMoved( const OIS::MouseEvent &arg )
{
	MyGUI::InputManager::getInstance().injectMouseMove(arg);
	return true;
}
//===================================================================================
bool OptionsState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MyGUI::InputManager::getInstance().injectMousePress(arg, id);
	return true;
}
//===================================================================================
bool OptionsState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MyGUI::InputManager::getInstance().injectMouseRelease(arg, id);
	return true;
}
//===================================================================================
bool OptionsState::keyPressed( const OIS::KeyEvent &arg )
{
	MyGUI::InputManager::getInstance().injectKeyPress(arg);
	return true;
}
//===================================================================================
bool OptionsState::keyReleased( const OIS::KeyEvent &arg )
{
	MyGUI::InputManager::getInstance().injectKeyRelease(arg);
	return true;
}
//===================================================================================
void OptionsState::exit()
{
	MyGUI::Gui::getInstance().shutdown();
}
//===================================================================================
void OptionsState::windowResize() // ����������� �� ��������� �������� ���� �������
{
}
//===================================================================================
void OptionsState::notifyMousePressed1(MyGUI::WidgetPtr _sender, bool _left)
{
//	int x = (::rand()%600) + 20;
//	int y = (::rand()%200) + 20;
//	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "WindowCSX", x, y, 200, 100, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
//	window->setCaption("Auto alpha");
//	window->eventWindowXPressed = MyGUI::newDelegate(this, &OptionsState::notifyWindowXPressed);
}
//===================================================================================
void OptionsState::notifyMousePressed2(MyGUI::WidgetPtr _sender, bool _left)
{
//	int x = (::rand()%600) + 20;
//	int y = (::rand()%200) + 320;
//	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "WindowCSX", x, y, 200, 100, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
//	window->setAutoAlpha(false);
//	window->show();
//	window->setCaption("Manual alpha");
//	window->eventWindowXPressed = MyGUI::newDelegate(this, &OptionsState::notifyWindowXPressed);
}
//===================================================================================
void OptionsState::notifyMousePressed3(MyGUI::WidgetPtr _sender, bool _left)
{
//	if (_sender->getUserData("button") == 1) {
//
//		size_t pos2 = list2->getItemSelect();
//		if (pos2 != MyGUI::Widget::ITEM_NONE) {
//			size_t pos1 = list1->getItemSelect();
//
//			if (pos1 == MyGUI::Widget::ITEM_NONE) list1->addItemString(list2->getItemString(pos2));
//			else list1->insertItemString(pos1, list2->getItemString(pos2));
//
//			list2->deleteItemString(pos2);
//		}
//		
//	} else {
//		size_t pos1 = list1->getItemSelect();
//		if (pos1 != MyGUI::Widget::ITEM_NONE) {
//			size_t pos2 = list2->getItemSelect();
//
//			if (pos2 == MyGUI::Widget::ITEM_NONE) list2->addItemString(list1->getItemString(pos1));
//			else list2->insertItemString(pos2, list1->getItemString(pos1));
//
//			list1->deleteItemString(pos1);
//		}
//	}
}
//===================================================================================
void OptionsState::notifyWindowXPressed(MyGUI::WidgetPtr _sender)
{
//	static_cast<MyGUI::WindowPtr>(_sender)->hide(true, true);
}
//===================================================================================
