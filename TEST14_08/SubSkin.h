#pragma once

#include <Ogre.h>
//#include <OgreOverlay.h>
//#include <OgreOverlayManager.h>
//#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include "SubWidget.h"

namespace widget
{
	using namespace Ogre;

	class SubSkin// : public SubWidget
	{

	public:
		SubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, char _align, SubWidget * _parent);
		~SubSkin();

		void move(int _x, int _y);
		void move(int _x, int _y, int _cx, int _cy);
//		void size(int _cx, int _cy);

		// ���������� ���� ������� ����� ���� ������ ��� �� �� ���� ������
		void update();
		// ������ ��������������� ��� ������� � �������
		void restore();
		// �������� ���������� �������
		void show(bool _show);
		// ������������ ��������� �����, ����� ��� ������� ����
		void correct();

		void attach(Ogre::OverlayElement * _element);

		void align(int _cx, int _cy);

//		void test();

	protected:

		float m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV;

		Ogre::PanelOverlayElement * m_overlayContainer;

	}; // class SubSkin

} // namespace widget