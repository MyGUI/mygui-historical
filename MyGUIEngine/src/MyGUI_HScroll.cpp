/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_HScroll.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Button.h"

namespace MyGUI
{

	HScroll::HScroll(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		VScroll(_coord, _align, _info, _parent, _name)
	{
	}

	void HScroll::updateTrack()
	{
		// ������ ��������� � ��������
		int pos = mCoord.width - (int)(mSkinRangeStart + mSkinRangeEnd);

		// �������� ���� �������� ��������� ��� ����� ����
		if ((mScrollRange < 2) || (1 > (int)pos)) {
			mWidgetTrack->hide();
			return;
		}
		// ���� ����� �� �������
		if (false == mWidgetTrack->isShow()) mWidgetTrack->show();

		// � ��������� �������
		pos = (int)(((size_t)pos * mScrollPosition) / (mScrollRange-1) + mSkinRangeStart);
		if (mWidgetTrack->getLeft() != (int)pos) mWidgetTrack->setPosition(IntPoint((int)pos, mWidgetTrack->getTop()));

	}

	void HScroll::TrackMove(int _left, int _top)
	{
		const IntPoint & point = InputManager::getInstance().getLastLeftPressed();

		// ����������� ������� �������
		int start = mPreActionRect.left + (_left - point.left);
		if (start < (int)mSkinRangeStart) start = (int)mSkinRangeStart;
		else if (start > (mCoord.width - (int)mSkinRangeEnd)) start = (mCoord.width - (int)mSkinRangeEnd);
		if (mWidgetTrack->getLeft() != start) mWidgetTrack->setPosition(IntPoint(start, mWidgetTrack->getTop()));

		// ����������� ��������� ��������������� �������
		// ���� ��� �������
		int pos = start - (int)mSkinRangeStart + (mCoord.width - (int)(mSkinRangeStart + mSkinRangeEnd)) / (((int)mScrollRange-1) * 2);
		// ����������� ��������� �������� � ���������
		pos = pos * (int)(mScrollRange-1) / (mCoord.width - (int)(mSkinRangeStart + mSkinRangeEnd));

		// ��������� �� ������ � ���������
		if (pos < 0) pos = 0;
		else if (pos >= (int)mScrollRange) pos = (int)mScrollRange - 1;
		if (pos == (int)mScrollPosition) return;

		mScrollPosition = pos;
		// �������� �������
		eventScrollChangePosition(this, (int)mScrollPosition);
	}

} // namespace MyGUI
