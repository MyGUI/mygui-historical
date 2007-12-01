/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LAYOUT_MANAGER_H__
#define __MYGUI_LAYOUT_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Common.h"
#include "xmlDocument.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport LayoutManager
	{
		INSTANCE_HEADER(LayoutManager);

	public:
		void initialise();
		void shutdown();

		bool load(const std::string & _file, bool _resource = true);

	private:
		void parseLayoutMyGUI(xml::xmlNodePtr _root);
		void parseLayoutCEGUI(xml::xmlNodePtr _root);

		void parseWidgetMyGUI(xml::xmlNodeIterator & _widget, WidgetPtr _parent);

		FloatRect convertToReal(const FloatRect & _rect, WidgetPtr _parent);
		
	}; // class LayoutManager

} // namespace MyGUI

#endif // __MYGUI_LAYOUT_MANAGER_H__