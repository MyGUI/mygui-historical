#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Button : public Window {
        Button(const __MYGUI_SUBSKIN_INFO * lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);
	public:
		static Button *create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_BUTTON);
	        
		// ��� ���������� ������� �����
		void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // ���������� ��� ������� �������
		// � ��� ��� ������
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed); // ������ ����� ������ ����

        //Called when the user clicks on a button
        private:
		void showPressed(bool bIsPressed); // ����� �� �� ����� �������

	};

}