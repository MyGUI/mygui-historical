/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	const std::string XML_TYPE("Layout");

	INSTANCE_IMPLEMENT(LayoutManager);

	void LayoutManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LayoutManager::_load);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LayoutManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	VectorWidgetPtr LayoutManager::load(const std::string & _file, bool _resource)
	{
		mVectorWidgetPtr.clear();
		Gui::getInstance()._loadImplement(_file, _resource, true, XML_TYPE, INSTANCE_TYPE_NAME);
		return mVectorWidgetPtr;
	}

	void LayoutManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		parseLayout(mVectorWidgetPtr, _node);
	}

	void LayoutManager::parseLayout(VectorWidgetPtr & _widgets, xml::xmlNodePtr _root)
	{
		// ����� ����� � ��������
		xml::xmlNodeIterator widget = _root->getNodeIterator();
		while (widget.nextNode("Widget")) parseWidget(_widgets, widget, 0);
	}

	void LayoutManager::parseWidget(VectorWidgetPtr & _widgets, xml::xmlNodeIterator & _widget, WidgetPtr _parent)
	{
		// ������ �������� �������
		Ogre::String widgetType, widgetSkin, widgetName, widgetLayer, tmp;
		FloatCoord coord;
		Align align = ALIGN_DEFAULT;

		_widget->findAttribute("type", widgetType);
		_widget->findAttribute("skin", widgetSkin);
		_widget->findAttribute("name", widgetName);
		_widget->findAttribute("layer", widgetLayer);
		if (_widget->findAttribute("align", tmp)) align = SkinManager::getInstance().parseAlign(tmp);
		if (_widget->findAttribute("position", tmp)) coord = FloatCoord::parse(tmp);
		if (_widget->findAttribute("position_real", tmp)) coord = convertFromReal(FloatCoord::parse(tmp), _parent);

		WidgetPtr wid;
		if (null == _parent) {
			wid = Gui::getInstance().createWidgetT(widgetType, widgetSkin, (int)coord.left, (int)coord.top, (int)coord.width, (int)coord.height, align, widgetLayer, widgetName);
			_widgets.push_back(wid);
		}
		else wid = _parent->createWidgetT(widgetType, widgetSkin, (int)coord.left, (int)coord.top, (int)coord.width, (int)coord.height, align, widgetName);

		// ����� ����� � ��������
		xml::xmlNodeIterator widget = _widget->getNodeIterator();
		while (widget.nextNode()) {

			std::string key, value;

			if (widget->getName() == "Widget") parseWidget(_widgets, widget, wid);
			else if (widget->getName() == "Property") {

				// ������ ��������
				if (false == widget->findAttribute("key", key)) continue;
				if (false == widget->findAttribute("value", value)) continue;
				// � ������ ��������
				WidgetManager::getInstance().parse(wid, key, value);
			}
			else if (widget->getName() == "UserString") {
				// ������ ��������
				if (false == widget->findAttribute("key", key)) continue;
				if (false == widget->findAttribute("value", value)) continue;
				wid->setUserString(key, value);
			}

		};
	}

	FloatCoord LayoutManager::convertFromReal(const FloatCoord& _coord, WidgetPtr _parent)
	{
		const FloatSize& size = Gui::getInstance().getViewSize();
		if (null == _parent) {
			return FloatCoord(_coord.left * size.width, _coord.top * size.height, _coord.width * size.width, _coord.height * size.height);
		}
		const IntCoord& coord = _parent->getClientRect();
		return FloatCoord(_coord.left * coord.width, _coord.top * coord.height, _coord.width * coord.width, _coord.height * coord.height);
	}

} // namespace MyGUI
