#ifndef _WIDGETEVENT_H_
#define _WIDGETEVENT_H_

#include "Prerequest.h"
#include "delegate.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	// �������� ��� ������� �������
	typedef CDelegate1<WidgetPtr> EventSimple;
	typedef CDelegate2<WidgetPtr, WidgetPtr> EventFocusInfo;
	typedef CDelegate2<WidgetPtr, bool> EventSimpleInfo;
	typedef CDelegate2<WidgetPtr, int> EventSimpleDataInfo;
	typedef CDelegate3<WidgetPtr, int, int> EventCoordInfo;
	typedef CDelegate3<WidgetPtr, int, wchar_t> EventKeyInfo;
	typedef CDelegate3<WidgetPtr, const std::string&, const std::string&> EventActionInfo;

	

	class _MyGUIExport WidgetEvent
	{
		friend InputManager;

	protected:
		WidgetEvent() : m_widgetEventSender(0), m_needKeyFocus(false) {}


		/*
		����� ���������� �� �������� �������� ��� ������� :

			void anyFunc(...) {}; // ���������� �������

			class AnyClass
			{
			public:
				static void anyStaticMethod(...) {}; // ����������� �����
				void anyMethod(...) {}; // ������� �����
			};
			
			AnyClass anyObject; // ������ ������

		�������� ��������:
			eventAny = MyGUI::newDelegate(anyFunc);
			eventAny = MyGUI::newDelegate(AnyClass::anyStaticMethod);
			eventAny = MyGUI::newDelegate(&anyObject, &AnyClass::anyMethod);
		*/

	public:

		//	������� : ������ ������� ����� ����
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		EventFocusInfo eventMouseLostFocus;

		//	������� : ������ ������� ����� ����
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		EventFocusInfo eventMouseSetFocus;

		//	������� : ���� ��������� ��� ����������� ��������
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, int _x, int _y);
		EventCoordInfo eventMouseMove;

		//	������� : ��������� ������� ��� ����������� ��������
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, int _rel);
		EventSimpleDataInfo eventMouseSheel;

		//	������� : ������ ������� ����
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _left);
		EventSimpleInfo eventMouseButtonPressed;

		//	������� : ��������� ������� ����
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _left);
		EventSimpleInfo eventMouseButtonReleased;

		//	������� : ������ � ��������� ������� ����
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _double);
		EventSimpleInfo eventMouseButtonClick;

		//	������� : ������ ������� ����� ����������
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		EventFocusInfo eventKeyLostFocus;

		//	������� : ������ ������� ����� ����������
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		EventFocusInfo eventKeySetFocus;

		//	������� : ������ �������
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, int _key, wchar_t _char);
		EventKeyInfo eventKeyButtonPressed;

		//	������� : ��������� �������
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, int _key);
		EventSimpleDataInfo eventKeyButtonReleased;

		//	������� : �������� ������ ������� ����� ����, ��������� ��� �������� ���������
		// ��� ������� �������� ������ �������� ������
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _focus);
		EventSimpleInfo  eventMouseChangeRootFocus;

		//	������� : �������� ������ ������� ����� �����, ��������� ��� �������� ���������
		// ��� ������� �������� ������ �������� ������
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, bool _focus);
		EventSimpleInfo  eventKeyChangeRootFocus;

		//	������� : ����� ����������� ������� ��� �������� ��� ������ �������
		//	�������� �������� : void method(MyGUI::WidgetPtr _sender, const std::string & _key, const std::string & _value);
		EventActionInfo eventActionInfo;

	protected:

		// ��������� �� ��������� �����
		virtual void _onMouseLostFocus(WidgetPtr _new)
		{
			eventMouseLostFocus(m_widgetEventSender, _new);
		}

		virtual void _onMouseSetFocus(WidgetPtr _old)
		{
			eventMouseSetFocus(m_widgetEventSender, _old);
		}

		virtual void _onMouseMove(int _x, int _y)
		{
			eventMouseMove(m_widgetEventSender, _x, _y);
		}

		virtual void _onMouseSheel(int _rel)
		{
			eventMouseSheel(m_widgetEventSender, _rel);
		}

		virtual void _onMouseButtonPressed(bool _left)
		{
			eventMouseButtonPressed(m_widgetEventSender, _left);
		}

		virtual void _onMouseButtonReleased(bool _left)
		{
			eventMouseButtonReleased(m_widgetEventSender, _left);
		}

		virtual void _onMouseButtonClick(bool _double)
		{
			eventMouseButtonClick(m_widgetEventSender, _double);
		}

		virtual void _onKeyLostFocus(WidgetPtr _new)
		{
			eventKeyLostFocus(m_widgetEventSender, _new);
		}

		virtual void _onKeySetFocus(WidgetPtr _old)
		{
			eventKeySetFocus(m_widgetEventSender, _old);
		}

		virtual void _onKeyButtonPressed(int _key, wchar_t _char)
		{
			eventKeyButtonPressed(m_widgetEventSender, _key, _char);
		}

		virtual void _onKeyButtonReleased(int _key)
		{
			eventKeyButtonReleased(m_widgetEventSender, _key);
		}

		virtual void _onMouseChangeRootFocus(bool _focus)
		{
			eventMouseChangeRootFocus(m_widgetEventSender, _focus);
		}

		virtual void _onKeyChangeRootFocus(bool _focus)
		{
			eventKeyChangeRootFocus(m_widgetEventSender, _focus);
		}

		// ����� �� ������� ���� � �����
		bool m_needKeyFocus;
		// �� ����� ����� �� �������� ���������
		WidgetPtr m_widgetEventSender;

	};

} // namespace MyGUI

#endif
