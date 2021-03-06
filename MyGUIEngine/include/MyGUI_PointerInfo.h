/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_POINTER_INFO_H__
#define __MYGUI_POINTER_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	struct PointerInfo
	{
		PointerInfo()
		{
		}

		PointerInfo(const FloatRect &_offset, const IntPoint & _point, const IntSize& _size, const std::string& _material) :
			offset(_offset),
			point(_point),
			size(_size),
			material(_material)
		{
		}

		FloatRect offset;
		IntPoint point;
		IntSize size;
		std::string material;
	};

	typedef std::map<std::string, PointerInfo> MapPointerInfo;

} // namespace MyGUI

#endif // __MYGUI_POINTER_INFO_H__
