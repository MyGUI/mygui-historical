/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#ifndef __MYGUI_MULTI_LIST_H__
#define __MYGUI_MULTI_LIST_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	struct RowInfo
	{
		ListPtr list;
		ButtonPtr button;
		int width;
	};

	typedef std::vector<RowInfo> VectorRowInfo;

	class _MyGUIExport MultiList : public Widget
	{
		// ��� ������ ��������� ������������
		friend class factory::MultiListFactory;

	protected:
		MultiList(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("MultiList"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//----------------------------------------------------------------------------------//
		// ������ ��� ������ �� ���������
		inline size_t getRowCount() { return mVectorRowInfo.size();}

		void insertRow(size_t _index, int _width, const Ogre::DisplayString & _name);
		inline void addRow(int _width, const Ogre::DisplayString & _name) {insertRow(ITEM_NONE, _width, _name);}
		void setRowName(size_t _index, const Ogre::DisplayString & _name);
		void setRowWidth(size_t _index, int _width);
		const Ogre::DisplayString & getRowName(size_t _index);
		int getRowWidth(size_t _index);

		void deleteRow(size_t _index);
		void deleteAllRows();

		//----------------------------------------------------------------------------------//
		// ������ ��� ������ �� ��������
		size_t getItemCount();

		void insertItem(size_t _index, const Ogre::DisplayString & _item);
		inline void addItem(const Ogre::DisplayString & _item) {insertItem(ITEM_NONE, _item);}
		void setItem(size_t _index, const Ogre::DisplayString & _item);
		const Ogre::DisplayString & getItem(size_t _index);

		void deleteItem(size_t _index);
		void deleteAllItems();

		size_t getItemSelect();
		void resetItemSelect();
		void setItemSelect(size_t _index);

		//----------------------------------------------------------------------------------//
		// ������ ��� ������ � ��� ��������
		void setSubItem(size_t _row, size_t _index, const Ogre::DisplayString & _item);
		const Ogre::DisplayString & getSubItem(size_t _row, size_t _index);

		//----------------------------------------------------------------------------------//
		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

	protected:
		void notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position);

		void updateRows();
		void updateOnlyEmpty();

	private:
		int mHeightButton;
		int mWidthBar;
		std::string mSkinButton, mSkinList;
		ButtonPtr mButtonMain;

		WidgetPtr mWidgetClient;
		VectorRowInfo mVectorRowInfo;
		//ListPtr mMainList;


	}; // class _MyGUIExport MultiList : public Widget

} // namespace MyGUI

#endif // __MYGUI_MULTI_LIST_H__
