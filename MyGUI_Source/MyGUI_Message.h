//=========================================================================================
#ifndef __MyGUI_Message_H__
#define __MyGUI_Message_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class WindowFrame;
	class Button;
	class GUI;

	class Message : public WindowFrame {

	public:
		Message(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);

		void _OnUpZOrder(); // ���������� ��� ��������� ����

		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, char cText); // ������ �������

		Button * m_pButton1; // ������ ������
		Button * m_pButton2; // ������ ������
		Window * m_pWindowFide; // ���� ����������

	};

}
//=========================================================================================
#endif
//=========================================================================================
