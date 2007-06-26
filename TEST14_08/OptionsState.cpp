#include "BasisManager.h"
#include "MyGUI_Source//MyGUI.h"

using namespace Ogre;
using namespace MyGUI;

void OptionsState::onMouseClick(MyGUI::Window * pWindow) // ������ � �������� ����� ������ ���� �� ���� �� ��������
{
    if (pWindow == m_buttonExit) {
		MyGUI::GUI::getSingleton()->createMessage(
		    "Message", "Do you really want to exit?", MESSAGE_ID_EXIT, true, "Ok", "Cancel");
	} else if (pWindow == m_buttonEditor) {
		if (!mEditor) {
			mEditor = new SkinEditor::SkinEditor(this);
			m_buttonEditor->hide();
		}
	}

}
//===================================================================================
void OptionsState::onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) // �������������� �������
{
	if (uEvent == MyGUI::WOE_MESSAGE_PRESS_BUTTON) {
		if (pWindow->m_uUserData == MESSAGE_ID_EXIT) {
			if (data == MyGUI::MBB_BUTTON1)
			    BasisManager::getSingleton()->m_exit = true;
		}
	} else if (uEvent == WOE_COMBO_SELECT_ACCEPT) {
		if (data < m_straColour.size())
			BasisManager::getSingleton()->mWallpaperOverlay->getChild("wallpaper")->setMaterialName(m_straColour[data]);
	} else if (uEvent == WOE_FRAME_CLOSE) {
		m_buttonEditor->show();
		delete mEditor;
		mEditor = 0;
	}
}
//===================================================================================
void OptionsState::enter(bool bIsChangeState)
{
	m_straColour.push_back("BACK_BLACK");
	m_straColour.push_back("BACK_WHITE");
	m_straColour.push_back("BACK_GRAY");
	m_straColour.push_back("BACK_GREEN");
	m_straColour.push_back("BACK_YELLOW");

	BasisManager::getSingleton()->mWallpaperOverlay->getChild("wallpaper")->setMaterialName(m_straColour.front());

	m_comboBackground = static_cast<MyGUI::ComboBox*> (GUI::getSingleton()->createWidget(
		MyGUI::WIDGET_COMBO_BOX, MyGUI::SKIN_DROP_LIST, (GUI::getSingleton()->getWidth()/2) - 100, 10, 200, -1, OVERLAY_MAIN) );
	m_comboBackground->m_pEventCallback = this;
	
	for (uint8 pos=0; pos<m_straColour.size(); pos++)
	    m_comboBackground->addString(m_straColour[pos]);
	
	m_comboBackground->setString(0);
    
	m_buttonExit = static_cast<MyGUI::Button *> (GUI::getSingleton()->createWidget(
		MyGUI::WIDGET_BUTTON, MyGUI::SKIN_BUTTON, 10, 10, 150, -1, MyGUI::OVERLAY_MAIN) );
	m_buttonExit->setCaption("Exit");
    
	m_buttonEditor = static_cast<MyGUI::Button *> (GUI::getSingleton()->createWidget(
		MyGUI::WIDGET_BUTTON, MyGUI::SKIN_BUTTON, GUI::getSingleton()->getWidth() - 160, 10, 150, -1, MyGUI::OVERLAY_MAIN) );
	m_buttonEditor->setCaption("Editor");
	m_buttonEditor->hide();
    
	mEditor = new SkinEditor::SkinEditor(this);


	OverlayManager &overlayManager = OverlayManager::getSingleton();

	Overlay * overlay;
	overlay = overlayManager.create("WidgetOverlay");
	overlay->setZOrder(200);
	overlay->show();

	m_widgetParent = new Widget(170, 170, 200, 200, 0);
	m_widgetParent->createOverlay(overlay, 0, "BACK_GREEN");

	m_widgetChild = m_widgetParent ->createChild(0, 0, 100, 100);
	m_widgetChild->createOverlay(0, m_widgetParent->m_overlayContainer, "MyGUI_button");

	m_widgetSubChild = m_widgetChild ->createChild(0, 0, 60, 60);
	m_widgetSubChild->createOverlay(0, m_widgetChild->m_overlayContainer, "MyGUI_button");
}
//===================================================================================
bool OptionsState::mouseMoved( const OIS::MouseEvent &arg )
{
	if(toggle)
		m_widgetChild->move(arg.state.X.abs-220, arg.state.Y.abs-220);
	else
		m_widgetSubChild->move(arg.state.X.abs-220, arg.state.Y.abs-220);
	return true;
}
//===================================================================================
bool OptionsState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	toggle = 1 - toggle;
	return true;
}
//===================================================================================
void OptionsState::exit()
{
	GUI::getSingleton()->destroyWindow(m_comboBackground);
	GUI::getSingleton()->destroyWindow(m_buttonExit);
	GUI::getSingleton()->destroyWindow(m_buttonEditor);

	if (mEditor) {
		delete mEditor;
		mEditor = 0;
	}
}
//===================================================================================
void OptionsState::windowResize() // ����������� �� ��������� �������� ���� �������
{
	m_buttonEditor->move(GUI::getSingleton()->getWidth() - 160, 10);
	m_comboBackground->move((GUI::getSingleton()->getWidth()/2) - 100, 10);
}
//===================================================================================
