/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WINDOW_H__
#define __MYGUI_WINDOW_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WindowFactory.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class Window;
	typedef Window * WindowPtr;

	typedef delegates::CDelegate2<WidgetPtr, const std::string&> EventButtonPressedInfo;

	class _MyGUIExport Window : public Widget
	{
		// ��� ������ ��������� ������������
		friend class factory::WindowFactory;

	protected:
		Window(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// �������������� ��� ���������� �������
		virtual WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name = "");

		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("Window"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		inline void setPosition(int _left, int _top) {setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// ��� ������������ ������ � �������
		void showSmooth(bool _reset = false);
		void hideSmooth();
		void destroySmooth();

		inline bool getAutoAlpha() {return mIsAutoAlpha;}
		inline void setAutoAlpha(bool _auto)
		{
			mIsAutoAlpha = _auto;
			if (false == _auto) setAlpha(ALPHA_MAX);
			else updateAlpha();
		}

		virtual void setCaption(const Ogre::DisplayString & _caption) {if (mWidgetCaption!=null)mWidgetCaption->setCaption(_caption);};
		virtual const Ogre::DisplayString & getCaption() {if (mWidgetCaption!=null) return mWidgetCaption->getCaption();return Widget::getCaption();};

		inline void setMinMax(IntRect _minmax) {mMinmax = _minmax;}
		inline void setMinMax(int _min_h, int _min_v, int _max_h, int _max_v) {mMinmax.set(_min_h, _min_v, _max_h, _max_v);}
		inline IntRect getMinMax() {return mMinmax;}

		virtual void setPosition(const IntPoint& _pos);
		virtual void setPosition(const IntCoord& _coord);
		virtual void setSize(const IntSize& _size);

		inline bool getIsToStick() {return mIsToStick;}
		inline void setIsToStick(bool _stick) {mIsToStick = _stick;}

		const IntCoord& getClientRect();

		virtual VectorWidgetPtr getChilds();

		// event : ����� �� ������ � ����
		// signature : void method(MyGUI::WidgetPtr _widget, const std::string& _name)
		EventButtonPressedInfo eventWindowButtonPressed;

	protected:

		void _onMouseChangeRootFocus(bool _focus);
		void _onKeyChangeRootFocus(bool _focus);
		void _onMouseMove(int _left, int _top);
		void _onMouseButtonPressed(bool _left);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender, bool _double);
		void notifyMouseMovedAction(MyGUI::WidgetPtr _sender, int _left, int _top);

		// ������ ��������� ����� ������������ �� ������
		void updateAlpha();

	private:
		WidgetPtr mWidgetCaption, mWidgetClient;

		// ������� ���� ����� ������� ��� ���������
		IntCoord mPreActionCoord;

		// ���� ������� ������
		bool mMouseRootFocus;
		bool mKeyRootFocus;

		// �������������� ��� ������ ���������� ������
		bool mIsAutoAlpha;

		// ����������� � ������������ ������� ����
		IntRect mMinmax;

		bool mIsToStick; // ���������� �� � �����

		IntCoord mCurrentActionScale;


	}; // class Window : public Widget

} // namespace MyGUI

#endif // __MYGUI_WINDOW_H__
