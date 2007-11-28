/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LAYER_ITEM_INFO_H__
#define __MYGUI_LAYER_ITEM_INFO_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class LayerInfo;
	class LayerItemInfo;
	class LayerManager;

	typedef LayerInfo * LayerInfoPtr;
	typedef LayerItemInfo * LayerItemInfoPtr;
	typedef std::vector<LayerItemInfoPtr> VectorLayerItemInfo;

	// �������������� ��������� ��� ���������� �������� ��������
	class LayerItemInfo
	{
		// ��� �������
		friend LayerInfo;
		friend LayerManager;

	protected:
		LayerItemInfo() : mOverlayInfo(0), mLayerInfo(0) {}

		// ��� �������� ���� ��� ����� ��������������
		virtual LayerItemInfoPtr findItem(int _left, int _top) {return 0;}

		// ��� ����� ��� ������������� � �������
		virtual void attachToOverlay(Ogre::Overlay * _overlay) = 0;
		virtual void detachToOverlay(Ogre::Overlay * _overlay) = 0;

		// ��� �������� ��������������, ��� ��������� ����
		virtual bool isWidget() {return false;}

	private:
		Ogre::Overlay * mOverlayInfo;
		LayerInfoPtr mLayerInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_ITEM_INFO_H__