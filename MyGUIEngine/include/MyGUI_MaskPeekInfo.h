/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_MASK_PEEK_INFO_H__
#define __MYGUI_MASK_PEEK_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	class _MyGUIExport MaskPeekInfo
	{
	public:
		MaskPeekInfo() :
		  width(0), height(0)
		{
			/*data.push_back(0);
			data.push_back(1);
			data.push_back(1);
			data.push_back(0);
			width = 2;
			height = 2;*/
		}

		MaskPeekInfo(const std::string& _file)
		{
		}

		bool peek(const IntPoint& _point, const IntCoord& _coord) const
		{
			if ((0 == _coord.width) || (0 == _coord.height)) return false;

			int x = ((_point.left * width)-1) / _coord.width;
			int y = ((_point.top * height)-1) / _coord.height;

			return (0 != data[y * width + x]);
		}

		bool empty() const
		{
			return data.empty();
		}

	private:
		std::vector<unsigned char> data;
		int width, height;
	};

} // namespace MyGUI

#endif // __MYGUI_MASK_PEEK_INFO_H__