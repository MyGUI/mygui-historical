/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_USER_DATA_H__
#define __MYGUI_WIDGET_USER_DATA_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	class _MyGUIExport UserData
	{
	public:
		UserData() : mInternalData(0) {}
		virtual ~UserData() {}

		// ���������������� ������ ������� ������
		inline void setUserString(const std::string & _key, const std::string & _value)
		{
			mMapUserString[_key] = _value;
		}

		const std::string & getUserString(const std::string & _key)
		{
			MapString::iterator iter = mMapUserString.find(_key);
			if (iter == mMapUserString.end()) {
				static std::string empty;
				return empty;
			}
			return iter->second;
		}

		inline void clearUserString(const std::string & _key)
		{
			MapString::iterator iter = mMapUserString.find(_key);
			if (iter != mMapUserString.end()) mMapUserString.erase(iter);
		}

		inline void clearUserString()
		{
			mMapUserString.clear();
		}

		inline int getInternalData() {return mInternalData;}
		inline void setInternalData(int _data) {mInternalData = _data;}

		inline const std::string& getInternalString() {return mInternalString;}
		inline void setInternalString(const std::string& _data) {mInternalString = _data;}

	private:
		// ���������������� ������
		MapString mMapUserString;
		// ����� ��� ������������ �������������
		int mInternalData;
		std::string mInternalString;
	};
} // namespace MyGUI

#endif // __MYGUI_WIDGET_USER_DATA_H__
