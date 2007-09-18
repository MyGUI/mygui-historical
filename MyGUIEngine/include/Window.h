#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Widget.h"

namespace MyGUI
{

	class _MyGUIExport Window : public Widget, public Ogre::FrameListener
	{
		// ��� ������ ��������� ������������
		friend WidgetFactory<Window>;

	protected:
		Window(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("Window"); return type;};

		void showWindowCaption(bool _show);
		void showWindowX(bool _show);
		void showWindowResize(bool _show);

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void show(bool _show, bool _smoot);

	protected:

		virtual void OnMouseLostFocus(WidgetPtr _new);
		virtual void OnMouseSetFocus(WidgetPtr _old);
		virtual void OnMouseButtonPressed(bool _left);
		virtual void OnMouseButtonReleased(bool _left);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMousePressedX(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMovedCaption(MyGUI::WidgetPtr _sender, int _x, int _y);
		void notifyMouseMovedResize(MyGUI::WidgetPtr _sender, int _x, int _y);

		void setDoAlpha(float _alpha);

	private:
		std::string mSkinCaption, mSkinX, mSkinResize;
		WidgetPtr mWidgetCaption, mWidgetX, mWidgetResize;
		floatRect mOffsetCaption, mOffsetX, mOffsetResize;
		char mAlignCaption, mAlignX, mAlignResize;

		intSize mSkinSize;

		// ������� ���� ����� ������� ��� ���������
		intRect m_preActionRect;

		// ���������� �� �� �� ����� �����
		bool m_bIsListenerAlpha;
		// �����, ������� �� �������� �������
		float m_doAlpha;
		// ���������� �� ����� �������
		bool m_isDestroy;


	}; // class Window : public Widget

	typedef Window * WindowPtr;

} // namespace MyGUI

#endif // _WINDOW_H_