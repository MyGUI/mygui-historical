#include "WidgetContainer.h"
#include "MyGUI.h"
#include "BasisManager.h"

const std::string LogSection = "LayoutEditor";

INSTANCE_IMPLEMENT(EditorWidgets);

void EditorWidgets::initialise()
{
	MyGUI::LogManager::registerSection(LogSection, MYGUI_LOG_FILENAME);
}

void EditorWidgets::shutdown()
{
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter) delete *iter;
	widgets.clear();
}

bool EditorWidgets::load(std::string _fileName)
{
	std::string _instance = "Editor";

	MyGUI::xml::xmlDocument doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		LOGGING(LogSection, Error, _instance << " : '" << _fileName << "' not found");
		return false;
	}
	if (false == doc.open(file)) {
		LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return false;
	}

	MyGUI::xml::xmlNodePtr root = doc.getRoot();
	if ( (null == root) || (root->getName() != "MyGUI") ) {
		LOGGING(LogSection, Error, _instance << " : '" << _fileName << "', tag 'MyGUI' not found");
		return false;
	}

	std::string type;
	if (root->findAttribute("type", type)) {
		if (type == "Layout")
		{
			// ����� ����� � ��������
			MyGUI::xml::xmlNodeIterator widget = root->getNodeIterator();
			while (widget.nextNode("Widget")) parseWidget(widget, 0);
		}
		
	}
	return true;
}

bool EditorWidgets::save(std::string _fileName)
{
	return true;
}

void EditorWidgets::add(std::string _name, MyGUI::WidgetPtr _widget)
{
	widgets.push_back(new WidgetContainer(_name, _widget));
}

void EditorWidgets::add(WidgetContainer * _container)
{
	widgets.push_back(_container);
}

WidgetContainer * EditorWidgets::find(MyGUI::WidgetPtr _widget)
{
	// ������ ��������������� ������ � ����������/��������
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		if ((*iter)->widget == _widget)
		{
			return *iter;
		}
	}
	return null;
}
WidgetContainer * EditorWidgets::find(std::string _name)
{
	// ������ ��������������� ������ � ����������/��������
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		if ((*iter)->name == _name)
		{
			return *iter;
		}
	}
	return null;
}

void EditorWidgets::parseWidget(MyGUI::xml::xmlNodeIterator & _widget, MyGUI::WidgetPtr _parent)
{
	WidgetContainer * container = new WidgetContainer();
	// ������ �������� �������
	MyGUI::IntCoord coord;
	MyGUI::Align align = MyGUI::ALIGN_DEFAULT;

	_widget->findAttribute("name", container->name);
	_widget->findAttribute("type", container->type);
	_widget->findAttribute("skin", container->skin);
	_widget->findAttribute("layer", container->layer);
	if (_widget->findAttribute("align", container->align)) align = MyGUI::SkinManager::getInstance().parseAlign(container->align);
	if (_widget->findAttribute("position", container->position)) coord = MyGUI::IntCoord::parse(container->position);
	if (_widget->findAttribute("position_real", container->position_real)) coord = MyGUI::LayoutManager::getInstance().convertRelativeToInt(MyGUI::FloatCoord::parse(container->position_real), _parent);

	// � ��� �� 2 ����������� ������� �������� � ������, � � ��� ����� ������ ���������������
	if (false == container->name.empty()) {
		WidgetContainer * iter = find(container->name);
		if (null != iter)
		{
			LOGGING(LogSection, Warning, "widget with same name name '" << container->name << "'. Renamed");
			static long renameN=0;
			container->name = MyGUI::utility::toString(container->name, renameN++);
		}
	} else {
		static long num=0;
		container->name = MyGUI::utility::toString(container->type, num++);
	}

	if (null == _parent) {
		// FIXME ���� ���������� ��������� ������ ������ ���� ��������, �.�. ����� �������� ���������
		container->widget = MyGUI::Gui::getInstance().createWidgetT("Button", "Button", coord, align, container->layer, container->name);
		//container->widget = MyGUI::Gui::getInstance().createWidgetT(container->type, container->skin, coord, align, container->layer, container->name);
		add(container);
	}
	else
	{
		// FIXME ���� ���������� ��������� ������ ������ ���� ��������, �.�. ����� �������� ���������
		container->widget = _parent->createWidgetT("Button", "Button", coord, align, container->name);
		//container->widget = _parent->createWidgetT(container->type, container->skin, coord, align, container->name);
		add(container);
	}

	// ����� ����� � ��������
	MyGUI::xml::xmlNodeIterator widget = _widget->getNodeIterator();
	while (widget.nextNode()) {

		std::string key, value;

		if (widget->getName() == "Widget") parseWidget(widget, container->widget);
		else if (widget->getName() == "Property") {

			// ������ ��������
			if (false == widget->findAttribute("key", key)) continue;
			if (false == widget->findAttribute("value", value)) continue;
			// � ������ ��������
			// FIXME ���� ���������� ��������� ������ ������ ���� ��������, �.�. ����� �������� ���������
			//MyGUI::WidgetManager::getInstance().parse(container->widget, key, value);
			// wid_rectangle store all fields from layout
			container->setUserString(key, value);
		}
		else if (widget->getName() == "UserString") {
			// ������ ��������
			if (false == widget->findAttribute("key", key)) continue;
			if (false == widget->findAttribute("value", value)) continue;
			container->widget->setUserString(key, value);
		}

	};
}
