
#include "SubSkin.h"

namespace widget
{

	SubSkin::SubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, Widget * _parent, PanelOverlayElement * _overlayContainer) : 
		m_parent (_parent),
		m_x (_x),
		m_y (_y),
		m_cx (_cx),
		m_cy (_cy),
/*		m_left_margin (0),
		m_right_margin (0),
		m_top_margin (0),
		m_bottom_margin (0),*/
		m_baseLeftUV (_leftUV),
		m_baseTopUV (_topUV),
		m_baseRightUV (_rightUV),
		m_baseBottomUV (_bottomUV)
	{

		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement(
			"Panel", "Widget_" + Ogre::StringConverter::toString((uint32)this)) );

		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_x, m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setMaterialName(_material);

		if (m_parent) m_overlayContainer->setPosition(m_parent->m_x + m_x, m_parent->m_y + m_y);

		if (_overlayContainer) {
			// ���� � ���� ���� ���� ���� �������, �� ��������� ���������
			_overlayContainer->addChild(m_overlayContainer);
		} else {
			// � ���� ���, �� �������� ����
			Overlay * overlay;
			overlay = overlayManager.create("WidgetOverlay");
			overlay->setZOrder(200);
			overlay->show();
			overlay->add2D(m_overlayContainer);
		}

//		update();
	}

	SubSkin::~SubSkin()
	{
	}

/*	void SubSkin::size(int _cx, int _cy)
	{
		m_cx = _cx;
		m_cy = _cy;
		update();
	}*/

	void SubSkin::move(int _x, int _y)
	{
		// ����������� ������� ������� ���� ���� �������� ������ ����
		m_overlayContainer->setPosition(m_x + _x, m_y + _y);
	}

/*	void SubSkin::check()
	{
		//�������, ��� �������� �������
		m_left_margin   = (left()   < m_parent->m_left_margin) ?
									m_parent->m_left_margin - left() : 0; //������� �� ������
		m_right_margin  = (right()  > m_parent->m_cx - m_parent->m_right_margin) ?
				    right() - (m_parent->m_cx - m_parent->m_right_margin) : 0; //������� �� �������
		m_top_margin    = (top()    < m_parent->m_top_margin) ?
									  m_parent->m_top_margin - top() : 0; //������� �� �����
		m_bottom_margin = (bottom() > m_parent->m_cy - m_parent->m_bottom_margin) ?
				  bottom() - (m_parent->m_cy - m_parent->m_bottom_margin) : 0; //������� �� ����

		// hide �� �����, ��� ��� ��� ����� ����� ������ �������, ��������� setTransparent

		if (right()  < m_parent->m_left_margin )                    { m_overlayContainer->setTransparent(true); return;} // ������ ������ ������
		if (left()   > m_parent->m_cx - m_parent->m_right_margin )  { m_overlayContainer->setTransparent(true); return;} // ������ ������ �������
		if (bottom() < m_parent->m_top_margin  )                    { m_overlayContainer->setTransparent(true); return;} // ������ ������ �����
		if (top()    > m_parent->m_cy - m_parent->m_bottom_margin ) { m_overlayContainer->setTransparent(true); return;} // ������ ������ ����

		m_overlayContainer->setTransparent(false); // ��� ���-�� �����

		//�������� �������
		m_overlayContainer->setPosition(view_left() - m_parent->m_left_margin, view_top() - m_parent->m_top_margin);
		m_overlayContainer->setWidth(view_width());
		m_overlayContainer->setHeight(view_height());

		if(!(m_left_margin || m_right_margin || m_top_margin || m_bottom_margin)){
			//���� ������ �� �������
			m_overlayContainer->setUV(m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV);
			return;
		}
		// ������ ������� ��������
		Real UV_lft = m_left_margin;
		Real UV_top = m_top_margin;
		Real UV_rgt = m_cx - m_right_margin;
		Real UV_btm = m_cy - m_bottom_margin;

		UV_lft = UV_lft / (float)m_cx;
		UV_top = UV_top / (float)m_cy;
		UV_rgt = UV_rgt / (float)m_cx;
		UV_btm = UV_btm / (float)m_cy;

		Real UV_sizeX = m_baseRightUV - m_baseLeftUV;
		Real UV_sizeY = m_baseBottomUV - m_baseTopUV;

		Real UV_lft_total = m_baseLeftUV + UV_lft * UV_sizeX;
		Real UV_top_total = m_baseTopUV + UV_top * UV_sizeY;
		Real UV_rgt_total = m_baseRightUV - (1-UV_rgt) * UV_sizeX;
		Real UV_btm_total = m_baseBottomUV - (1-UV_btm) * UV_sizeY;

		m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);
	}*/

	void SubSkin::restore()
	{
		m_overlayContainer->setPosition(m_x + m_parent->m_x, m_y + m_parent->m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setUV(m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV);
	}

	void SubSkin::updateMain()
	{
		//�������, ��� �������� �������
/*		m_left_margin   = (left()   < m_parent->m_left_margin) ?
									m_parent->m_left_margin - left() : 0; //������� �� ������
		m_right_margin  = (right()  > m_parent->m_cx - m_parent->m_right_margin) ?
				    right() - (m_parent->m_cx - m_parent->m_right_margin) : 0; //������� �� �������
		m_top_margin    = (top()    < m_parent->m_top_margin) ?
									  m_parent->m_top_margin - top() : 0; //������� �� �����
		m_bottom_margin = (bottom() > m_parent->m_cy - m_parent->m_bottom_margin) ?
				  bottom() - (m_parent->m_cy - m_parent->m_bottom_margin) : 0; //������� �� ����
*/
		// hide �� �����, ��� ��� ��� ����� ����� ������ �������, ��������� setTransparent

//		if (m_parent->right()  < m_parent->m_parent->m_left_margin )                    { m_overlayContainer->hide(); return;} // ������ ������ ������
//		if (m_parent->left()   > m_parent->m_parent->m_cx - m_parent->m_parent->m_right_margin )  { m_overlayContainer->hide(); return;} // ������ ������ �������
//		if (m_parent->bottom() < m_parent->m_parent->m_top_margin  )                    { m_overlayContainer->hide(); return;} // ������ ������ �����
//		if (m_parent->top()    > m_parent->m_parent->m_cy - m_parent->m_parent->m_bottom_margin ) { m_overlayContainer->hide(); return;} // ������ ������ ����

//		m_overlayContainer->show(); // ��� ���-�� �����

		//�������� �������
		m_overlayContainer->setPosition(m_parent->view_left() - m_parent->m_parent->m_left_margin, m_parent->view_top() - m_parent->m_parent->m_top_margin);
		m_overlayContainer->setDimensions(m_parent->view_width(), m_parent->view_height());

//		if (!m_parent->m_margin) {
//		if(!(m_left_margin || m_right_margin || m_top_margin || m_bottom_margin)){
			//���� ������ �� �������
//			m_overlayContainer->setUV(m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV);
//			return;
//		}

		// ������ ������� ��������
/*		Real UV_lft = m_left_margin;
		Real UV_top = m_top_margin;
		Real UV_rgt = m_cx - m_right_margin;
		Real UV_btm = m_cy - m_bottom_margin;

		UV_lft = UV_lft / (float)m_cx;
		UV_top = UV_top / (float)m_cy;
		UV_rgt = UV_rgt / (float)m_cx;
		UV_btm = UV_btm / (float)m_cy;

		Real UV_sizeX = m_baseRightUV - m_baseLeftUV;
		Real UV_sizeY = m_baseBottomUV - m_baseTopUV;

		Real UV_lft_total = m_baseLeftUV + UV_lft * UV_sizeX;
		Real UV_top_total = m_baseTopUV + UV_top * UV_sizeY;
		Real UV_rgt_total = m_baseRightUV - (1-UV_rgt) * UV_sizeX;
		Real UV_btm_total = m_baseBottomUV - (1-UV_btm) * UV_sizeY;

		m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);*/

	}

	void SubSkin::updateSub()
	{
	}


} // namespace SubSkin