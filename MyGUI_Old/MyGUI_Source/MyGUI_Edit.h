#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Edit : public Window {
        Edit(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);
	public:
		static Edit *Edit::createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_EDIT);

		void _OnMouseSetFocus(MyGUI::Window * pWindowOld); // ���������� ��� ����� ���������� �� �������
		void _OnMouseLostFocus(MyGUI::Window * pWindowNew); // ���������� ��� ����� ���������� �� �������
		void _OnKeySetFocus(MyGUI::Window * pWindowOld); // ���������� ��� ����� ���������� �����
		void _OnKeyLostFocus(MyGUI::Window * pWindowNew); // ���������� ��� ����� ���������� �����
		void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		virtual Window *setCaption(const DisplayString & strText); // ������������� ����� ����
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����

		Window *m_pWindowCursor; // ���� ������ �������
		bool m_bIsFocus; // ���� � ������
		uint8 m_uOffsetCursor; // �������� ������� �� ������
	};

}