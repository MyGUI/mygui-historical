#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_EventCallback.h"
#include "MyGUI_Skin.h"
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>

namespace MyGUI {

    using Ogre::DisplayString;

	class Window : public EventCallback { // ������� ����� ���� ���

	public:
		Window(const __tag_MYGUI_SUBSKIN_INFO *lpSkin, uint8 uOverlay, Window *pWindowParent);		
	
        virtual ~Window();
	    
	    static Window *Window::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_WINDOWFRAME);
	    
	    template <typename Widget> Widget *spawn(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
            uint16 uAlign, const String &Skin = __SKIN_WIDGET_DEFAULT)
        {
            if(Skin == __SKIN_WIDGET_DEFAULT)
                return Widget::create(PosX, PosY, SizeX, SizeY, this, uAlign, 0);
            else
                return Widget::create(PosX, PosY, SizeX, SizeY, this, uAlign, 0, Skin);
        }
        
        template <typename Widget> Widget *spawnReal(Real PosX, Real PosY, Real SizeX, Real SizeY,
            uint16 uAlign, const String &Skin = __SKIN_WIDGET_DEFAULT)
        {
            uint16 NewPosX  = PosX  * GUI::getSingleton()->m_uWidth;
            uint16 NewPosY  = PosY  * GUI::getSingleton()->m_uHeight;
            uint16 NewSizeX = SizeX * GUI::getSingleton()->m_uWidth;
            uint16 NewSizeX = SizeX * GUI::getSingleton()->m_uHeight;
                
            return spawn<Widget>(NewPosX, NewPosY, NewSizX, NewSizeY, uAlign, uSkin);
        }
            
        virtual void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		virtual void _OnMouseMove(int16 iPosX, int16 iPosY); // ���������� ��� �������� ����
		virtual void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // ���������� ��� ������� �������
		virtual void _OnMouseButtonClick(bool bIsDouble); // ���������� ��� ������� ������� � ���������� �� ��� �� ��������
		virtual void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����
//		virtual void _OnKeyButtonClick(int keyEvent); // ���������� ��� ���������� ������� �����
		virtual void _OnKeyChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �����
		virtual void _OnUpZOrder() {}; // ���������� ��� ��������� ����
		
		void show(bool bIsShow); // ������ �������� ����
		void move(int16 iPosX, int16 iPosY); // ����������� ����
		virtual void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		bool check(int16 iPosX, int16 iPosY, bool bCapture = false); // �������� ���� �� ������� �������
		void setState(uint8 uState); // ��������� ����
		uint8 getState(); // ��������� ����
		void showFocus(bool bIsFocus); // ������������� ����
		virtual void setWindowText(const DisplayString & strText); // ������������� ����� ����
		const DisplayString & getWindowText(); // ���������� ������ ����
		virtual void alignWindowText(); // ��������� ������������ ������
		void alignWindow(int16 rNewSizeX, int16 rNewSizeY); // ����������� ���� ������������ ����
		void setFont(const String &Font, Ogre::ColourValue colour); // ������������� ����� ��� ��������
		
		private:
		void setFont(const __tag_MYGUI_FONT_INFO *lpFont, Ogre::ColourValue colour); // ������������� ����� ��� ��������		
        
        public:
		void addEvent(uint16 addEvent) {m_uEventCallback |= addEvent;}; // ��������� ������� �� ������� ���� �����������

		inline void setUserString(const DisplayString & strUserString) {m_strUserString = strUserString;}; // ������ ������ ������������
		inline DisplayString & getUserString() {return m_strUserString;}; // ���������� ������ ������������
		inline void setUserData(uint32 uUserData) {m_uUserData = uUserData;}; // ������ ������ ������������
		inline uint32 getUserData() {return m_uUserData;}; // ���������� ������ ������������

		Ogre::Overlay* m_overlay; // ������� ����� ����
		Ogre::PanelOverlayElement * m_overlayContainer; // ������� ��������
		Ogre::OverlayContainer* m_overlayCaption; // ������� ������
		
		
		/*
		    Child windows of this window.  Each window is actually made up of a main window
		    and anywhere from 0 to many child windows, which define things such as frames and other
		    miscellaneous graphics.
		*/
		std::vector<MyGUI::Window*> mChildWindows;
		typedef std::vector<MyGUI::Window*>::iterator ChildWindowsIterator;	
		
		Window * m_pWindowParent; // ���� ����
		Window * m_pWindowText; // ������� ����� ���������� ����� ����� �������� (�� ������� this)
		Window * m_pWindowClient; // ������� ����� �������� ��������� ����� ����� �������� (�� ������� this)
		int16 m_iPosX, m_iPosY, m_iSizeX, m_iSizeY; // ������� ����
		int16 m_iOffsetAlignX, m_iOffsetAlignY; // ��������, ������������ ������ ��� ������������ �� ������ ��� ����������

		uint8 m_uState; // ������ ����
		EventCallback *m_pEventCallback; // ��������� �� ����� ��� ������ �������
		uint16 m_uEventCallback; // ����� ��� ������� �������
		uint16 m_uAlign; // ������������ ���� � ������
		int16 m_sizeTextX; // ������ ������ ����
		int16 m_sizeTextY; // ������ ������ ����
		int16 m_sizeCutTextX; // ������� ������ ������
		int16 m_sizeCutTextY; // ������� ������ ������
		DisplayString m_strWindowText; // ����� ���� ��� ������ �������, ����� ����� �� ������� ��� ������ �����
		bool m_bIsOverlapped; // ���� ���������������
		bool m_bIsTextShiftPressed; // ������� �� ����� ����

		String m_font; // ����� ����
		Ogre::ColourValue m_fontColour; // ���� ������
		String m_paStrSkins[__SKIN_STATE_COUNT]; // ����� ���������
		uint16 m_uExData; // �������������� ���������� �� ��������

		DisplayString m_strUserString; // ������ ��� �������������� ���������� ������������
		uint32 m_uUserData; // �������������� ���������� ������������

	};
}