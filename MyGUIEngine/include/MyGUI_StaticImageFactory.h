/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_STATIC_IMAGE_FACTORY_H__
#define __MYGUI_STATIC_IMAGE_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport StaticImageFactory : public WidgetFactoryInterface
		{
		public:
			StaticImageFactory::StaticImageFactory();
			StaticImageFactory::~StaticImageFactory();

			// ���������� ���������� �������
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name);

			// ������ ��� ��������
			void ImageMaterial(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void ImageRect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void ImageTile(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void ImageNum(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_STATIC_IMAGE_FACTORY_H__