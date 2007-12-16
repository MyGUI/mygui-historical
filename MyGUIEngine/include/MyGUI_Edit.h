/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_EDIT_H__
#define __MYGUI_EDIT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_EditFactory.h"
#include "MyGUI_Widget.h"
#include "MyGUI_TextChangeHistory.h"

namespace MyGUI
{

	class Edit;
	typedef Edit * EditPtr;

	class _MyGUIExport Edit : public Widget
	{
		// ��� ������ ��������� ������������
		friend factory::EditFactory;

	protected:
		Edit(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);
		virtual ~Edit();

	public:

		// ��� ������� �������
		inline static const Ogre::String & getType() {static Ogre::String type("Edit"); return type;}

		// ������������� �����
		void setCaption(const Ogre::DisplayString & _caption);
		// ���������� ���������
		void getTextSelect(size_t & _start, size_t & _end);
		// ������� �� �����
		inline bool isTextSelect() {return ( (mStartSelect != SIZE_MAX) && (mStartSelect != mEndSelect) );}
		// ���������� ���������� �����
		Ogre::DisplayString getSelectedText();

		inline size_t getTextLenght() {return mTextLenght;}
		inline size_t getTextCursor() {return mCursorPosition;}

		void setTextCursor(size_t _index);
		// ������������� ���������
		void setTextSelect(size_t _start, size_t _end);
		// ���������� �����
		Ogre::DisplayString getText(size_t _start, size_t _count);

		//---------------------------------------------------------------//
		// ������ ���������� ������� �� ������� �������
		//---------------------------------------------------------------//

		// ������� ��� ��� ���������
		inline bool deleteTextSelect() {return deleteTextSelect(false);}
		// ��������� ����� � ��������� �������
		inline void insertText(const Ogre::DisplayString & _text, size_t _index = SIZE_MAX) {insertText(_text, _index, false);}
		// ��������� ����� � �����
		inline void addText(const Ogre::DisplayString & _text) {addText(_text, false);}
		// ������� �����
		inline void eraseText(size_t _start, size_t _count = 1) {eraseText(_start, _count, false);}
		// �������� ������ ���������
		inline void setTextSelectColour(const Ogre::ColourValue & _colour)
		{
			setTextSelectColour(_colour, false);
		}
		// �������� ������ ��������
		inline void setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour)
		{
			setTextColour(_start, _count, _colour, false);
		}

		inline bool getEditReadOnly() {return mModeReadOnly;}
		inline void setEditReadOnly(bool _read)
		{
			mModeReadOnly = _read;
			// ���������� �������
			commandResetHistory();
		}

		inline bool getEditPassword() {return mModePassword;}
		inline void setEditPassword(bool _password)
		{
			if (mModePassword == _password) return;
			mModePassword = _password;
			if (mModePassword) {
				mPasswordText = mText->getCaption();
				mText->setCaption(Ogre::DisplayString(mTextLenght, '*'));
			}
			else {
				mText->setCaption(mPasswordText);
				mPasswordText.clear();
			}
			// ��������� �� ��������
			updateView(false);
			// ���������� �������
			commandResetHistory();
		}

		inline bool getEditMultiLine() {return mModeMultiline;}
		inline void setEditMultiLine(bool _multi)
		{
			mModeMultiline = _multi;
			// �� ������, ��� �������� ���������
			if (false == mModeMultiline) {
				setCaption(getRealString());
				//setRealString(getRealString());
			}
			// ��������� �� ��������
			else updateView(false);
			// ���������� �������
			commandResetHistory();
		}

		inline void setEditStatic(bool _static)
		{
			mModeStatic = _static;
			resetSelect();
		}

		inline bool getEditStatic()
		{
			return mModeStatic;
		}

		void setPosition(const IntCoord& _coord);
		void setSize(const IntSize& _size);

		// event : ����� �����, ��� ������� ����� � ����������
		// signature : void method(MyGUI::WidgetPtr _widget)
		EventSimple eventEditSelectAccept;

	protected:

		// ������������� �����
		void setText(const Ogre::DisplayString & _text, bool _history);
		// ������� ��� ��� ���������
		bool deleteTextSelect(bool _history);
		// ��������� ����� � ��������� �������
		void insertText(const Ogre::DisplayString & _text, size_t _index, bool _history);
		// ��������� ����� � �����
		inline void addText(const Ogre::DisplayString & _text, bool _history) {insertText(_text, SIZE_MAX, _history);}
		// ������� �����
		void eraseText(size_t _start, size_t _count, bool _history);
		// �������� ������ ���������
		void setTextSelectColour(const Ogre::ColourValue & _colour, bool _history);
		// �������� ������ ��������
		void setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour, bool _history);

	protected:
		void _frameEntered(float _frame, float _event);

		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top);

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onMouseMove(int _left, int _top);
		virtual void _onKeyLostFocus(WidgetPtr _new);
		virtual void _onKeySetFocus(WidgetPtr _old);
		virtual void _onKeyButtonPressed(int _key, wchar_t _char);
		virtual void _onKeyButtonReleased(int _key);

		void updateEditState();

		// ��������� ������ �� ����������
		void updateCursor(IntPoint _point);
		// ����� �� ������ � ���� ���������
		bool isShowCursorInView();
		// ���������� �������������
		void updateView(bool _showCursor);

		inline IntPoint getWorldPostion(WidgetPtr _widget)
		{
			IntPoint point(_widget->getLeft(), _widget->getTop());
			WidgetPtr parent = _widget->getParent();
			while (parent != null) {
				point.left += parent->getLeft();
				point.top += parent->getTop();
				parent = parent->getParent();
			};
			return point;
		}

		inline void resetSelect()
		{
			if (mStartSelect != SIZE_MAX) {
				mStartSelect = SIZE_MAX;
				mText->setTextSelect(0, 0);
			}
		}

		// ������ � ������� ������ � �������
		void commandPosition(size_t _undo, size_t _redo, size_t _lenght, VectorChangeInfo * _info = null);

		// �������� ������ � ������
		bool commandRedo();
		bool commandUndo();
		// ���������� ��������� ��� ��������
		void commandMerge();
		// �������
		inline void commandResetRedo() {mVectorRedoChangeInfo.clear();}
		inline void commandResetHistory() {mVectorRedoChangeInfo.clear();mVectorUndoChangeInfo.clear();}
		void saveInHistory(VectorChangeInfo * _info = null);

		// ������ � ������� ������
		void commandCut();
		void commandCopy();
		void commandPast();


		inline const Ogre::DisplayString & getRealString()
		{
			if (mModePassword) return mPasswordText;
			return mText->getCaption();
		}

		inline void setRealString(const Ogre::DisplayString & _caption)
		{
			if (mModePassword) {
				mPasswordText = _caption;
				mText->setCaption(Ogre::DisplayString(mTextLenght, '*'));
			}
			else {
				mText->setCaption(_caption);
			}
		}


	protected:
		// ������ �� ������
		bool mIsPressed;
		// � ������ �� ������
		bool mIsFocus;

		WidgetPtr mWidgetUpper;
		WidgetPtr mWidgetCursor;

		bool mCursorActive;
		float mCursorTimer, mActionMouseTimer;

		// ������� �������
		size_t mCursorPosition;
		// ������������ �����������
		size_t mTextLenght;

		bool mShiftPressed;
		bool mCtrlPressed;

		// ���������
		size_t mStartSelect, mEndSelect;

		// ������� ��������� ��� ������ � �������
		DequeUndoRedoInfo mVectorUndoChangeInfo;
		DequeUndoRedoInfo mVectorRedoChangeInfo;

		// ������
		IntSize mSizeView;
		IntSize mHalfCursor;

		bool mMouseLeftPressed;

		bool mModeReadOnly;
		bool mModePassword;
		bool mModeMultiline;
		bool mModeStatic;

		// ��������� �����, �������� �� �����������
		Ogre::DisplayString mPasswordText;

	}; // class Edit : public Widget

} // namespace MyGUI

#endif // __MYGUI_EDIT_H__
