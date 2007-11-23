#ifndef __LAYOUT_MANAGER_H__
#define __LAYOUT_MANAGER_H__

#include "Prerequest.h"
#include "Instance.h"
#include <string>
#include "Common.h"
#include "xmlDocument.h"
#include "WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport LayoutManager
	{
		INSTANCE_HEADER(LayoutManager);

	public:
		void load(const std::string & _file);

	private:
		void parseLayoutMyGUI(xml::xmlNodePtr _root);
		void parseLayoutCEGUI(xml::xmlNodePtr _root);

		void parseWidgetMyGUI(xml::xmlNodeIterator & _widget, WidgetPtr _parent);
		
	}; // class LayoutManager

} // namespace MyGUI

#endif // __LAYOUT_MANAGER_H__