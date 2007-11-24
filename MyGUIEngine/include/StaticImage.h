#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "Prerequest.h"
#include "StaticImageFactory.h"
#include "Widget.h"
#include "PanelAlphaOverlayElement.h"

namespace MyGUI
{

	class StaticImage;
	typedef StaticImage * StaticImagePtr;

	class _MyGUIExport StaticImage : public Widget
	{
		// ��� ������ ��������� ������������
		friend factory::StaticImageFactory;

	protected:
		StaticImage(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		void setImageInfo(const std::string & _material, const FloatSize & _tile);
		void setImageInfo(const std::string & _material, const FloatRect & _rect, const FloatSize & _tile);
		void setImageMaterial(const std::string & _material);

		inline void setImageRect(const FloatRect & _rect)
		{
			mRectImage = _rect;
			mNum = (size_t)-1;
		}

		inline void setImageTile(const FloatSize & _tile)
		{
			mSizeTile = _tile;
			mNum = (size_t)-1;
		}

		void setImageNum(size_t _num);
		inline size_t getImageNum(size_t _num) {return mNum;}

	private:
		// ����� � �������� ����� ��������
		FloatRect mRectImage;
		// ������ ����� ��������
		FloatSize mSizeTile;
		// ������ ��������
		FloatSize mSizeTexture;
		// ������� ��������
		size_t mNum;
		// ������������ ������� ������ �������
		PanelAlphaOverlayElement * mElement;

	}; // class StaticImage : public Widget

} // namespace MyGUI

#endif // __STATIC_IMAGE_H__