
#include "Widget.h"
#include "debugOut.h"

namespace widget
{

	Widget::Widget(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfo * _info, Widget * _parent) :
		BasisWidget(_x, _y, _info->width(), _info->height(), _align, _parent), // ������ �� �����
		m_text(0),
		m_visible(true),
		m_alpha(1.0),
		m_color(1.0, 1.0, 1.0, 1.0),
		m_stateInfo(_info->getStateInfo())
	{
		for (BasisInfo::const_iterator iter =_info->getBasisInfo().begin(); iter!=_info->getBasisInfo().end(); iter ++) {
			addSubSkin(*iter, _info->getMaterial());
		}
		if (!_info->getFontName().empty()) setFontName(_info->getFontName(), _info->getFontHeight());
		// � ��� ������ ���������� ������
		setState("normal");
		size(_cx, _cy);
		update();
	}

	Widget::~Widget()
	{
		while (m_widgetChild.size() > 0)  {
			delete m_widgetChild[0];
			m_widgetChild.pop_back();
		};
	}

	Widget * Widget::createChild(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfo * _info)
	{
		Widget * widget = new Widget(_x, _y, _cx, _cy, _align, _info, this);
		m_widgetChild.push_back(widget);

		return widget;
	}

	BasisWidget *  Widget::addSubSkin(const tagBasisWidgetInfo &_info, const String & _material)
	{
		BasisWidget * sub = BasisWidgetCreator::getInstance().createBasisWidget(_info, _material, this);
		// ���� ��� ���� ������, �� ����������
		if (sub->isText()) m_text = sub;
		// ��������� � ����� ������
		m_subSkinChild.push_back(sub);
		return sub;
	}

	void Widget::attach(Ogre::OverlayElement * _element, bool _child)
	{
		if (_child) { // ��� � ��� ����� ����� �����������
			assert(m_subSkinChild.size() > 0);
			m_subSkinChild[0]->attach(_element, true);
		} else if (m_parent) {
			// ���� � ���� ���� ���� ���� �������, �� ��������� ���������
			m_parent->attach(_element, true);
		} else {
			// � ���� ���, �� �������� ����
			OverlayManager &overlayManager = OverlayManager::getSingleton();
			Overlay * overlay;
			overlay = overlayManager.create("WidgetOverlay");
			overlay->setZOrder(200);
			overlay->show();
			overlay->add2D(static_cast<Ogre::OverlayContainer*>(_element));
		}
	}

	void Widget::visible(bool _visible)
	{

		if (m_visible == _visible) return;
		m_visible = _visible;

		// ���� ������ �������������, �� �� ����������
		if (_visible && !m_show) return;

		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(m_visible);
	}

	void Widget::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
		// ���� ����� �� ������� �� �� ����������
		if (_show && !m_visible) return;

		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(m_show);
	}

	void Widget::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		// ��� ������� ��������� � �  �� ���������
		align(_cx, _cy, _update);
	}

	void Widget::align(int _cx, int _cy, bool _update)
	{
		if (!m_parent) return;

		bool need_move = false;
		bool need_size = false;
		int x = m_x;
		int y = m_y;
		int cx = m_cx;
		int cy = m_cy;

		// �������������� ������������ 
		if (m_align & ALIGN_RIGHT) {
			if (m_align & ALIGN_LEFT) {
				// �����������
				cx = m_cx + (m_parent->width() - _cx);
				need_size = true;
			} else {
				// ������� �� ������� ����
				x = m_x + (m_parent->width() - _cx);
				need_move = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			x = (m_parent->width() - m_cx) / 2;
			need_move = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// �����������
				cy = m_cy + (m_parent->height() - _cy);
				need_size = true;
			} else {
				y = m_y + (m_parent->height() - _cy);
				need_move = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			y = (m_parent->height() - m_cy) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) move(x, y, cx, cy);
			else move(x, y);
		} else if (need_size) {
			size(cx, cy);
		} else update(); // ������ ���� �� ������� ������������ � ����

	}


	void Widget::move(int _x, int _y)
	{

		// � ��� ������ ���������� ����� ����������
		m_x = _x;
		m_y = _y;

		update();
	}

	void Widget::move(int _x, int _y, int _cx, int _cy)
	{

		if (!m_parent) return;

		// � ��� ������ ���������� ����� ����������
		m_x = _x;
		m_y = _y;

		// ������ ���������� �������
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		bool show = true;

		// ��������� ������������
		bool margin = check_margin();

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (check_outside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(m_x, m_y, _cx, _cy, m_margin || margin);
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(m_x, m_y, _cx, _cy, m_margin || margin);

		// ���������� ������� ���������
		m_margin = margin;

	}

	void Widget::size(int _cx, int _cy)
	{

		if (!m_parent) return;

		// ������ ���������� �������
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		bool show = true;

		// ��������� ������������
		bool margin = check_margin();

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (check_outside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(_cx, _cy, m_margin || margin);
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(_cx, _cy, m_margin || margin);

		// ���������� ������� ���������
		m_margin = margin;

	}

	void Widget::update()
	{

		if (!m_parent) return;

		bool margin = check_margin();

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (check_outside()) {

				// ��������
				visible(false);
				// ���������� ������� ���������
				m_margin = margin;
				return;

			}

		} else if (!m_margin) { // �� �� �������� � ���� ����������

			// ���������� ������� ���������
			m_margin = margin;

			// ��� ��� ���� ����� ���������� ���� ��� ��������
			for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->correct();

			// ��������
			visible(true);

			return;

		}

		// ���������� ������� ���������
		m_margin = margin;

		// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->update();
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->update();

		// ���� ���� ��� �����, �� �������
		visible(true);

	}

	void Widget::setCaption(const Ogre::DisplayString & _caption)
	{
		if (!m_text) return;
		m_text->setCaption(_caption);
	}

	const Ogre::DisplayString & Widget::getCaption()
	{
		if (!m_text) return BasisWidget::getCaption();
		return m_text->getCaption();
	}

	void Widget::setAlpha(float _alpha)
	{
		m_alpha = _alpha;
		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->setAlpha(m_alpha);
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->setAlpha(m_alpha);
	}

	void Widget::setColour(const Ogre::ColourValue & _color)
	{
		m_color = _color;
		if (!m_text) return;
		m_text->setColour(_color);
	}

	void Widget::setFontName(const Ogre::String & _font)
	{
		if (!m_text) return;
		m_text->setFontName(_font);
	}

	void Widget::setFontName(const Ogre::String & _font, Ogre::ushort _height)
	{
		if (!m_text) return;
		m_text->setFontName(_font, _height);
	}

	const Ogre::String & Widget::getFontName()
	{
		if (!m_text) return BasisWidget::getFontName();
		return m_text->getFontName();
	}

	void Widget::setCharHeight(Ogre::ushort _height)
	{
		if (!m_text) return;
		m_text->setCharHeight(_height);
	}

	Ogre::ushort Widget::getCharHeight()
	{
		if (!m_text) return 0;
		return m_text->getCharHeight();
	}

	void Widget::setState(const Ogre::String & _state)
	{
		StateInfo::const_iterator iter = m_stateInfo.find(_state);
		if (iter == m_stateInfo.end()) return;
		size_t index=0;
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
			(*skin)->setUVSet(iter->second.m_offsets[index++]);
		}
	}

} // namespace widget