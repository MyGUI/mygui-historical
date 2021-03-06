/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_H__
#define __MYGUI_POPUP_MENU_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class _MyGUIExport PopupMenu : public Widget
	{
		// ��� ������ ��������� ������������
		friend class factory::PopupMenuFactory;

	protected:
		PopupMenu(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("PopupMenu"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// ������ ��� ������ �� �������
		void addItem(const Ogre::UTFString& _item);
		void insertItem(size_t _index, const Ogre::UTFString& _item);
		void setItem(size_t _index, const Ogre::UTFString& _item);

		void deleteItem(size_t _index);
		void deleteAllItems();

		size_t getItemCount();
		const Ogre::UTFString& getItem(size_t _index);

		// ������ ��� ������ � �����
		void showPopupMenu(const IntPoint& _point);

		// event : ����� �����, ��� ������ ����
		// signature : void method(MyGUI::WidgetPtr _widget, size_t _index)
		EventInfo_WidgetSizeT eventPopupMunuAccept;

	private:
		void notifyMouseClick(MyGUI::WidgetPtr _sender);

		void _onKeyChangeRootFocus(bool _focus);

		void update();

	private:
		VectorWidgetPtr m_listWidgets;

		int mHeightLine;
		std::string mSkinLine;

		WidgetPtr mWidgetClient;

		int mMaxWidth;

	}; // class _MyGUIExport PopupMenu : public Widget

} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_H__
