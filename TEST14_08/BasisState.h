//=====================================================================================
#pragma once
//=====================================================================================
#include "OIS\\OIS.h"
#include "MyGUI_Source\\MyGUI.h"
//#include "GameData.h"
//=====================================================================================
// ������� ����� ��� ���� �������
//===================================================================================
class BasisState : public MyGUI::EventCallback
{
public:
	virtual void enter(bool bIsChangeState) = 0; // �����������, � ���� ������ �� ������ ������� ��� ���� ������ ��� ������ ������
	virtual void exit() = 0; // �����������, � ���� ������ �� ������ ���������� ��� ��� ���� �������� � ������ enter()
	virtual void pause() {}; // �� �����������, ����� �� ������ �������� ��� ���������� ��� �������
	virtual void resume() {}; // �� �����������, ����� �� ������ �������� ��� ������� ��� ��� �� �������� � ������ pause()
//	virtual void save(GameData & data) {}; // ��������� ���, ����� �� ������ ��������� ��� ��������� ����� �������, ����� ����� ����� ���� ������� � ��� �����, ����� ������ enter() � ������ load()
//	virtual void load(GameData & data) {}; // ��������� ���, ����� �� ������ ������������ ��������� ������, ��������� ������ ���������� � ������ save()

	virtual bool frameStarted(const FrameEvent& evt) {return true;};
	virtual bool frameEnded(const FrameEvent& evt) {return true;};
	virtual bool keyPressed( const OIS::KeyEvent &arg ) {return true;};
	virtual bool keyReleased( const OIS::KeyEvent &arg ) {return true;};
	virtual bool mouseMoved( const OIS::MouseEvent &arg ) {return true;};
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;};
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;};

	virtual void windowResize() {}; // ����������� �� ��������� �������� ���� �������

//	GameData mGameData; // ����� �������� ������ ��� ���������� ����������
};
//===================================================================================
