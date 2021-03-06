/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_CONTROLLER_MANAGER_H__
#define __MYGUI_CONTROLLER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_FrameListener.h"
#include "MyGUI_ControllerItem.h"
#include "MyGUI_UnlinkWidget.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	typedef std::pair<WidgetPtr, ControllerItem *> PairControllerItem;
	typedef std::list<PairControllerItem> ListControllerItem;

	class _MyGUIExport ControllerManager : public FrameListener, public UnlinkWidget
	{
		INSTANCE_HEADER(ControllerManager);

	public:
		void initialise();
		void shutdown();

		void addItem(WidgetPtr _widget, ControllerItem * _item);
		void removeItem(WidgetPtr _widget);

		void _unlinkWidget(WidgetPtr _widget);

	private:
		void _frameEntered(float _time);
		void clear();

	private:
		ListControllerItem mListItem;

	};

} // namespace MyGUI

#endif // __MYGUI_CONTROLLER_MANAGER_H__
