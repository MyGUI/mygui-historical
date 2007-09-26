
#include "VScroll.h"

namespace MyGUI
{
	// ������� ������� ��� ����� �������
	namespace factory { WidgetFactory<VScroll> VScrollFactoryInstance("VScroll"); }

	VScroll::VScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		ScrollBase(_x, _y, _cx, _cy, _align, _info, _parent, _name)
	{
	}

	void VScroll::updateTrack()
	{
		// ������ ��������� � ��������
		size_t pos = m_cy - (mSkinRangeStart + mSkinRangeEnd);

		// �������� ���� �������� ��������� ��� ����� ����
		if ((mScrollRange < 2) || (mWidgetTrack->height() > (int)pos)) {
			mWidgetTrack->show(false);
			return;
		}
		// ���� ����� �� �������
		if (!mWidgetTrack->isShow()) mWidgetTrack->show(true);

		// � ��������� �������
		pos = (pos * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart;
		if (mWidgetTrack->top() != (int)pos) mWidgetTrack->move(mWidgetTrack->left(), (int)pos);
	}

	void VScroll::notifyTrackMove(int _x, int _y)
	{
		const intPoint & point = InputManager::getInstance().getLastLeftPressed();
		int pos = m_preActionRect.top + (_y - point.top) - (int)mSkinRangeStart;

		// ���� ��� �������
		pos += (m_cy - (int)(mSkinRangeStart + mSkinRangeEnd)) / (((int)mScrollRange-1) * 2);
		// ����������� ��������� �������� � ���������
		pos = pos * (int)(mScrollRange-1) / (m_cy - (int)(mSkinRangeStart + mSkinRangeEnd));

		// ��������� �� ������ � ���������
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		updateTrack();
	}

} // namespace MyGUI
