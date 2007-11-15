
#include "Gui.h"
#include "LayoutManager.h"
#include "SkinManager.h"
#include "ParserManager.h"
#include <vector>


namespace MyGUI
{

	INSTANCE_IMPLEMENT(LayoutManager);

	LayoutManager::LayoutManager() {}
	LayoutManager::~LayoutManager() {}

	void LayoutManager::load(const std::string & _file)
	{
		xml::xmlDocument doc;
		if (!doc.open(helper::getResourcePath(_file))) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if (xml_root == 0) return;
		if (xml_root->getName() == "MyGUI_Layout") parseLayoutMyGUI(xml_root);
		else if (xml_root->getName() == "GUILayout") parseLayoutCEGUI(xml_root);
	}

	void LayoutManager::parseLayoutCEGUI(xml::xmlNodePtr _root)
	{
	}

	void LayoutManager::parseLayoutMyGUI(xml::xmlNodePtr _root)
	{
		// ����� ����� � ��������
		xml::xmlNodeIterator widget = _root->getNodeIterator();
		while (widget.nextNode("Widget")) parseWidgetMyGUI(widget, 0);
	}

	void LayoutManager::parseWidgetMyGUI(xml::xmlNodeIterator & _widget, WidgetPtr _parent)
	{
		// ������ �������� �������
		//const xml::VectorAttributes & attrib = _widgetInfo->getAttributes();
		Ogre::String widgetType, widgetSkin, widgetName, widgetLayer, tmp;
		FloatRect coord;
		char align;

		_widget->findAttribute("Type", widgetType);
		_widget->findAttribute("Skin", widgetSkin);
		_widget->findAttribute("Name", widgetName);
		_widget->findAttribute("Layer", widgetLayer);
		if (_widget->findAttribute("Align", tmp)) align = SkinManager::getInstance().parseAlign(tmp);
		if (_widget->findAttribute("Position", tmp)) coord = util::parseFloatRect(tmp);
		if (_widget->findAttribute("PositionReal", tmp)) coord = Gui::getInstance().convertToReal(util::parseFloatRect(tmp));

		WidgetPtr wid;
		if (!_parent) wid = Gui::getInstance().createWidget(widgetType, widgetSkin, coord.left, coord.top, coord.right, coord.bottom, align, widgetLayer, widgetName);
		else wid = _parent->createWidget(widgetType, widgetSkin, coord.left, coord.top, coord.right, coord.bottom, align, widgetName);

		// ����� ����� � ��������
		xml::xmlNodeIterator widget = _widget->getNodeIterator();
		while (widget.nextNode()) {
			if (widget->getName() == "Widget") parseWidgetMyGUI(widget, wid);
			else if (widget->getName() == "Property") {

				// ������ ��������
				std::string propertyKey, propertyValue;
				if (false == widget->findAttribute("Key", propertyKey)) continue;
				if (false == widget->findAttribute("Value", propertyValue)) continue;
				// � ������ ��������
				ParserManager::getInstance().parce(wid, propertyKey, propertyValue);
			}

		};
	}

} // namespace MyGUI