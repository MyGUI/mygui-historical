/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_GUI_H__
#define __MYGUI_GUI_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Common.h"

#include "MyGUI_TextSimpleOverlayElementFactory.h"
#include "MyGUI_TextEditOverlayElementFactory.h"
#include "MyGUI_PanelAlphaOverlayElementFactory.h"
#include "MyGUI_SharedPanelAlphaOverlayElementFactory.h"

#include "MyGUI_InputManager.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_DynLibManager.h"

namespace MyGUI
{
	
	class _MyGUIExport Gui : public Ogre::FrameListener
	{
		friend WidgetManager;
		INSTANCE_HEADER(Gui);

		typedef std::list<WidgetPtr> ListFrameListener;

	public:
		void initialise(Ogre::RenderWindow* _window);
		void shutdown();

		// ������ � ������� ��� �������� �������
		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "");
		inline WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, IntRect _rect, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidget(_type, _skin, _rect.left, _rect.top, _rect.getWidth(), _rect.getHeight(), _align, _layer, _name);
		}

		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidget(_type, _skin, (int)(_left*mWidth), (int)(_top*mHeight), (int)(_width*mWidth), (int)(_height*mHeight), _align, _layer, _name);
		}
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, FloatRect _rect, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidget(_type, _skin, _rect.left, _rect.top, _rect.getWidth(), _rect.getHeight(), _align, _layer, _name);
		}

		// ������� ��� �������� ������� �� ����
		template <class T>
		inline T* createWidgetT(const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			Widget * widget = createWidget(T::getType(), _skin, _left, _top, _width, _height, _align, _layer, _name);
			MYGUI_TYPE(T*, widget);
			return static_cast<T*>(widget);
		}
		template <class T>
		inline T* createWidgetT(const Ogre::String & _skin, IntRect _rect, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidgetT<T>(_skin, _rect.left, _rect.top, _rect.getWidth(), _rect.getHeight(), _align, _layer, _name);
		}
		template <class T>
		inline T* createWidgetRealT(const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			Widget * widget = createWidgetReal(T::getType(), _skin, _left, _top, _width, _height, _align, _layer, _name);
			MYGUI_TYPE(T*, widget);
			return static_cast<T*>(widget);
		}
		template <class T>
		inline T* createWidgetRealT(const Ogre::String & _skin, FloatRect _rect, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidgetRealT<T>(_skin, _rect.left, _rect.top, _rect.getWidth(), _rect.getHeight(), _align, _layer, _name);
		}


		inline float getWidth() {return mWidth;}
		inline float getHeight() {return mHeight;}

		// �������� �� �����
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		bool addFrameListener(WidgetPtr _listener);
		bool removeFrameListener(WidgetPtr _listener);

	private:
		// ������� ������ �������� ������� ��������
		void _destroyChildWidget(WidgetPtr & _widget);
		// ������� ���� �����
		void _destroyAllChildWidget();

	private:
		// ������ ���� ����� ��������
		VectorWidgetPtr mWidgetChild;

		float mHeight, mWidth;

		// ������� ����� ��������
		TextSimpleOverlayElementFactory * mFactoryTextSimpleOverlay;
		TextEditOverlayElementFactory * mFactoryTextEditOverlay;
		PanelAlphaOverlayElementFactory * mFactoryPanelAlphaOverlay;
		SharedPanelAlphaOverlayElementFactory * mFactorySharedPanelAlphaOverlay;

		// ��������� ���
		InputManager * mInputManager;
		SubWidgetManager * mCroppedRectangleManager;
		ClipboardManager* mClipboardManager;
		LayerManager* mLayerManager;
		SkinManager* mSkinManager;
		WidgetManager* mWidgetManager;
		LayoutManager* mLayoutManager;
		FontManager* mFontManager;
		PointerManager* mPointerManager;
		DynLibManager* mDynLibManager;
		PluginManager* mPluginManager;

		ListFrameListener mListFrameListener;
		ListFrameListener mListFrameListenerAdd;

	}; // class Gui

} // namespace MyGUI

#endif // __MYGUI_GUI_H__
