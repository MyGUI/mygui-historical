#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "WidgetOIS.h"
#include "Instance.h"
#include "Common.h"
#include "WidgetDefines.h"

namespace MyGUI
{

	// ������� ��� ����� ���������� ����� ������
	typedef delegates::CDelegate1<const std::string &> EventChangeLanguage;

	class _MyGUIExport InputManager :  public Ogre::FrameListener
	{
		INSTANCE_HEADER(InputManager);

		typedef std::vector<wchar_t> LangInfo;
		typedef std::map<std::string, LangInfo> MapLang;

	public:
		void initialise();
		void shutdown();

	public:

		void load(const std::string & _file);

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		bool injectMouseMove( const OIS::MouseEvent & _arg);
		bool injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );
		bool injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );

		bool injectKeyPress(const OIS::KeyEvent & _arg);
		bool injectKeyRelease(const OIS::KeyEvent & _arg);

		inline bool isCaptureMouse() {return mWidgetMouseFocus != null;}
		inline bool isCaptureKey() {return mWidgetKeyFocus != null;}

		void setKeyFocusWidget(WidgetPtr _widget);
		inline void resetKeyFocusWidget() {setKeyFocusWidget(null);}

		inline WidgetPtr getMouseFocusWidget() {return mWidgetMouseFocus;}
		inline WidgetPtr getKeyFocusWidget() {return mWidgetKeyFocus;}
		inline const std::string & getCurrentLanguage() {return mCurrentLanguage->first;}
		inline const IntPoint & getLastLeftPressed() {return mLastLeftPressed;}
		inline const IntPoint & getMousePosition() {return mMousePosition;}

		// �������� �������, ������� ������ ����
		void resetMouseFocusWidget();

		// ������� ����� ������
		EventChangeLanguage eventChangeLanguage;

	protected:
		void detectLangShift(int keyEvent, bool bIsKeyPressed);
		wchar_t getKeyChar(int keyEvent); // ���������� ������ �� ��� ���� ����

		// ������� ��������� ���������
		void createDefaultCharSet();

		// ������� ���� �� ���������
		inline void changeLanguage()
		{
			mCurrentLanguage++;
			if (mCurrentLanguage == mMapLanguages.end())
				mCurrentLanguage = mMapLanguages.begin();
			// ������� �������
			eventChangeLanguage(mCurrentLanguage->first);
		}

		// ���������� ������� ��� ��������� ����������
		inline void storeKey(int _key)
		{
			mHoldKey = OIS::KC_UNASSIGNED;

			if ( ! isCaptureKey() ) return;
			if ( ! mIsListener) {
				Ogre::Root::getSingleton().addFrameListener(this);
				mIsListener = true;
			}
			if ( (_key == OIS::KC_LSHIFT)
				|| (_key == OIS::KC_RSHIFT)
				|| (_key == OIS::KC_LCONTROL)
				|| (_key == OIS::KC_RCONTROL)
				|| (_key == OIS::KC_LMENU)
				|| (_key == OIS::KC_RMENU)
				) return;

			mHoldKey = _key;
			mFirstPressKey = true;
			mTimerKey = 0.0f;
		}

		// ���������� ������� ����������
		inline void resetKey()
		{
			mHoldKey = OIS::KC_UNASSIGNED;
		}


	protected:

		// ������� ������� ����������� �����
		WidgetPtr mWidgetMouseFocus;
		WidgetPtr mWidgetKeyFocus;
		// �������� �������, ����� ������� ����������� �����
		WidgetPtr mWidgetRootMouseFocus;
		WidgetPtr mWidgetRootKeyFocus;
		// �������� �� ���� �������� ������
		bool mIsWidgetMouseCapture;
		// ������ ��� �������� �����
	    Ogre::Timer mTime; //used for double click timing

		// ����� ������
		MapLang mMapLanguages;
		// ������� ����
		MapLang::iterator mCurrentLanguage;
		// ����� �� ����
		bool mIsCharShift;
		// ������ ��� �������
		LangInfo mNums;
		// ��� ��� ���� ��������� ��� ������ ����� ������
		IntPoint mLastLeftPressed;
		IntPoint mMousePosition;
		// ������� ��� �������
		int mHoldKey;
		bool mFirstPressKey;
		float mTimerKey;
		bool mIsListener;

	}; // class InputManager

} // namespace MyGUI

#endif // __INPUT_MANAGER_H__
