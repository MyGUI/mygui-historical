
#include "Edit.h"
#include "EditParser.h"
#include "TextIterator.h"
#include "ClipboardManager.h"

namespace MyGUI
{
	// ������� ������� ��� ����� �������
	namespace factory { WidgetFactory<Edit> EditFactoryInstance("Edit"); }
	// ������ ������
	namespace parser { EditParser EditParserInstance; }

	const float EDIT_CURSOR_TIMER  = 0.7f;
	const float EDIT_ACTION_MOUSE_TIMER  = 0.20f;
	const int EDIT_CURSOR_MAX_POSITION = 100000;
	const int EDIT_CURSOR_MIN_POSITION = -100000;
	const size_t EDIT_MAX_UNDO = 128;
	const size_t EDIT_MAX_LENGHT = 2048;
	const float EDIT_OFFSET_HORZ_CURSOR = 50.0f; // �������������� �������� ��� �������
	const int EDIT_ACTION_MOUSE_ZONE = 50; // ������� ��� ���������� ���� �� �������� �����
	const std::string EDIT_CLIPBOARD_TYPE_TEXT = "Text";

	Edit::Edit(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		m_isPressed(false),
		m_isFocus(false),
		mCursorTimer(0),
		mCursorActive(false),
		mCursorPosition(0),
		mTextLenght(0),
		mShiftPressed(false),
		mCtrlPressed(false),
		mStartSelect(SIZE_MAX),
		mHalfWidthCursor(1),
		mHalfHeightCursor(1),
		mMouseLeftPressed(false),
		mActionMouseTimer(0),
		mReadOnly(false),
		mPassword(false),
		mMultiLine(true)
	{

		ASSERT(m_text);

		// ������� ���������, ����� ������ ��� ��� �������
		mWidgetUpper = createWidget("Widget", "Empty", m_text->left(), m_text->top(), m_text->width(), m_text->height(), m_text->getAlign());
		mWidgetUpper->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
		mWidgetUpper->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
		mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
		mWidgetUpper->eventMouseButtonReleased = newDelegate(this, &Edit::notifyMouseReleased);
		mWidgetUpper->eventMouseMove = newDelegate(this, &Edit::notifyMouseMove);

		// ������ ��������
		const SkinParam & param = _info->getParams();
		SkinParam::const_iterator iter = param.find("CursorSkin");
		String skin = "Default";
		if (iter != param.end()) skin = iter->second;

		// ������� ������
		WidgetSkinInfo * info = SkinManager::getInstance().getSkin(skin);
		mWidgetCursor = mWidgetUpper->createWidget("Widget", skin, 0, 0, info->getSize().width, getFontHeight(), ALIGN_LEFT | ALIGN_TOP);
		mWidgetCursor->show(false);
		mWidgetCursor->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
		mWidgetCursor->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
		mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
		mWidgetCursor->eventMouseButtonReleased = newDelegate(this, &Edit::notifyMouseReleased);
		mWidgetCursor->eventMouseMove = newDelegate(this, &Edit::notifyMouseMove);

		// ����������� ���������
		mHalfWidthCursor = (mWidgetCursor->width()/2);
		if (mHalfWidthCursor < 1) mHalfWidthCursor = 1;
		mHalfHeightCursor = (mWidgetCursor->height()/2);
		if (mHalfHeightCursor < 1) mHalfHeightCursor = 1;

	}

	void Edit::notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		if ( (_old == this) || (_old == mWidgetUpper) || (_old == mWidgetCursor) || (m_isFocus) ) return;
		m_isFocus = true;
		updateEditState();
	}

	void Edit::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		if ( (_new == this) || (_new == mWidgetUpper) || (_new == mWidgetCursor) || (!m_isFocus) ) return;
		m_isFocus = false;
		updateEditState();
	}

	void Edit::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		InputManager::getInstance().setKeyFocusWidget(this);

		IntPoint point = InputManager::getInstance().getLastLeftPressed();
		mCursorPosition = m_text->getTextCursorFromPoint(point);
		updateCursor(point);

		if (_left) mMouseLeftPressed = true;
	}

	void Edit::notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		// ���������� ������
		mMouseLeftPressed = false;
	}

	void Edit::notifyMouseMove(MyGUI::WidgetPtr _sender, int _x, int _y)
	{
		// ������������� ������
		if ( ! mWidgetCursor->isShow()) mWidgetCursor->show(true);

		// ���������� ��� �������
		mCursorTimer = 0;
		mActionMouseTimer = 0;

		size_t Old = mCursorPosition;
		IntPoint point(_x, _y);
		mCursorPosition = m_text->getTextCursorFromPoint(point);
		if (Old == mCursorPosition) return;

		WidgetPtr parent = mWidgetUpper;
		while (parent != null) {
			point.left -= parent->left();
			point.top -= parent->top();
			parent = parent->getParent();
		}
		mWidgetCursor->move(point.left, point.top);

		// ���� �� ���� ���������
		if (mStartSelect == SIZE_MAX) mStartSelect = Old;

		// ������ ���������
		mEndSelect = (size_t)mCursorPosition;
		if (mStartSelect > mEndSelect) m_text->setTextSelect(mEndSelect, mStartSelect);
		else m_text->setTextSelect(mStartSelect, mEndSelect);

	}

	void Edit::_onMouseSetFocus(WidgetPtr _old)
	{
		Widget::_onMouseSetFocus(_old);
		if (m_isFocus) return;
		m_isFocus = true;
		updateEditState();
	}

	void Edit::_onMouseLostFocus(WidgetPtr _new)
	{
		Widget::_onMouseLostFocus(_new);
		if ( (!m_isFocus) || (_new == mWidgetUpper) || (_new == mWidgetCursor) ) return;
		m_isFocus = false;
		updateEditState();
	}

	void Edit::_onMouseMove(int _x, int _y)
	{
		Widget::_onMouseMove(_x, _y);
		notifyMouseMove(null, _x, _y);
	}

	void Edit::_onKeySetFocus(WidgetPtr _old)
	{
		Widget::_onKeySetFocus(_old);
		if (m_isPressed) return;
		m_isPressed = true;
		updateEditState();

		mCursorActive = true;
		Ogre::Root::getSingleton().addFrameListener(this);
		mWidgetCursor->show(true);
		mCursorTimer = 0;
	}

	void Edit::_onKeyLostFocus(WidgetPtr _new)
	{
		Widget::_onKeyLostFocus(_new);
		if ( ! m_isPressed) return;
		m_isPressed = false;
		updateEditState();

		mCursorActive = false;
		Ogre::Root::getSingleton().removeFrameListener(this);
		mWidgetCursor->show(false);
	}

	void Edit::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		Widget::_onKeyButtonPressed(_key, _char);
		ASSERT(m_text);

		if ( ! mWidgetCursor->isShow()) mWidgetCursor->show(true);
		mCursorTimer = 0.0f;

		if (_key == OIS::KC_ESCAPE) InputManager::getInstance().setKeyFocusWidget(null);
		else if (_key == OIS::KC_BACK) {
			// ���� ���� �� ������� ���������� �����
			if ( (false == mReadOnly) && (false == deleteTextSelect(true)) ) {
				// ������� �� ���� ����� � �������
				if (mCursorPosition != 0) {
					mCursorPosition -- ;
					eraseText(mCursorPosition, 1, true);
				}
			}

		} else if (_key == OIS::KC_DELETE) {
			if (mShiftPressed) commandCut();
			else if (false == mReadOnly) {
				// ���� ���� �� ������� ���������� �����
				if (false == deleteTextSelect(true)) {
					if (mCursorPosition != mTextLenght) {
						eraseText(mCursorPosition, 1, true); 
					}
				}
			}

		} else if (_key == OIS::KC_INSERT) {
			if (mShiftPressed) commandPast();
			else if (mCtrlPressed) commandCopy();

		} else if (_key == OIS::KC_RETURN) {
			if ((false == mReadOnly) && (mMultiLine) ) {
				// ������� ����������� ���� �������
				size_t size = mVectorUndoChangeInfo.size();
				// ��������������� ��������
				deleteTextSelect(true);
				insertText(TextIterator::getTextNewLine(), mCursorPosition, true);
				// ��������� �� ����������� �����������
				if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
			}

		} else if (_key == OIS::KC_RIGHT) {
			if ((mCursorPosition) < mTextLenght) {
				mCursorPosition ++;
				IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
				updateCursor(point);
			}
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_LEFT) {
			if (mCursorPosition != 0) {
				mCursorPosition --;
				IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
				updateCursor(point);
			}
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_UP) {
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top -= mHalfHeightCursor;
			size_t old = mCursorPosition;
			mCursorPosition = m_text->getTextCursorFromPoint(point);
			// ����� ������� �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					point.left = EDIT_CURSOR_MIN_POSITION;
					mCursorPosition = m_text->getTextCursorFromPoint(point);
					updateCursor(point);
				}
				// ���������� ���������
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			} else updateCursor(point);

		} else if (_key == OIS::KC_DOWN) {
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top += getFontHeight() + mHalfHeightCursor;
			size_t old = mCursorPosition;
			mCursorPosition = m_text->getTextCursorFromPoint(point);
			// ����� ������ �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLenght) {
					point.left = EDIT_CURSOR_MAX_POSITION;
					point.top += mHalfHeightCursor;
					mCursorPosition = m_text->getTextCursorFromPoint(point);
					updateCursor(point);
				}
				// ���������� ���������
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			} else updateCursor(point);

		} else if (_key == OIS::KC_HOME) {
			IntPoint point(EDIT_CURSOR_MIN_POSITION, EDIT_CURSOR_MIN_POSITION);
			if ( false == mCtrlPressed) {
				// � ������ ������
				point = getWorldPostion(mWidgetCursor);
				point.left = EDIT_CURSOR_MIN_POSITION;
				point.top += mHalfHeightCursor;
			}
			size_t old = mCursorPosition;
			mCursorPosition = m_text->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_END) {
			IntPoint point(EDIT_CURSOR_MAX_POSITION, EDIT_CURSOR_MAX_POSITION);
			if ( !  mCtrlPressed) {
				// � ����� ������
				point = getWorldPostion(mWidgetCursor);
				point.left = EDIT_CURSOR_MAX_POSITION;
				point.top += mHalfHeightCursor;
			}
			size_t old = mCursorPosition;
			mCursorPosition = m_text->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_PGUP) {
			// �� ������ ����, �� �� ������ ����� ������
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top -= (mHalfHeightCursor > mWidgetUpper->height()) ? mHalfHeightCursor : mWidgetUpper->height();
			size_t old = mCursorPosition;
			mCursorPosition = m_text->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_PGDOWN) {
			// �� ������ ����, �� �� ������ ����� ������
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top += (mHalfHeightCursor > mWidgetUpper->height()) ? (mHalfHeightCursor + getFontHeight()) : mWidgetUpper->height();
			size_t old = mCursorPosition;
			mCursorPosition = m_text->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		/*} else if (_key == OIS::KC_F1) {
			setTextSelectColor(Ogre::ColourValue::Black, true);

		} else if (_key == OIS::KC_F2) {
			setTextSelectColor(Ogre::ColourValue::Red, true);

		} else if (_key == OIS::KC_F3) {
			setTextSelectColor(Ogre::ColourValue::Blue, true);

		} else if (_key == OIS::KC_F4) {
			setTextSelectColor(Ogre::ColourValue::Green, true);

		} else if (_key == OIS::KC_F5) {
			setTextSelectColor(Ogre::ColourValue::White, true);

		} else if (_key == OIS::KC_F6) {
			m_text->setTextAlign(ALIGN_LEFT | ALIGN_TOP);
			IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
			updateCursor(point);

		} else if (_key == OIS::KC_F7) {
			m_text->setTextAlign(ALIGN_CENTER);
			IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
			updateCursor(point);

		} else if (_key == OIS::KC_F8) {
			m_text->setTextAlign(ALIGN_RIGHT | ALIGN_BOTTOM);
			IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
			updateCursor(point);*/

		} else if ( (_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT) ) {
			if ( ! mShiftPressed) {
				mShiftPressed = true;
				if (mStartSelect == SIZE_MAX) {
					mStartSelect = mEndSelect = mCursorPosition;
				}
			}
		} else if ( (_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL) ) {
			mCtrlPressed = true;

		} else if (_char != 0) {

			// ���� �� ����� ������, �� ������������ ��� �����
			if ( false == mCtrlPressed ) {
				if (false == mReadOnly) {
					// ������� ����������� ���� �������
					size_t size = mVectorUndoChangeInfo.size();
					// ��������������� ��������
					deleteTextSelect(true);
					insertText(TextIterator::getTextCharInfo(_char), mCursorPosition, true);
					// ��������� �� ����������� �����������
					if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
				}
			} else if (_key == OIS::KC_C) {
				commandCopy();

			} else if (_key == OIS::KC_X) {
				commandCut();

			} else if (_key == OIS::KC_V) {
				commandPast();

			} else if (_key == OIS::KC_A) {
				// �������� ���� �����
				setTextSelect(0, mTextLenght);

			} else if (_key == OIS::KC_Z) {
				// ������
				commandUndo();

			} else if (_key == OIS::KC_Y) {
				// ������
				commandRedo();

			}

		}
	}

	void Edit::_onKeyButtonReleased(int _key)
	{
		Widget::_onKeyButtonReleased(_key);
		if ( (_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT) ) mShiftPressed = false;
		if ( (_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL) ) mCtrlPressed = false;
	}

	bool Edit::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (mCursorActive) {
			mCursorTimer += evt.timeSinceLastFrame;

			if (mCursorTimer > EDIT_CURSOR_TIMER ) {
				mWidgetCursor->show( ! mWidgetCursor->isShow());
				mCursorTimer -= EDIT_CURSOR_TIMER ;
			}
		}

		// �������� ������ �� ��������� ����
		if (mMouseLeftPressed) {
			mActionMouseTimer += evt.timeSinceLastFrame;

			if (mActionMouseTimer > EDIT_ACTION_MOUSE_TIMER ) {
				
				IntPoint point = InputManager::getInstance().getMousePosition();

				WidgetPtr parent = mWidgetUpper;
				while (parent != null) {
					point.left -= parent->left();
					point.top -= parent->top();
					parent = parent->getParent();
				}

				bool action = false;

				// ����� �� ���� �������
				if ( (point.top < 0) && (point.top > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (point.left > 0) && (point.left <= mWidgetUpper->width()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.top -= mHalfHeightCursor;
						action = true;
					}
				}
				// ���� �� ���� �������
				else if ( (point.top > mWidgetUpper->height()) && (point.top < (mWidgetUpper->height() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (point.left > 0) && (point.left <= mWidgetUpper->width()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.top += getFontHeight() + mHalfHeightCursor;
						action = true;
					}
				}
				// ����� �� ��������� ����������
				else if ( (point.left < 0) && (point.left > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (point.top > 0) && (point.top <= mWidgetUpper->height()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.left -= EDIT_OFFSET_HORZ_CURSOR;
						point.top += mHalfHeightCursor;
						action = true;
					}
				}
				// ������ �� ��������� ����������
				else if ( (point.left > mWidgetUpper->width()) && (point.left < (mWidgetUpper->width() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (point.top > 0) && (point.top <= mWidgetUpper->height()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.left += EDIT_OFFSET_HORZ_CURSOR;
						point.top += mHalfHeightCursor;
						action = true;
					}
				}

				if (action) {
					size_t old = mCursorPosition;
					mCursorPosition = m_text->getTextCursorFromPoint(point);
					if ( old != mCursorPosition ) {

						WidgetPtr parent = mWidgetUpper;
						while (parent != null) {
							point.left -= parent->left();
							point.top -= parent->top();
							parent = parent->getParent();
						}
						mWidgetCursor->move(point.left, point.top);

						mEndSelect = (size_t)mCursorPosition;
						if (mStartSelect > mEndSelect) m_text->setTextSelect(mEndSelect, mStartSelect);
						else m_text->setTextSelect(mStartSelect, mEndSelect);

					}
					// �������� �������� ������
					updateView(true);

				}
				// ���� � ���� �� �������� �� ����������
				else mActionMouseTimer = 0;

				mActionMouseTimer -= EDIT_ACTION_MOUSE_TIMER ;
			}

		} // if (mMouseLeftPressed)

		return true;
	}

	bool Edit::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	}

	void Edit::setTextCursor(size_t _index)
	{
		// ���������� ���������
		resetSelect();

		// ����� �������
		if (_index > mTextLenght) _index = mTextLenght;
		if (mCursorPosition == _index) return;
		mCursorPosition = _index;

		// ��������� �� �������
		IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::setTextSelect(size_t _start, size_t _end)
	{
		if (_start > mTextLenght) _start = mTextLenght;
		if (_end > mTextLenght) _end = mTextLenght;
		
		mStartSelect = _start;
		mEndSelect = _end;

		if (mStartSelect > mEndSelect) m_text->setTextSelect(mEndSelect, mStartSelect);
		else m_text->setTextSelect(mStartSelect, mEndSelect);

		if (mCursorPosition == mEndSelect) return;
		// ������ �� ����� ���������
		mCursorPosition = mEndSelect;

		// ��������� �� �������
		IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);

		WidgetPtr parent = mWidgetUpper;
		while (parent != null) {
			point.left -= parent->left();
			point.top -= parent->top();
			parent = parent->getParent();
		}
		mWidgetCursor->move(point.left, point.top);
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

	void Edit::commandPosition(size_t _undo, size_t _redo, size_t _lenght, VectorChangeInfo * _info)
	{
		if (_info != null) 	_info->push_back(tagChangeInfo(_undo, _redo, _lenght));
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
				mTextLenght = (*iter).lenght;
			}
		}

		// ���������� �����
		setRealString(text);

		// ��������� �� �������
		IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

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
				mTextLenght = (*iter).lenght;
			}

		}

		// ���������� �����
		setRealString(text);

		// ��������� �� �������
		IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

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
		if (_start > mTextLenght) _start = mTextLenght;
		// ����� ���������
		size_t end = _start + _count;

		// �������� ����� ������
		TextIterator iterator(getRealString());

		// ��������� ����
		Ogre::DisplayString color = TextIterator::convertTagColor(m_text->getColour());

		// ����� �� ��������� ����
		bool need_color = true;

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ��� ����
			if (pos < _start) {
				// ����� ���� �� ������� � ����������
				iterator.getTagColor(color);

				continue;
			}

			// ��������� �� ���������� ���������� ����
			else if (pos == _start) {
				need_color = ! iterator.getTagColor(color);
				// ��������� ����� ������ ����������
				iterator.saveStartPoint();

			}

			// � ������ ������ �� ����� ���������
			else if (pos == end) break;

		};

		// ���������� ������
		if (need_color) return color + iterator.getFromStart();
		return iterator.getFromStart();
	}

	// �������� ������ ��������
	void Edit::setTextColor(size_t _start, size_t _count, const Ogre::ColourValue & _color, bool _history)
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
		Ogre::DisplayString color = TextIterator::convertTagColor(m_text->getColour());

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ����� ���� �� ������� � ����������
			iterator.getTagColor(color);

			// ��� ����
			if (pos < _start) continue;

			// ������ ��������� ���
			else if (pos == _start)
				iterator.setTagColor(_color);

			// ������ ��������� ������� ���
			else if (pos < end)
				iterator.clearTagColor();

			// �� ����� ������ ��������� ��������� ��� ���������
			else if (pos == end) {
				iterator.setTagColor(color);
				// � ������� �� �����
				break;
			}

		};

		// ��������� ������� ��� �������������� �������
		commandPosition(_start, _start+_count, mTextLenght, history);

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

	void Edit::updateCursor(IntPoint _point)
	{
		OUT(mCursorPosition);

		WidgetPtr parent = mWidgetUpper;
		while (parent != null) {
			_point.left -= parent->left();
			_point.top -= parent->top();
			parent = parent->getParent();
		}

		// ���� �� ����������� ������ ���������� �����, �� ����� �������� ����
		if (_point.left == -mHalfWidthCursor) _point.left = 0;
		else if (_point.left == mWidgetUpper->width()-mHalfWidthCursor) _point.left = mWidgetUpper->width() - mWidgetCursor->width();
		mWidgetCursor->move(_point.left, _point.top);

		if ( (mShiftPressed) && (mStartSelect != SIZE_MAX) ) {
			// ������ ���������
			mEndSelect = (size_t)mCursorPosition;
			if (mStartSelect > mEndSelect) m_text->setTextSelect(mEndSelect, mStartSelect);
			else m_text->setTextSelect(mStartSelect, mEndSelect);

		} else if (mStartSelect != SIZE_MAX) {
			// ���������� ����
			mStartSelect = SIZE_MAX;
			m_text->setTextSelect(0, 0);
		}

		// �������� �������� ������
		updateView(true);
	}

	bool Edit::isShowCursorInView()
	{
		return ( (mWidgetCursor->left() >= 0) 
			&& (mWidgetCursor->top() >= 0) 
			&& (mWidgetCursor->right() <= mWidgetUpper->width()) 
			&& (mWidgetCursor->bottom() <= mWidgetUpper->height()) );
	}

	void Edit::updateView(bool _showCursor)
	{
		// ������ ���������
		mSizeView = m_text->getTextSize();
		// ������� �������
		IntPoint point = m_text->getTextShift();
		// ��������� ��������
		IntPoint offset;


		// ��������� � ���������� ������
		if (_showCursor && ( false == isShowCursorInView())) {

			// �������������� ��������
			if (mSizeView.width > mWidgetUpper->width()) {
				if (mWidgetCursor->left() < 0) {
					offset.left = mWidgetCursor->left() - EDIT_OFFSET_HORZ_CURSOR;
				}
				else if (mWidgetCursor->right() > mWidgetUpper->width()) {
					offset.left = mWidgetCursor->right() - mWidgetUpper->width() + EDIT_OFFSET_HORZ_CURSOR;
				}
			}

			// ������������ ��������
			if (mSizeView.height > mWidgetUpper->height()) {
				if (mWidgetCursor->top() < 0) {
					offset.top = mWidgetCursor->top();
				}
				else if (mWidgetCursor->bottom() > mWidgetUpper->height()) {
					offset.top = mWidgetCursor->bottom() - mWidgetUpper->height();
				}
			}

		}

		if (mSizeView.width > mWidgetUpper->width()) {

			// ������������ ����� �����
			if ((point.left + offset.left) < mHalfWidthCursor) {
				offset.left = - (mHalfWidthCursor + point.left);
			}
			// ������������ ����� ������
			else if ( (point.left + offset.left) > (mSizeView.width-mWidgetUpper->width()+mHalfWidthCursor) ) {
				offset.left = (mSizeView.width-mWidgetUpper->width()+mHalfWidthCursor) - point.left;
			}
		}

		if (mSizeView.height > mWidgetUpper->height()) {

			// ������������ ����� �����
			if ((point.top + offset.top) < 0) {
				offset.top = - point.top;
			}
			// ������������ ����� ����
			else if ( (point.top + offset.top) > (mSizeView.height-mWidgetUpper->height()) ) {
				offset.top = (mSizeView.height-mWidgetUpper->height()) - point.top;
			}
		}

		// ������ �� ����������
		if (offset.empty()) return;

		mWidgetCursor->move(mWidgetCursor->left() - offset.left, mWidgetCursor->top() - offset.top);

		point.left += offset.left;
		point.top += offset.top;
		m_text->setTextShift(point);

		//OUT("OFFSET : ", point.left, " ,  ", point.top);

	}

	void Edit::setTextSelectColor(const Ogre::ColourValue & _color, bool _history)
	{
		// ����� ���������
		if ( false == isTextSelect()) return;
		// ������ � ����� ���������
		size_t start, end;
		getTextSelect(start, end);
		setTextColor(start, end-start, _color, _history);
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
		if (mStartSelect == SIZE_MAX) {_start=SIZE_MAX; _end=SIZE_MAX;}
		else if (mStartSelect > mEndSelect) {_start = mEndSelect; _end = mStartSelect;}
		else {_start = mStartSelect; _end = mEndSelect;}
	}

	void Edit::setCaption(const Ogre::DisplayString & _caption)
	{
		setText(_caption, false);
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
		iterator.setText(_caption, mMultiLine);

		// �������� �� ������������ ������
		iterator.cutMaxLenght(EDIT_MAX_LENGHT);

		// ���������� ������ ������
		size_t old = mTextLenght;
		// ����� ������� � ��������� �� ����� �������
		mCursorPosition = mTextLenght = iterator.getSize();

		// ��������� ������� ��� �������������� �������
		commandPosition(0, mTextLenght, old, history);

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
		IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::insertText(const Ogre::DisplayString & _text, size_t _start, bool _history)
	{
		// ���������� ���������
		resetSelect();

		// ���� ������ ������, ��� ������ ����������
		if ((_text.empty()) || (mTextLenght == EDIT_MAX_LENGHT) ) return;

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� ����
		Ogre::DisplayString color = TextIterator::convertTagColor(m_text->getColour());
		// ����� �� ��� ������
		bool need_color = ( (_text.size() > 6) && (_text[0] == '#') && (_text[1] != '#') );

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ������� ����
			if (need_color) iterator.getTagColor(color);

			// ���� ����� �� �������
			if (pos == _start) break;

		};

		// ���� ����� ���� �� ���������
		if (need_color) iterator.setTagColor(color);

		// � ������ ��������� ������
		iterator.insertText(_text, mMultiLine);

		// �������� �� ������������ ������
		iterator.cutMaxLenght(EDIT_MAX_LENGHT);

		// ���������� ������ ������
		size_t old = mTextLenght;
		// ����� ������� � ��������� �� ����� �������
		mTextLenght = iterator.getSize();
		mCursorPosition += mTextLenght - old;

		// ��������� ������� ��� �������������� �������
		commandPosition(_start, _start + mTextLenght - old, old, history);

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
		IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
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
		Ogre::DisplayString color;
		// ����� ���������
		size_t end = _start + _count;
		bool need_color = false;

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ��� ����
			if (pos < _start) {
				// ����� ���� �� ������� � ����������
				iterator.getTagColor(color);
				continue;
			}

			// ��������� ����� ������ ����������
			else if (pos == _start) {
				// ���� �� ��������� ��� ����, �� ����� ������� ���
				if ( ! color.empty()) {
					need_color = true;
					color.clear();
				}
				// ����� ���� �� ������� � ����������
				iterator.getTagColor(color);
				iterator.saveStartPoint();
			}

			// ������ ���������
			else if (pos < end) {
				// ����� ���� �� ������� � ����������
				iterator.getTagColor(color);
			}

			// ��������� ���������
			else if (pos == end) {
				// ����� ������� ��� ��� ���
				if ( ! color.empty()) need_color = true;
				if ( iterator.getTagColor(color)) need_color = false;

				break;
			}

		};

		// ������� ��������
		iterator.eraseFromStart();
		// � ��������� ��������� ����
		if (need_color) iterator.setTagColor(color);

		// ��������� ������� ��� �������������� �������
		commandPosition(_start + _count, _start, mTextLenght, history);

		// �� ����� ����������
		mCursorPosition = _start;
		mTextLenght -= _count;

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
		IntPoint point = m_text->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::commandCut()
	{
		// �������� � ����� ������
		if ( isTextSelect() ) {
			ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getSelectedText());
			if (false == mReadOnly) deleteTextSelect(true);
		}
		else ClipboardManager::getInstance().ClearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void Edit::commandCopy()
	{
		// �������� � ����� ������
		if ( isTextSelect() ) ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getSelectedText());
		else ClipboardManager::getInstance().ClearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void Edit::commandPast()
	{
		// �������� �� ������ ������
		std::string clipboard = ClipboardManager::getInstance().GetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
		if ( (false == mReadOnly) && ( false == clipboard.empty()) ) {
			// ������� ����������� ���� �������
			size_t size = mVectorUndoChangeInfo.size();
			// ��������������� ��������
			deleteTextSelect(true);
			insertText(clipboard, mCursorPosition, true);
			// ��������� �� ����������� �����������
			if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
		}
	}

} // namespace MyGUI