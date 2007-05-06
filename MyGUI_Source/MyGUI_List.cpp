//=========================================================================================
//=========================================================================================
#include "MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace std;
using namespace OIS;
//=========================================================================================
namespace MyGUI {

	// ��� ����������� ��������
//	#define __LIST_ITEM_NONSELECT 0xFFFF

	class GUI;

	List::List(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, gui, uOverlay, pWindowFother),
		m_scroll(0),
		m_uSkinButton(0),
		m_uSizeYButton(1),
		m_uSizeXScroll(0),
		m_uCurrentFillSize(0),
		m_uStartWindow(0),
		m_uCountVisible(0),
		m_uOffsetDrawString(0),
		m_uSelectItem(ITEM_NON_SELECT),
		pWindowSelect(0),
		m_bIsFocus(false),
		m_bIsVisibleScroll(false),
		m_bIsOneClickActived(false)
	{
	}

	List::~List()
	{
		for (uint16 pos=0; pos <(uint16)m_aString.size(); pos++) delete m_aString[pos];
	}


	void List::onKeyButton(MyGUI::Window * pWindow, int keyEvent, char cText) // ������ �������
	{
		uint16 oldPos = m_uSelectItem;
		uint16 size = (uint16)m_aString.size();
		if (size == 0) return;

		if (keyEvent == KC_WEBFORWARD) { // ���������
			if (size <= m_uCountVisible) return;
			m_uOffsetDrawString ++;
			if ((m_uOffsetDrawString + m_uCountVisible) >= size) m_uOffsetDrawString = size - m_uCountVisible;
			redrawStrings();
			recalcScroll();
			return;
		} else if (keyEvent == KC_WEBBACK) { // ���������
			if (size <= m_uCountVisible) return;
			if (m_uOffsetDrawString > 0) m_uOffsetDrawString --;
			redrawStrings();
			recalcScroll();
			return;
		}

		if (keyEvent == KC_ESCAPE) { // ������ ������������ ��������
			if (m_pEventCallback) m_pEventCallback->onKeyButton(this, keyEvent, cText);
			return;
		}

		if (m_uSelectItem == ITEM_NON_SELECT) m_uSelectItem = 0;
		else if (keyEvent == KC_UP) {
			if (m_uSelectItem > 0) m_uSelectItem --;
		} else if (keyEvent == KC_DOWN) {
			if ((m_uSelectItem + 1) < size) m_uSelectItem ++;
		} else if (keyEvent == KC_HOME) {
			m_uSelectItem = 0;
		} else if (keyEvent == KC_END) {
			m_uSelectItem = size - 1;
		} else if (keyEvent == KC_PGUP) {
			int16 pos = (int16)m_uSelectItem - (int16)m_uCountVisible;
			if (pos < 0) m_uSelectItem = 0;
			else m_uSelectItem = (uint16)pos;
		} else if (keyEvent == KC_PGDOWN) {
			m_uSelectItem += m_uCountVisible;
			if (m_uSelectItem >= size) m_uSelectItem = size - 1;
		} else if ((keyEvent == KC_RETURN) || (keyEvent == KC_NUMPADENTER)) { // ������� ����
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_ACCEPT, m_uSelectItem);
			return;
		} else {
			// ����� ���� �����, ��� ������
			if (m_pEventCallback) m_pEventCallback->onKeyButton(this, keyEvent, cText);
			return; // ���������� �������
		}
		// ���� ������� �� �����, �� ��� ����������
		if (pWindowSelect) { // ����� ���������
			pWindowSelect->setState(WS_NORMAL);
			pWindowSelect = 0;
		}
		if ((m_uSelectItem >= m_uOffsetDrawString) && (m_uSelectItem < (m_uOffsetDrawString + m_uCountVisible))) {
			// ������� �����
			pWindowSelect = m_pWindowClient->m_aWindowChild[m_uSelectItem-m_uOffsetDrawString+m_uStartWindow];
			if (m_bIsFocus) pWindowSelect->setState(WS_PRESSED);
		} else {// ������� �� �����
			if (m_uSelectItem > (m_uCountVisible >> 1)) m_uOffsetDrawString = m_uSelectItem - (m_uCountVisible >> 1);
			else m_uOffsetDrawString = m_uSelectItem;
			if ((m_uOffsetDrawString + m_uCountVisible) >= size) m_uOffsetDrawString = size - m_uCountVisible;
			redrawStrings();
			recalcScroll();
		}
		// �������� ���������
		if ((m_pEventCallback) && (oldPos != m_uSelectItem)) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_CHANGE, m_uSelectItem);

	}

	void List::_OnKeyChangeFocus(bool bIsFocus) // ���������� ��� ����� ���������� �����
	{
		onKeyFocus(this, bIsFocus);
	}

	void List::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // ���������� ��� ������� ������� �����
	{
		onKeyButton(this, keyEvent, cText);
	}

	void List::onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) // ����� ������ �����
	{
		if (!bIsFocus) {
			if (pWindowSelect) { // ����� ���������
				pWindowSelect->setState(WS_NORMAL);
			}
			m_bIsFocus = false;
		} else {
			if (pWindowSelect) {
				pWindowSelect->setState(WS_PRESSED);
			}
			m_bIsFocus = true;
		}
		if (m_pEventCallback) m_pEventCallback->onKeyFocus(this, bIsFocus);
	}

	void List::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // ����� ������
	{
		if (bIsFocus) {
			if ((pWindow->m_uUserData + m_uOffsetDrawString) >= m_aString.size()) return;
			pWindow->showFocus(true);
		} else  pWindow->showFocus(false);
		
	}

	void List::onMouseClick(MyGUI::Window * pWindow)
	{
		uint16 oldPos = m_uSelectItem;

		if (pWindowSelect) pWindowSelect->setState(WS_NORMAL);
		uint16 item = pWindow->m_uUserData + m_uOffsetDrawString;
		if (item >= m_aString.size()) {
			m_uSelectItem = ITEM_NON_SELECT;
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_NONE, 0);
			return;
		}
		pWindowSelect = pWindow;
		pWindowSelect->setState(WS_PRESSED);
		m_uSelectItem = item;
		// �������� ���������
		if (m_pEventCallback) {
			if (oldPos != m_uSelectItem) {
				if (m_bIsOneClickActived) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_ACCEPT, m_uSelectItem);
				else  m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_CHANGE, m_uSelectItem);
			} else m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_NONE, 0); // ��� ��������� ��������� ��� ���������� ���� �����
		}
	}

	void List::onMouseDoubleClick(MyGUI::Window * pWindow) // ������� ������ ����
	{
		if (m_uSelectItem == ITEM_NON_SELECT) return;
		if (m_bIsOneClickActived) return; // ��� ���������� ��� ����������� ������
		if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_ACCEPT, m_uSelectItem);
	}

	void List::onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) // �������������� �������
	{
		if (uEvent == WOE_SCROLL_CHANGE_POS) {
			m_uOffsetDrawString = data;
			redrawStrings();
		}
	}

	void List::size(int16 iSizeX, int16 iSizeY) // �������� ������ ����
	{
		int16 oldSizeY = m_iSizeY;
		Window::size(iSizeX, iSizeY);

		if (oldSizeY == m_iSizeY) return; // ������ ������ �� ����������

		while (m_uCurrentFillSize < m_pWindowClient->m_iSizeY) {
			int16 cutSize = m_uSizeXScroll;
			if (!m_bIsVisibleScroll) cutSize = 0; // �������� ������ �� ����
			Window * pChild = m_pWindowClient->createWindow(0, m_uCurrentFillSize, m_pWindowClient->m_iSizeX - cutSize, m_uSizeYButton, WA_TOP|WA_HSTRETCH, m_uSkinButton);
			pChild->m_pEventCallback = (EventCallback *)this;
			pChild->m_overlayContainer->hide();
			pChild->setFont(m_font, m_fontColour);
			pChild->m_uUserData = ((uint16)m_pWindowClient->m_aWindowChild.size()-1) - m_uStartWindow; // ���������� ����� ����
			m_uCurrentFillSize += m_uSizeYButton;
		};

		uint8 oldCountVisible = m_uCountVisible;
		m_uCountVisible = 0;

		uint8 size = (uint8)m_pWindowClient->m_aWindowChild.size();
		for (uint8 pos = m_uStartWindow; pos<size; pos++) {
			Window * pWindow = m_pWindowClient->m_aWindowChild[pos];
			if ((pWindow->m_iPosY + m_uSizeYButton) > m_pWindowClient->m_iSizeY) { // �� �������
				if (pWindow->m_overlayContainer->isVisible()) pWindow->m_overlayContainer->hide();
				else pos = size; // ������� �� �����
			} else {
				m_uCountVisible ++;
				if (!pWindow->m_overlayContainer->isVisible()) {
					pWindow->m_overlayContainer->show(); // ��������
					if ((pWindow->m_uUserData + m_uOffsetDrawString) == m_uSelectItem) {
						pWindowSelect = pWindow;
						if (m_bIsFocus) pWindowSelect->setState(WS_PRESSED);
					}
				}
			}
		}
	
		if (oldCountVisible >= m_uCountVisible) { // ����������� ����� �� �����������
			if (oldCountVisible != m_uCountVisible) recalcScroll();
			return;
		}
		if (m_aString.size() == 0) {
			recalcScroll();
			return;
		}

		int16 visible = (int16)m_aString.size() - m_uOffsetDrawString;
		if (visible < m_uCountVisible) { // ���� ������ ����� �����
			if (m_uOffsetDrawString != 0) { // � ������� �� �������
				visible = (int16)m_uOffsetDrawString - ((int16)m_uCountVisible - visible);
				if (visible < 0) visible = 0;
				m_uOffsetDrawString = visible;
				redrawStrings();
			}
		} else {
			for (uint16 draw = oldCountVisible; draw<m_uCountVisible; draw++) {
				Window * pWindow = m_pWindowClient->m_aWindowChild[draw+m_uStartWindow];
				pWindow->m_pWindowText->setWindowText(*m_aString[draw+m_uOffsetDrawString]);
			}
		}

		recalcScroll();

	}

	void List::recalcScroll() // �������� ������ ���������
	{
		if (!m_scroll) return;
		if (m_uCountVisible == 0) return;

		uint16 size = (uint16)m_aString.size();
		if (m_uCountVisible >= size) {
			m_scroll->setScrollRange(0);
			if (m_uExData & WES_LIST_HIDE_SCROLL) { // �������� , ���������� ������
				if (m_bIsVisibleScroll) {
					m_scroll->show(false);
					m_bIsVisibleScroll = false;

					uint8 size = (uint8)m_pWindowClient->m_aWindowChild.size();
					for (uint8 pos = m_uStartWindow; pos<size; pos++) {
						Window * pWindow = m_pWindowClient->m_aWindowChild[pos];
						pWindow->size(pWindow->m_iSizeX+m_uSizeXScroll, pWindow->m_iSizeY);
					}

				}
			}
			return;
		}
		// ���������� ������� ������
		m_scroll->setScrollRange(size - m_uCountVisible + 1);
		m_scroll->setScrollPos(m_uOffsetDrawString);

		if (m_uExData & WES_LIST_HIDE_SCROLL) { // �������� , ���������� ������
			if (!m_bIsVisibleScroll) {
				m_scroll->show(true);
				m_bIsVisibleScroll = true;

				uint8 size = (uint8)m_pWindowClient->m_aWindowChild.size();
				for (uint8 pos = m_uStartWindow; pos<size; pos++) {
					Window * pWindow = m_pWindowClient->m_aWindowChild[pos];
					pWindow->size(pWindow->m_iSizeX-m_uSizeXScroll, pWindow->m_iSizeY);
				}

			}
		}
	}

	void List::redrawStrings() // �������������� ��� ������
	{
		if (pWindowSelect) { // ����� ���������
			pWindowSelect->setState(WS_NORMAL);
			pWindowSelect = 0;
		}

		uint16 size = (uint16)m_aString.size();
		uint16 index = m_uStartWindow;
		uint8 visible = 0;
		for (uint16 pos=m_uOffsetDrawString; pos<size; pos++) {
			Window * pWindow = m_pWindowClient->m_aWindowChild[index];
			pWindow->m_pWindowText->setWindowText(*m_aString[pos]);
			if (pos == m_uSelectItem) {
				pWindowSelect = pWindow;
				if (m_bIsFocus) pWindowSelect->setState(WS_PRESSED);
			}
			index ++;
			visible ++;
			if (visible >= m_uCountVisible) return;
		}

		// ������ �����
		for (uint16 pos=size+1; pos<m_uCountVisible; pos++) {
			m_pWindowClient->m_aWindowChild[pos]->setWindowText("");
		}

	}

	void List::addString(const DisplayString & strElement, bool bIsRedraw) // �������� ������ � ������
	{
		m_aString.push_back(new DisplayString(strElement));
		if (bIsRedraw) {
			redrawStrings();
			recalcScroll();
		}
	}

	void List::deleteString(uint16 index, bool bIsRedraw) // ������� ������ �� ������
	{
		uint16 size = (uint16)m_aString.size();
		if (size == 0) return;
		if (index >= size) return;
		delete m_aString[index];
		for (uint16 pos=index+1; pos<size; pos++) {
			m_aString[pos-1] = m_aString[pos];
		}
		m_aString.pop_back();
		if (bIsRedraw) {
			redrawStrings();
			recalcScroll();
		}
	}

	void List::deleteStringAll(bool bIsRedraw) // ������� ��� ������
	{
		m_uSelectItem = ITEM_NON_SELECT; // ���������� ���������� �������
		uint16 size = (uint16)m_aString.size();
		if (size == 0) return;
		for (uint16 pos=0; pos<size; pos++) delete m_aString[pos];
		m_aString.clear();
		if (bIsRedraw) {
			redrawStrings();
			recalcScroll();
		}
	}

	void List::insertString(uint16 index, const DisplayString & strElement, bool bIsRedraw) // �������� ������ � ������
	{
		m_aString.push_back(0);
		int16 size = (int16)m_aString.size();
		if (index >= size) index = size - 1;

		for (int16 pos=size-1; pos>index; pos--) {
			m_aString[pos] = m_aString[pos-1];
		}
		m_aString[index] = new DisplayString(strElement);

		if (bIsRedraw) {
			redrawStrings();
			recalcScroll();
		}
	}

	const DisplayString & List::getString(uint16 index) // ���������� ������
	{
		if (index >= (int16)m_aString.size()) {
			// �������� ������� ��� �� �������, ����� ������ ��� ���������
			static const DisplayString strEmpty("");
			return strEmpty;
		}
		return *m_aString[index];
	}

	void List::setFont(__LP_MYGUI_FONT_INFO lpFont, uint32 colour) // ������������� ����� ��� ����� ������
	{
		// ���������� ����������
		m_font = lpFont;
		m_fontColour = colour;
		// ����������� ��� ������������ ��������
		for (uint16 pos=m_uStartWindow; pos<(uint16)m_pWindowClient->m_aWindowChild.size(); pos++) {
			m_pWindowClient->m_aWindowChild[pos]->setFont(lpFont, colour);
		}
	}

	MyGUI::List * Window::createList(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback)
	{
		__ASSERT(uSkin < __SKIN_COUNT); // ����
		__LP_MYGUI_WINDOW_INFO pSkin = m_GUI->m_windowInfo[uSkin];
		List * pWindow = new List(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, m_pWindowClient);
		pWindow->m_uAligin |= uAligin;
		pWindow->m_font = pSkin->fontWindow;
		pWindow->m_fontColour = pSkin->colour;
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // ������� �������� ���� �����
			Window *pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild;
		}

		if (__WINDOW_DATA3(pSkin)) { // ���� ������
			uint8 uSkinScroll = __WINDOW_DATA3(pSkin);
			__ASSERT(uSkinScroll < __SKIN_COUNT); // ����
			pWindow->m_uSizeXScroll = m_GUI->m_windowInfo[uSkinScroll]->subSkins[0]->sizeX;
			pWindow->m_scroll = pWindow->m_pWindowClient->createVScroll(pWindow->m_pWindowClient->m_iSizeX-pWindow->m_uSizeXScroll, 0, -1, pWindow->m_pWindowClient->m_iSizeY, WA_RIGHT|WA_VSTRETCH, __WINDOW_DATA3(pSkin));
			pWindow->m_uStartWindow = 1;
			pWindow->m_scroll->m_pEventCallback = (EventCallback*)pWindow;
			pWindow->m_bIsVisibleScroll = true;
		}
		pWindow->m_uSkinButton = __WINDOW_DATA4(pSkin);
		__ASSERT(pWindow->m_uSkinButton < __SKIN_COUNT); // ����
		pWindow->m_uSizeYButton = m_GUI->m_windowInfo[pWindow->m_uSkinButton]->subSkins[0]->sizeY;
		if (pWindow->m_uSizeYButton == 0) pWindow->m_uSizeYButton = 1;

		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

	MyGUI::List * GUI::createList(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback)
	{
		__ASSERT(uSkin < __SKIN_COUNT); // ����
		__LP_MYGUI_WINDOW_INFO pSkin = m_windowInfo[uSkin];
		List * pWindow = new List(pSkin->subSkins[0], this, uOverlay, 0);
		pWindow->m_font = pSkin->fontWindow;
		pWindow->m_fontColour = pSkin->colour;
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // ������� �������� ���� �����
			Window *pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild;
		}

		if (__WINDOW_DATA3(pSkin)) { // ���� ������
			uint8 uSkinScroll = __WINDOW_DATA3(pSkin);
			__ASSERT(uSkinScroll < __SKIN_COUNT); // ����
			pWindow->m_uSizeXScroll = m_windowInfo[uSkinScroll]->subSkins[0]->sizeX;
			pWindow->m_scroll = pWindow->m_pWindowClient->createVScroll(pWindow->m_pWindowClient->m_iSizeX-pWindow->m_uSizeXScroll, 0, -1, pWindow->m_pWindowClient->m_iSizeY, WA_RIGHT|WA_VSTRETCH, __WINDOW_DATA3(pSkin));
			pWindow->m_uStartWindow = 1;
			pWindow->m_scroll->m_pEventCallback = (EventCallback*)pWindow;
			pWindow->m_bIsVisibleScroll = true;
		}
		pWindow->m_uSkinButton = __WINDOW_DATA4(pSkin);
		__ASSERT(pWindow->m_uSkinButton < __SKIN_COUNT); // ����
		pWindow->m_uSizeYButton = m_windowInfo[pWindow->m_uSkinButton]->subSkins[0]->sizeY;
		if (pWindow->m_uSizeYButton == 0) pWindow->m_uSizeYButton = 1;

		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

}
//=========================================================================================
