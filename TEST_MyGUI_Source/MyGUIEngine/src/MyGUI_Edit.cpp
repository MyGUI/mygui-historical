/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Edit.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_SubWidgetTextInterface.h"

namespace MyGUI
{

	const float EDIT_CURSOR_TIMER  = 0.7f;
	const float EDIT_ACTION_MOUSE_TIMER  = 0.20f;
	const int EDIT_CURSOR_MAX_POSITION = 100000;
	const int EDIT_CURSOR_MIN_POSITION = -100000;
	const size_t EDIT_MAX_UNDO = 128;
	const size_t EDIT_MAX_LENGHT = 2048;
	const float EDIT_OFFSET_HORZ_CURSOR = 50.0f; // �������������� �������� ��� �������
	const int EDIT_ACTION_MOUSE_ZONE = 50; // ������� ��� ���������� ���� �� �������� �����
	const std::string EDIT_CLIPBOARD_TYPE_TEXT = "Text";

	Edit::Edit(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mIsPressed(false),
		mIsFocus(false),
		mWidgetUpper(null),
		mCursorActive(false),
		mCursorTimer(0),
		mActionMouseTimer(0),
		mCursorPosition(0),
		mTextLength(0),
		mShiftPressed(false),
		mCtrlPressed(false),
		mStartSelect(ITEM_NONE),
		mEndSelect(0),
		mMouseLeftPressed(false),
		mModeReadOnly(false),
		mModePassword(false),
		mModeMultiline(false),
		mModeStatic(false)
	{

		MYGUI_ASSERT(null != mText, "TextEdit not found in skin (Edit must have TextEdit)");

		mOriginalPointer = mPointer;

		// ��� ����� ����� �����
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetUpper = (*iter);
				mWidgetUpper->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
				mWidgetUpper->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
				mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
				mWidgetUpper->eventMouseButtonReleased = newDelegate(this, &Edit::notifyMouseReleased);
				mWidgetUpper->eventMouseDrag = newDelegate(this, &Edit::notifyMouseDrag);
			}
		}
		MYGUI_ASSERT(null != mWidgetUpper, "Child Widget Client not found in skin (Edit must have Client)");


		// �������������� ������������� �������
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();

		//setCaption("erghsdkflhgs;flhgs;lkjh;lsdkfh;dkkflhgkljdhfgklsdhfkglshdfkglhdflkhg");
		//mText->setViewOffset(IntPoint(-100, -100));


	}

	Edit::~Edit()
	{
		// �� ������ ����������
		Gui::getInstance().removeFrameListener(this);
	}

	void Edit::notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		if ( (_old == mWidgetUpper) || (mIsFocus) ) return;
		mIsFocus = true;
		updateEditState();
	}

	void Edit::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		if ( (_new == mWidgetUpper) || (false == mIsFocus) ) return;
		mIsFocus = false;
		updateEditState();
	}

	void Edit::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		// � ������� ��� ����������
		if (mModeStatic) return;

		IntPoint point = InputManager::getInstance().getLastLeftPressed();
		mCursorPosition = mText->getCursorPosition(point);
		mText->setCursorPosition(mCursorPosition);
		mText->setShowCursor(true);
		mCursorTimer = 0;
		updateSelectText();

		if (_left) mMouseLeftPressed = true;
	}

	void Edit::notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		// ���������� ������
		mMouseLeftPressed = false;
	}

	void Edit::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		// � ������� ��� ����������
		if (mModeStatic) return;

		// ������������� ������
		mText->setShowCursor(true);

		// ���������� ��� �������
		mCursorTimer = 0;
		mActionMouseTimer = 0;

		size_t Old = mCursorPosition;
		IntPoint point(_left, _top);
		mCursorPosition = mText->getCursorPosition(point);
		if (Old == mCursorPosition) return;

		mText->setCursorPosition(mCursorPosition);

		// ���� �� ���� ���������
		if (mStartSelect == ITEM_NONE) mStartSelect = Old;

		// ������ ���������
		mEndSelect = (size_t)mCursorPosition;
		if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
		else mText->setTextSelect(mStartSelect, mEndSelect);

	}

	void Edit::_onMouseDrag(int _left, int _top)
	{
		notifyMouseDrag(null, _left, _top);

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseDrag(_left, _top);
	}

	void Edit::_onKeySetFocus(WidgetPtr _old)
	{
		if (false == mIsPressed) {
			mIsPressed = true;
			updateEditState();

			if (false == mModeStatic) {
				mCursorActive = true;
				Gui::getInstance().addFrameListener(this);
				mText->setShowCursor(true);
				mText->setSelectBackground(true);
				mCursorTimer = 0;
				// ��� ��������������� ����������
				//mText->setCursorPosition(mCursorPosition);
				//updateSelectText();

			}
		}
		// !!! ����������� �������� � ����� ������
		Widget::_onKeySetFocus(_old);
	}

	void Edit::_onKeyLostFocus(WidgetPtr _new)
	{
		if (mIsPressed) {
			mIsPressed = false;
			updateEditState();

			mCursorActive = false;
			Gui::getInstance().removeFrameListener(this);
			mText->setShowCursor(false);
			mText->setSelectBackground(false);
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyLostFocus(_new);
	}

	void Edit::_onKeyButtonPressed(int _key, Char _char)
	{
		// � ����������� ������ ������ �� ��������
		if (mModeStatic) {
			Widget::_onKeyButtonPressed(_key, _char);
			return;
		}

		mText->setShowCursor(true);
		mCursorTimer = 0.0f;

		if (_key == OIS::KC_ESCAPE) {
			InputManager::getInstance().setKeyFocusWidget(null);
		}
		else if (_key == OIS::KC_BACK) {
			// ���� ���� �� ������� ���������� �����
			if (false == mModeReadOnly) {
				if (false == deleteTextSelect(true)) {
					// ������� �� ���� ����� � �������
					if (mCursorPosition != 0) {
						mCursorPosition -- ;
						eraseText(mCursorPosition, 1, true);
					}
				}
				// �������� ������� � ���������
				eventEditTextChange(this);
			}

		}
		else if (_key == OIS::KC_DELETE) {
			if (mShiftPressed) commandCut();
			else if (false == mModeReadOnly) {
				// ���� ���� �� ������� ���������� �����
				if (false == deleteTextSelect(true)) {
					if (mCursorPosition != mTextLength) {
						eraseText(mCursorPosition, 1, true);
					}
				}
				// �������� ������� � ���������
				eventEditTextChange(this);
			}

		}
		else if (_key == OIS::KC_INSERT) {
			if (mShiftPressed) commandPast();
			else if (mCtrlPressed) commandCopy();

		}
		else if (_key == OIS::KC_RETURN) {
			// �������� ������ � ������ ��������������
			if (false == mModeReadOnly) {
				if ((mModeMultiline) && (false == mCtrlPressed)) {
					// ������� ����������� ���� �������
					size_t size = mVectorUndoChangeInfo.size();
					// ��������������� ��������
					deleteTextSelect(true);
					insertText(TextIterator::getTextNewLine(), mCursorPosition, true);
					// ��������� �� ����������� �����������
					if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
					// �������� ������� � ���������
					eventEditTextChange(this);
				}
				// ��� ����� ���� � � ������+������� ���� �����
				else eventEditSelectAccept(this);
			}

		}
		else if (_key == OIS::KC_RIGHT) {
			if ((mCursorPosition) < mTextLength) {
				mCursorPosition ++;
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		}
		else if (_key == OIS::KC_LEFT) {
			if (mCursorPosition != 0) {
				mCursorPosition --;
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		}
		else if (_key == OIS::KC_UP) {
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top -= mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// ����� ������� �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					mCursorPosition = 0;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// ���������� ���������
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == OIS::KC_DOWN) {
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top += mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// ����� ������ �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLength) {
					mCursorPosition = mTextLength;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// ���������� ���������
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == OIS::KC_HOME) {
			// � ������ ������
			if ( false == mCtrlPressed) {
				IntPoint point = mText->getCursorPoint(mCursorPosition);
				point.left = EDIT_CURSOR_MIN_POSITION;
				size_t old = mCursorPosition;
				mCursorPosition = mText->getCursorPosition(point);
				if ( old != mCursorPosition ) {
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			}
			// � ������ ����� ������
			else {
				if (0 != mCursorPosition) {
					mCursorPosition = 0;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			}

		}
		else if (_key == OIS::KC_END) {
			// � ����� ������
			if ( false ==   mCtrlPressed) {
				IntPoint point = mText->getCursorPoint(mCursorPosition);
				point.left = EDIT_CURSOR_MAX_POSITION;
				size_t old = mCursorPosition;
				mCursorPosition = mText->getCursorPosition(point);
				if ( old != mCursorPosition ) {
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			}
			// � ����� �����
			else {
				if (mTextLength != mCursorPosition) {
					mCursorPosition = mTextLength;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			}

		}
		else if (_key == OIS::KC_PGUP) {
			// �� ������ ����, �� �� ������ ����� ������
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top -= (mWidgetUpper->getHeight() > mText->getFontHeight()) ? mWidgetUpper->getHeight() : mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// ����� ������� �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					mCursorPosition = 0;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// ���������� ���������
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == OIS::KC_PGDOWN) {
			// �� ������ ����, �� �� ������ ����� ������
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top += (mWidgetUpper->getHeight() > mText->getFontHeight()) ? mWidgetUpper->getHeight() : mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// ����� ������ �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLength) {
					mCursorPosition = mTextLength;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// ���������� ���������
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if ( (_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT) ) {
			if ( ! mShiftPressed) {
				mShiftPressed = true;
				if (mStartSelect == ITEM_NONE) {
					mStartSelect = mEndSelect = mCursorPosition;
				}
			}
		}
		else if ( (_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL) ) {
			mCtrlPressed = true;

		}
		else if (_char != 0) {

			// ���� �� ����� ������, �� ������������ ��� �����
			if ( false == mCtrlPressed ) {
				if (false == mModeReadOnly) {
					// ������� ����������� ���� �������
					size_t size = mVectorUndoChangeInfo.size();
					// ��������������� ��������
					deleteTextSelect(true);
					insertText(TextIterator::getTextCharInfo(_char), mCursorPosition, true);
					// ��������� �� ����������� �����������
					if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
					// �������� ������� � ���������
					eventEditTextChange(this);
				}
			}
			else if (_key == OIS::KC_C) {
				commandCopy();

			}
			else if (_key == OIS::KC_X) {
				commandCut();

			}
			else if (_key == OIS::KC_V) {
				commandPast();

			}
			else if (_key == OIS::KC_A) {
				// �������� ���� �����
				setTextSelect(0, mTextLength);

			}
			else if (_key == OIS::KC_Z) {
				// ������
				commandUndo();

			}
			else if (_key == OIS::KC_Y) {
				// ������
				commandRedo();

			}
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyButtonPressed(_key, _char);
	}

	void Edit::_onKeyButtonReleased(int _key)
	{
		if ( (_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT) ) mShiftPressed = false;
		if ( (_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL) ) mCtrlPressed = false;

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyButtonReleased(_key);
	}

	void Edit::_frameEntered(float _frame)
	{
		// � ������� ��� ����������
		if (mModeStatic) return;

		if (mCursorActive) {
			mCursorTimer += _frame;

			if (mCursorTimer > EDIT_CURSOR_TIMER ) {
				mText->setShowCursor(!mText->isCursorShow());
				mCursorTimer -= EDIT_CURSOR_TIMER ;
			}
		}

		// �������� ������ �� ��������� ����
		if (mMouseLeftPressed) {
			mActionMouseTimer += _frame;

			if (mActionMouseTimer > EDIT_ACTION_MOUSE_TIMER ) {

				IntPoint mouse = InputManager::getInstance().getMousePosition();
				const IntRect& view = mWidgetUpper->getAbsoluteRect();
				mouse.left -= view.left;
				mouse.top -= view.top;
				IntPoint point;

				bool action = false;

				// ����� �� ���� �������
				if ( (mouse.top < 0) && (mouse.top > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (mouse.left > 0) && (mouse.left <= mWidgetUpper->getWidth()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.top -= mText->getFontHeight();
						action = true;
					}
				}
				// ���� �� ���� �������
				else if ( (mouse.top > mWidgetUpper->getHeight()) && (mouse.top < (mWidgetUpper->getHeight() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (mouse.left > 0) && (mouse.left <= mWidgetUpper->getWidth()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.top += mText->getFontHeight();
						action = true;
					}
				}
				// ����� �� ��������� ����������
				else if ( (mouse.left < 0) && (mouse.left > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (mouse.top > 0) && (mouse.top <= mWidgetUpper->getHeight()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.left -= (int)EDIT_OFFSET_HORZ_CURSOR;
						action = true;
					}
				}
				// ������ �� ��������� ����������
				else if ( (mouse.left > mWidgetUpper->getWidth()) && (mouse.left < (mWidgetUpper->getWidth() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (mouse.top > 0) && (mouse.top <= mWidgetUpper->getHeight()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.left += (int)EDIT_OFFSET_HORZ_CURSOR;
						action = true;
					}
				}

				if (action) {
					size_t old = mCursorPosition;
					mCursorPosition = mText->getCursorPosition(point);
					//MYGUI_OUT(mCursorPosition);

					if ( old != mCursorPosition ) {

						mText->setCursorPosition(mCursorPosition);

						mEndSelect = (size_t)mCursorPosition;
						if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
						else mText->setTextSelect(mStartSelect, mEndSelect);

						// �������� �������� ������
						updateView(true);
					}

				}
				// ���� � ���� �� �������� �� ����������
				else mActionMouseTimer = 0;

				mActionMouseTimer -= EDIT_ACTION_MOUSE_TIMER ;
			}

		} // if (mMouseLeftPressed)
  }

	void Edit::setTextCursor(size_t _index)
	{
		// ���������� ���������
		resetSelect();

		// ����� �������
		if (_index > mTextLength) _index = mTextLength;
		if (mCursorPosition == _index) return;
		mCursorPosition = _index;

		// ��������� �� �������
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::setTextSelect(size_t _start, size_t _end)
	{
		if (_start > mTextLength) _start = mTextLength;
		if (_end > mTextLength) _end = mTextLength;

		mStartSelect = _start;
		mEndSelect = _end;

		if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
		else mText->setTextSelect(mStartSelect, mEndSelect);

		if (mCursorPosition == mEndSelect) return;
		// ������ �� ����� ���������
		mCursorPosition = mEndSelect;

		// ��������� �� �������
		mText->setCursorPosition(mCursorPosition);
	}

	bool Edit::deleteTextSelect(bool _history)
	{
		if ( ! isTextSelect()) return false;

		// ������ � ����� ���������
		size_t start, end;
		getTextSelect(start, end);

		eraseText(start, end - start, _history);

		return true;
	}

	void Edit::resetSelect()
	{
		if (mStartSelect != ITEM_NONE) {
			mStartSelect = ITEM_NONE;
			mText->setTextSelect(0, 0);
		}
	}

	void Edit::commandPosition(size_t _undo, size_t _redo, size_t _length, VectorChangeInfo * _info)
	{
		if (_info != null) 	_info->push_back(tagChangeInfo(_undo, _redo, _length));
	}

	void Edit::commandMerge()
	{
		if (mVectorUndoChangeInfo.size() < 2) return; // �� ������
		// ��������� ��������� ����� �����
		VectorChangeInfo info = mVectorUndoChangeInfo.back();
		mVectorUndoChangeInfo.pop_back();

		// ���������� ������������������
		for (VectorChangeInfo::iterator iter=info.begin(); iter!=info.end(); iter++) {
			mVectorUndoChangeInfo.back().push_back((*iter));
		}
	}

	bool Edit::commandUndo()
	{
		if (mVectorUndoChangeInfo.empty()) return false;

		// ���������� ���������
		resetSelect();

		// ��������� ��������� ����� �����
		VectorChangeInfo info = mVectorUndoChangeInfo.back();
		// ������������ ��������� ����� �����
		mVectorUndoChangeInfo.pop_back();
		mVectorRedoChangeInfo.push_back(info);

		// ����� ����� ��� �������������
		Ogre::DisplayString text = getRealString();

		// ��������������� ������������������
		for (VectorChangeInfo::reverse_iterator iter=info.rbegin(); iter!=info.rend(); iter++) {

			if ((*iter).type == COMMAND_INSERT) text.erase((*iter).start, (*iter).text.size());
			else if ((*iter).type == COMMAND_ERASE) text.insert((*iter).start, (*iter).text);
			else {
				mCursorPosition = (*iter).undo;
				mTextLength = (*iter).length;
			}
		}

		// ���������� �����
		setRealString(text);

		// ��������� �� �������
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();

		// �������� ������� � ���������
		eventEditTextChange(this);

		return true;
	}

	bool Edit::commandRedo()
	{
		if (mVectorRedoChangeInfo.empty()) return false;

		// ���������� ���������
		resetSelect();

		// ��������� ��������� ����� �����
		VectorChangeInfo info = mVectorRedoChangeInfo.back();
		// ������������ ��������� ����� �����
		mVectorRedoChangeInfo.pop_back();
		mVectorUndoChangeInfo.push_back(info);

		// ����� ����� ��� �������������
		Ogre::DisplayString text = getRealString();

		// ��������������� ������������������
		for (VectorChangeInfo::iterator iter=info.begin(); iter!=info.end(); iter++) {

			if ((*iter).type == COMMAND_INSERT) text.insert((*iter).start, (*iter).text);
			else if ((*iter).type == COMMAND_ERASE) text.erase((*iter).start, (*iter).text.size());
			else {
				mCursorPosition = (*iter).redo;
				mTextLength = (*iter).length;
			}

		}

		// ���������� �����
		setRealString(text);

		// ��������� �� �������
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();

		// �������� ������� � ���������
		eventEditTextChange(this);

		return true;
	}

	void Edit::saveInHistory(VectorChangeInfo * _info)
	{
		if (_info == null) return;
		// ���� ��� ���������� �� ���������
		if ( _info->empty() ) return;
		if ( (_info->size() == 1) && (_info->back().type == COMMAND_POSITION)) return;

		mVectorUndoChangeInfo.push_back(*_info);
		// ��������� �� ������������ ������
		if (mVectorUndoChangeInfo.size() > EDIT_MAX_UNDO)
			mVectorUndoChangeInfo.pop_front();
	}

	// ���������� �����
	Ogre::DisplayString Edit::getText(size_t _start, size_t _count)
	{
		// ������������
		if (_start > mTextLength) _start = mTextLength;
		// ����� ���������
		size_t end = _start + _count;

		// �������� ����� ������
		TextIterator iterator(getRealString());

		// ��������� ����
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());

		// ����� �� ��������� ����
		bool need_colour = true;

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ��� ����
			if (pos < _start) {
				// ����� ���� �� ������� � ����������
				iterator.getTagColour(colour);

				continue;
			}

			// ��������� �� ���������� ���������� ����
			else if (pos == _start) {
				need_colour = ! iterator.getTagColour(colour);
				// ��������� ����� ������ ����������
				iterator.saveStartPoint();

			}

			// � ������ ������ �� ����� ���������
			else if (pos == end) break;

		};

		// ���������� ������
		if (need_colour) return colour + iterator.getFromStart();
		return iterator.getFromStart();
	}

	// �������� ������ ��������
	void Edit::setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour, bool _history)
	{
		// ��� ���������� ����� ���������� ������
		commandResetRedo();

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// ����� ���������
		size_t end = _start + _count;

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� ����
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ����� ���� �� ������� � ����������
			iterator.getTagColour(colour);

			// ��� ����
			if (pos < _start) continue;

			// ������ ��������� ���
			else if (pos == _start)
				iterator.setTagColour(_colour);

			// ������ ��������� ������� ���
			else if (pos < end)
				iterator.clearTagColour();

			// �� ����� ������ ��������� ��������� ��� ���������
			else if (pos == end) {
				iterator.setTagColour(colour);
				// � ������� �� �����
				break;
			}

		};

		// ��������� ������� ��� �������������� �������
		commandPosition(_start, _start+_count, mTextLength, history);

		// ���������� � �������
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// ���������� �������
		else commandResetHistory();

		// � ���������� ������ �� �����
		setRealString(iterator.getText());

	}

	void Edit::setTextSelectColour(const Ogre::ColourValue & _colour, bool _history)
	{
		// ����� ���������
		if ( false == isTextSelect()) return;
		// ������ � ����� ���������
		size_t start, end;
		getTextSelect(start, end);
		setTextColour(start, end-start, _colour, _history);
	}

	Ogre::DisplayString Edit::getSelectedText()
	{
		if ( false == isTextSelect()) return "";
		size_t start, end;
		getTextSelect(start, end);
		return getText(start, end-start);
	}

	void Edit::getTextSelect(size_t & _start, size_t & _end)
	{
		if (mStartSelect == ITEM_NONE) {_start=ITEM_NONE; _end=ITEM_NONE;}
		else if (mStartSelect > mEndSelect) {_start = mEndSelect; _end = mStartSelect;}
		else {_start = mStartSelect; _end = mEndSelect;}
	}

	void Edit::setEditPassword(bool _password)
	{
		if (mModePassword == _password) return;
		mModePassword = _password;
		if (mModePassword) {
			mPasswordText = mText->getCaption();
			mText->setCaption(Ogre::DisplayString(mTextLength, '*'));
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

	void Edit::setText(const Ogre::DisplayString & _caption, bool _history)
	{
		// ���������� ���������
		resetSelect();

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� �����
		iterator.setText(_caption, mModeMultiline);

		// �������� �� ������������ ������
		iterator.cutMaxLength(EDIT_MAX_LENGHT);

		// ���������� ������ ������
		size_t old = mTextLength;
		// ����� ������� � ��������� �� ����� �������
		mCursorPosition = mTextLength = iterator.getSize();

		// ��������� ������� ��� �������������� �������
		commandPosition(0, mTextLength, old, history);

		// ���������� � �������
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// ���������� �������
		else commandResetHistory();

		// � ���������� ������ �� �����
		setRealString(iterator.getText());

		// ��������� �� �������
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::insertText(const Ogre::DisplayString & _text, size_t _start, bool _history)
	{
		// ���������� ���������
		resetSelect();

		// ���� ������ ������, ��� ������ ����������
		if ((_text.empty()) || (mTextLength == EDIT_MAX_LENGHT) ) return;

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� ����
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());
		// ����� �� ��� ������
		// ����� ���������� ����� TextIterator ����� �������� ������� ��� �� �����
		bool need_colour = ( (_text.size() > 6) && (_text[0] == _T('#')) && (_text[1] != _T('#')) );

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ������� ����
			if (need_colour) iterator.getTagColour(colour);

			// ���� ����� �� �������
			if (pos == _start) break;

		};

		// ���� ����� ���� �� ���������
		if (need_colour) iterator.setTagColour(colour);

		// � ������ ��������� ������
		iterator.insertText(_text, mModeMultiline);

		// �������� �� ������������ ������
		iterator.cutMaxLength(EDIT_MAX_LENGHT);

		// ���������� ������ ������
		size_t old = mTextLength;
		// ����� ������� � ��������� �� ����� �������
		mTextLength = iterator.getSize();
		mCursorPosition += mTextLength - old;

		// ��������� ������� ��� �������������� �������
		commandPosition(_start, _start + mTextLength - old, old, history);

		// ���������� � �������
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// ���������� �������
		else commandResetHistory();

		// � ���������� ������ �� �����
		setRealString(iterator.getText());

		// ��������� �� �������
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::eraseText(size_t _start, size_t _count, bool _history)
	{
		// ���� ��������
		if (_count == 0) return;

		// ���������� ���������
		resetSelect();

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� ����
		Ogre::DisplayString colour;
		// ����� ���������
		size_t end = _start + _count;
		bool need_colour = false;

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ��� ����
			if (pos < _start) {
				// ����� ���� �� ������� � ����������
				iterator.getTagColour(colour);
				continue;
			}

			// ��������� ����� ������ ����������
			else if (pos == _start) {
				// ���� �� ��������� ��� ����, �� ����� ������� ���
				if ( ! colour.empty()) {
					need_colour = true;
					colour.clear();
				}
				// ����� ���� �� ������� � ����������
				iterator.getTagColour(colour);
				iterator.saveStartPoint();
			}

			// ������ ���������
			else if (pos < end) {
				// ����� ���� �� ������� � ����������
				iterator.getTagColour(colour);
			}

			// ��������� ���������
			else if (pos == end) {
				// ����� ������� ��� ��� ���
				if ( ! colour.empty()) need_colour = true;
				if ( iterator.getTagColour(colour)) need_colour = false;

				break;
			}

		};

		// ������� ��������
		iterator.eraseFromStart();
		// � ��������� ��������� ����
		if (need_colour) iterator.setTagColour(colour);

		// ��������� ������� ��� �������������� �������
		commandPosition(_start + _count, _start, mTextLength, history);

		// �� ����� ����������
		mCursorPosition = _start;
		mTextLength -= _count;

		// ���������� � �������
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// ���������� �������
		else commandResetHistory();

		// � ���������� ������ �� �����
		setRealString(iterator.getText());

		// ��������� �� �������
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::commandCut()
	{
		// �������� � ����� ������
		if ( isTextSelect() && (false == mModePassword) ) {
			ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getSelectedText());
			if (false == mModeReadOnly) {
				deleteTextSelect(true);
				// �������� ������� � ���������
				eventEditTextChange(this);
			}
		}
		else ClipboardManager::getInstance().ClearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void Edit::commandCopy()
	{
		// �������� � ����� ������
		if ( isTextSelect() && (false == mModePassword) ) ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getSelectedText());
		else ClipboardManager::getInstance().ClearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void Edit::commandPast()
	{
		// �������� �� ������ ������
		std::string clipboard = ClipboardManager::getInstance().GetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
		if ( (false == mModeReadOnly) && ( false == clipboard.empty()) ) {
			// ������� ����������� ���� �������
			size_t size = mVectorUndoChangeInfo.size();
			// ��������������� ��������
			deleteTextSelect(true);
			insertText(clipboard, mCursorPosition, true);
			// ��������� �� ����������� �����������
			if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
			// �������� ������� � ���������
			eventEditTextChange(this);
		}
	}

	const Ogre::DisplayString & Edit::getRealString()
	{
		if (mModePassword) return mPasswordText;
		return mText->getCaption();
	}

	void Edit::setRealString(const Ogre::DisplayString & _caption)
	{
		if (mModePassword) {
			mPasswordText = _caption;
			mText->setCaption(Ogre::DisplayString(mTextLength, '*'));
		}
		else {
			mText->setCaption(_caption);
		}
	}

	void Edit::updateEditState()
	{
		if (mIsFocus) {
			if (mIsPressed) setState("select");
			else setState("active");
		} else {
			if (mIsPressed) setState("pressed");
			else setState("normal");
		}
	}

	void Edit::setPosition(const IntCoord& _coord)
	{
		Widget::setPosition(_coord);
		updateView(false);
	}

	void Edit::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
		updateView(false);
	}

	void Edit::setCaption(const Ogre::DisplayString & _caption)
	{
		setText(_caption, false);
	}

	const Ogre::DisplayString& Edit::getCaption()
	{
		return getRealString();
	}

	void Edit::updateSelectText()
	{
		if (mModeStatic) return;

		if ( (mShiftPressed) && (mStartSelect != ITEM_NONE) ) {
			// ������ ���������
			mEndSelect = (size_t)mCursorPosition;
			if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
			else mText->setTextSelect(mStartSelect, mEndSelect);

		} else if (mStartSelect != ITEM_NONE) {
			// ���������� ����
			mStartSelect = ITEM_NONE;
			mText->setTextSelect(0, 0);
		}

		// �������� �������� ������
		updateView(true);
	}

	void Edit::updateView(bool _showCursor)
	{
		// ������ ��������� ������
		IntSize textSize = mText->getTextSize();
		// ������� �������� ��������� ������
		IntPoint point = mText->getViewOffset();
		// ��������� ��������
		IntPoint offset = point;

		// ���������� ���������� �������
		IntRect cursor = mText->getCursorRect(mCursorPosition);
		cursor.right ++;
		// ���������� ���������� ���
		const IntRect& view = mWidgetUpper->getAbsoluteRect();

		// ��������� ��������� �������
		bool inside = view.inside(cursor);

		// ��������� � ���������� ������
		if (_showCursor && ( false == inside)) {

			// �������������� ��������
			if (textSize.width >= view.width()) {
				if (cursor.left < view.left) {
					offset.left = point.left - (view.left - cursor.left);
					// ��������� ��������, ������ ���� ������ �� �����������
					if ((view.width() - EDIT_OFFSET_HORZ_CURSOR) > EDIT_OFFSET_HORZ_CURSOR) offset.left -= EDIT_OFFSET_HORZ_CURSOR;
				}
				else if (cursor.right > view.right) {
					offset.left = point.left + (cursor.right - view.right);
					// ��������� ��������, ������ ���� ������ �� �����������
					if ((view.width() - EDIT_OFFSET_HORZ_CURSOR) > EDIT_OFFSET_HORZ_CURSOR) offset.left += EDIT_OFFSET_HORZ_CURSOR;
				}
			}

			// ������������ ��������
			if (textSize.height >= view.height()) {
				if (cursor.top < view.top) {
					offset.top = point.top - (view.top - cursor.top);
				}
				else if (cursor.bottom > view.bottom) {
					offset.top = point.top + (cursor.bottom - view.bottom);
				}
			}

		}

		// ������������ ������
		Align align = mText->getTextAlign();

		if (textSize.width >= view.width()) {
			// ������������ ����� �����
			if ((offset.left + view.width()) > textSize.width) {
				offset.left = textSize.width - view.width();
			}
			// ������������ ����� ������
			else if (offset.left < 0) {
				offset.left = 0;
			}
		}
		else {
			if (IS_ALIGN_LEFT(align)) offset.left = 0;
			else if (IS_ALIGN_RIGHT(align)) offset.left = textSize.width - view.width();
			else if (IS_ALIGN_HCENTER(align)) offset.left = (textSize.width - view.width()) / 2;
		}

		if (textSize.height >= view.height()) {
			// ������������ ����� �����
			if ((offset.top + view.height()) > textSize.height) {
				offset.top = textSize.height - view.height();
			}
			// ������������ ����� ����
			else if (offset.top < 0) {
				offset.top = 0;
			}
		}
		else {
			if (IS_ALIGN_TOP(align)) offset.top = 0;
			else if (IS_ALIGN_BOTTOM(align)) offset.top = textSize.height - view.height();
			else if (IS_ALIGN_VCENTER(align)) offset.top = (textSize.height - view.height()) / 2;
		}
		
		if (offset != point) mText->setViewOffset(offset);
	}

	void Edit::setTextAlign(Align _align)
	{
		Widget::setTextAlign(_align);
		// ��� ��� �� ���� ����� ����������
		updateView(false);
	}

} // namespace MyGUI
