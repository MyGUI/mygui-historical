#ifndef _POINTERINFO_H_
#define _POINTERINFO_H_

#include "Prerequest.h"
#include <map>
#include <string>
#include "TRect.h"
#include "TSize.h"

namespace MyGUI
{

	struct PointerInfo
	{
		PointerInfo() {}
		PointerInfo(const floatRect &_offset, const intPoint & _point) : offset(_offset), point(_point) {}
		floatRect offset;
		intPoint point;
	};

	typedef std::map<std::string, PointerInfo> MapPointerInfo;

} // namespace MyGUI


#endif
