#pragma once

#include "BasisState.h"
#include "SkinEditor.h"

class OptionsState : public BasisState
{
public:
	enum {
		NONE,
		MESSAGE_ID_EXIT,
	};
public:

	void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
	void onOtherEvent(MyGUI::Window * pWindow, MyGUI::uint16 uEvent, MyGUI::uint32 data); // �������������� �������

	MyGUI::Button * m_buttonExit;
	MyGUI::Button * m_buttonEditor;

	MyGUI::ComboBox * m_comboBackground; // ���
	std::vector <std::string> m_straColour;

	SkinEditor::SkinEditor * mEditor; // �������� ������

public:
	void enter(bool bIsChangeState);
	void exit();
	void windowResize(); // ����������� �� ��������� �������� ���� �������

};