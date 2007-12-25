/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_CAST_WIDGET_H__
#define __MYGUI_CAST_WIDGET_H__

namespace MyGUI
{
	// ��������� ����� ��� �������� ���� �������
	class Widget;
	template <class T> inline T* castWidget(Widget * _widget)
	{
	#if MYGUI_DEBUG_MODE == 1
		if (null == dynamic_cast<T*>(_widget)) {
			std::string error(util::toString("ASSERT : 'null == dynamic_cast<T*>(_widget)'  dest type = '",
				T::getType(), ((_widget != null) ? "' source name = '" + _widget->getName() : ""), "'"));
			MyGUI::MYGUI_LOG(error, " at  ", __FILE__, " (line ", __LINE__, ")");
			OGRE_EXCEPT(0, error, "");
		}
	#endif // MYGUI_DEBUG_MODE == 1
		return static_cast<T*>(_widget);
	}

} // namespace MyGUI

#endif // __MYGUI_CAST_WIDGET_H__