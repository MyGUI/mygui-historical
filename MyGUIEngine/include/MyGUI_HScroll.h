/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_HSCROLL_H__
#define __MYGUI_HSCROLL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_HScrollFactory.h"
#include "MyGUI_ScrollBase.h"

namespace MyGUI
{

	class HScroll;
	typedef HScroll * HScrollPtr;

	class _MyGUIExport HScroll : public ScrollBase
	{
		// ��� ������ ��������� ������������
		friend factory::HScrollFactory;

	protected:
		HScroll(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		void notifyTrackMove(int _left, int _top);
		void updateTrack();

	}; // class HScroll : public ScrollBase

} // namespace MyGUI

#endif // __MYGUI_HSCROLL_H__