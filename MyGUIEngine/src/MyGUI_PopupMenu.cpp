/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_PopupMenu.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Button.h"
#include "MyGUI_WidgetManager.h"

#include "MyGUI_ControllerManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	const float POPUP_MENU_SPEED_COEF = 3.0f;

	PopupMenu::PopupMenu(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mWidgetClient(null),
		mHeightLine(1),
		mMaxWidth(0)
	{
		// ��� ����� ����� �����
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
		}
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (List must have Client)");

		// ������ ��������
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("SkinLine");
		if (iter != param.end()) mSkinLine = iter->second;
		MYGUI_ASSERT(false == mSkinLine.empty(), "SkinLine property or skin not found (PopupMenu must have SkinLine property)");

		iter = param.find("HeightLine");
		if (iter != param.end()) mHeightLine = utility::parseInt(iter->second);
		if (mHeightLine < 1) mHeightLine = 1;

		// ������������� �������� ����
		hide();

	}

	void PopupMenu::addItem(const Ogre::UTFString& _item)
	{
		insertItem(ITEM_NONE, _item);
	}

	void PopupMenu::insertItem(size_t _index, const Ogre::UTFString& _item)
	{
		if (_index > m_listWidgets.size()) _index = m_listWidgets.size();
		WidgetPtr widget = mWidgetClient->createWidget<Button>(mSkinLine, IntCoord(0, 0, mWidgetClient->getWidth(), mHeightLine), ALIGN_TOP | ALIGN_HSTRETCH);
		widget->eventMouseButtonClick = newDelegate(this, &PopupMenu::notifyMouseClick);
		widget->setCaption(_item);

		IntSize size = widget->getTextSize(_item);
		size.width += 7;
		widget->_setInternalData(size.width);

		m_listWidgets.insert(m_listWidgets.begin() + _index, widget);

		update();
	}

	void PopupMenu::setItem(size_t _index, const Ogre::UTFString& _item)
	{
		MYGUI_ASSERT(_index < m_listWidgets.size(), "index out of range");
		WidgetPtr widget = m_listWidgets[_index];
		widget->setCaption(_item);

		IntSize size = widget->getTextSize();
		widget->_setInternalData(size.width);

		update();
	}

	void PopupMenu::deleteItem(size_t _index)
	{
		MYGUI_ASSERT(_index < m_listWidgets.size(), "index out of range");
		WidgetManager::getInstance().destroyWidget(m_listWidgets[_index]);
		m_listWidgets.erase(m_listWidgets.begin() + _index);

		update();
	}

	void PopupMenu::deleteAllItems()
	{
		if (false == m_listWidgets.empty()) {
			WidgetManager & manager = WidgetManager::getInstance();
			for (VectorWidgetPtr::iterator iter=m_listWidgets.begin(); iter!=m_listWidgets.end(); ++iter) {
				manager.destroyWidget(*iter);
			}
			m_listWidgets.clear();

			update();
		}
	}

	size_t PopupMenu::getItemCount()
	{
		return m_listWidgets.size();
	}

	const Ogre::UTFString& PopupMenu::getItem(size_t _index)
	{
		MYGUI_ASSERT(_index < m_listWidgets.size(), "index out of range");
		return m_listWidgets[_index]->getCaption();
	}

	void PopupMenu::update()
	{
		IntSize size;
		for (VectorWidgetPtr::iterator iter=m_listWidgets.begin(); iter!=m_listWidgets.end(); ++iter) {
			(*iter)->setPosition(0, size.height);
			size.height += mHeightLine;
			if ((*iter)->_getInternalData() > size.width) size.width = (*iter)->_getInternalData();
		}

		setSize(size + mCoord.size() - mWidgetClient->getSize());
	}

	void PopupMenu::notifyMouseClick(MyGUI::WidgetPtr _sender)
	{
		size_t index = ITEM_NONE;
		for (VectorWidgetPtr::iterator iter=m_listWidgets.begin(); iter!=m_listWidgets.end(); ++iter) {
			if ((*iter) == _sender) {
				index = iter - m_listWidgets.begin();
				break;
			}
		}
		eventPopupMunuAccept(this, index);

		// ���������
		setEnabled(false, true);

		// ������ �������
		static_cast<ButtonPtr>(_sender)->setButtonPressed(true);

		ControllerManager::getInstance().addItem(
			this, new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, ControllerFadeAlpha::ACTION_HIDE, false));
	}

	void PopupMenu::showPopupMenu(const IntPoint& _point)
	{
		setPosition(_point);
		InputManager::getInstance().setKeyFocusWidget(this);

		for (VectorWidgetPtr::iterator iter=m_listWidgets.begin(); iter!=m_listWidgets.end(); ++iter) {
			if (static_cast<ButtonPtr>(*iter)->getButtonPressed()) {
				static_cast<ButtonPtr>(*iter)->setButtonPressed(false);
			}
		}

		ControllerManager::getInstance().addItem(
			this, new ControllerFadeAlpha(ALPHA_MAX,
			POPUP_MENU_SPEED_COEF, ControllerFadeAlpha::ACTION_NONE, true));
	}

	void PopupMenu::_onKeyChangeRootFocus(bool _focus)
	{
		if (false == _focus) {
			// ���������
			setEnabled(false, true);
			// �������� ��������
			ControllerManager::getInstance().addItem(
				this, new ControllerFadeAlpha(ALPHA_MIN, POPUP_MENU_SPEED_COEF, ControllerFadeAlpha::ACTION_HIDE, false));
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyChangeRootFocus(_focus);
	}

} // namespace MyGUI
