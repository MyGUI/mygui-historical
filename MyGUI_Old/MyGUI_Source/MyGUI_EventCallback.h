#pragma once

#include "MyGUI_Defines.h"

namespace MyGUI
{
    //This could be implemented as a pure virtual, but you might not want to handle all the functions listed
    class EventCallback // ������� ����� ��� �������
	{
	public:
		virtual void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed) {}; // ������ ����� ������ ����
		virtual void onMouseClick(MyGUI::Window * pWindow) {}; // ������ � �������� ����� ������ ���� �� ���� �� ��������
		virtual void onMouseDoubleClick(MyGUI::Window * pWindow) {}; // ������� ������ ����
		virtual void onMouseSetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld) {}; // ����� ������
		virtual void onMouseLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew) {}; // ����� ������
		virtual void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY) {}; // ����������� � ��������, �� �� ��������

		virtual void onKeySetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld) {}; // ����� ������ �����
		virtual void onKeyLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew) {}; // ����� ������ �����
		virtual void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText) {}; // ������ �������

		virtual void onWarningEvent(MyGUI::Window * pWindow, uint16 uEvent) {}; // ������� ��������������� ������
		virtual void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) {}; // �������������� �������
		virtual void onFadeEnd(bool bIsFade, uint8 fadeID) {}; // ����������� ����������
	};
}