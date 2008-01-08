/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_COMBO_BOX_H__
#define __MYGUI_COMBO_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ComboBoxFactory.h"
#include "MyGUI_Edit.h"
#include "MyGUI_List.h"
#include "MyGUI_Button.h"

namespace MyGUI
{

	class ComboBox;
	typedef ComboBox* ComboBoxPtr;

	class _MyGUIExport ComboBox : public Edit
	{
		// ��� ������ ��������� ������������
		friend class factory::ComboBoxFactory;

	protected:
		ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		virtual void _onKeyButtonPressed(int _key, wchar_t _char);

		void notifyButtonPressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyListLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyListSelectAccept(MyGUI::WidgetPtr _widget);
		void notifyListMouseChangePosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyEditTextChange(MyGUI::WidgetPtr _sender);

		void showList();
		void hideList();

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("ComboBox"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		inline void setComboItemIndex(size_t _index)
		{
			MYGUI_ASSERT(_index < mList->getItemCount(), "index out of range");
			mItemIndex = _index;
			Edit::setCaption(mList->getItemString(_index));
		}

		inline size_t getComboItemIndex()
		{
			return mItemIndex;
		}

		inline void setComboModeDrop(bool _drop)
		{
			mModeDrop = _drop;
			setEditStatic(mModeDrop);
		}

		inline bool getComboModeDrop()
		{
			return mModeDrop;
		}

		inline size_t getItemCount()
		{
			return mList->getItemCount();
		}

		inline const Ogre::DisplayString & getItemString(size_t _index)
		{
			MYGUI_ASSERT(_index < mList->getItemCount(), "index out of range");
			return mList->getItemString(_index);
		}

		inline void setItemString(size_t _index, const Ogre::DisplayString & _item)
		{
			MYGUI_ASSERT(_index < mList->getItemCount(), "index out of range");
			mList->setItemString(_index, _item);
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
		}

		void insertItemString(size_t _index, const Ogre::DisplayString & _item)
		{
			mList->insertItemString(_index, _item);
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
		}

		void deleteItemString(size_t _index)
		{
			mList->deleteItemString(_index);
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
		}

		inline void addItemString(const Ogre::DisplayString& _item)
		{
			mList->addItemString(_item);
			mItemIndex = ITEM_NONE;
			mList->setItemSelect(mItemIndex);
		}

		inline void setSmoothShow(bool _smooth)
		{
			mShowSmooth = _smooth;
		}

		inline bool getSmoothShow()
		{
			return mShowSmooth;
		}

		// event : ����� ����� � ����� ������ ��� ������ ����� � ���� ������
		// signature : void method(MyGUI::WidgetPtr _widget)
		EventSimple eventComboAccept;

	private:
		ButtonPtr mButton;
		ListPtr mList;

		bool mListShow;
		int mMaxHeight;
		size_t mItemIndex;
		bool mModeDrop;
		bool mDropMouse;
		bool mShowSmooth;

		//float mDoAlpha;

	}; // class _MyGUIExport ComboBox : public Edit

} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_H__
