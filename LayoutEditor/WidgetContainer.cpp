#include "WidgetContainer.h"
#include "BasisManager.h"

const std::string LogSection = "LayoutEditor";

INSTANCE_IMPLEMENT(EditorWidgets);

void EditorWidgets::initialise()
{
	global_counter = 0;
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
	if (file.empty())
	{
		if (false == doc.open(_fileName)) {
			LOGGING(LogSection, Error, _instance << " : '" << _fileName << "' not found");
			return false;
		}
	}
	else if (false == doc.open(file))
	{
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
	std::string _instance = "Editor";

	MyGUI::xml::xmlDocument doc;
	std::string file(MyGUI::helper::getResourcePath(_fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
	if (file.empty()) {
		file = _fileName;
	}

	doc.createInfo();
	MyGUI::xml::xmlNodePtr root = doc.createRoot("MyGUI");
	root->addAttributes("type", "Layout");

	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		// � ������ ������ �����
		if (null == (*iter)->widget->getParent()) serialiseWidget(*iter, root);
	}

	if (false == doc.save(file)) {
		LOGGING(LogSection, Error, _instance << " : " << doc.getLastError());
		return false;
	}

	return true;
}

void EditorWidgets::loadxmlDocument(MyGUI::xml::xmlDocument * doc)
{
	MyGUI::xml::xmlNodePtr root = doc->getRoot();

	std::string type;
	if (root->findAttribute("type", type)) {
		if (type == "Layout")
		{
			// ����� ����� � ��������
			MyGUI::xml::xmlNodeIterator widget = root->getNodeIterator();
			while (widget.nextNode("Widget")) parseWidget(widget, 0);
		}
	}
}

MyGUI::xml::xmlDocument * EditorWidgets::savexmlDocument()
{
	MyGUI::xml::xmlDocument * doc = new MyGUI::xml::xmlDocument();

	doc->createInfo();
	MyGUI::xml::xmlNodePtr root = doc->createRoot("MyGUI");
	root->addAttributes("type", "Layout");

	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		// � ������ ������ �����
		if (null == (*iter)->widget->getParent()) serialiseWidget(*iter, root);
	}

	return doc;
}

void EditorWidgets::add(WidgetContainer * _container)
{
	widgets.push_back(_container);
}

void EditorWidgets::remove(MyGUI::WidgetPtr _widget)
{
	// ���� ������
	MyGUI::VectorWidgetPtr childs = _widget->getChilds();
	for (MyGUI::VectorWidgetPtr::iterator iter = childs.begin(); iter != childs.end(); ++iter)
	{
		if (null != find(*iter)) remove(*iter);
	}
	WidgetContainer * _container = find(_widget);

	MyGUI::WidgetPtr parent = _widget->getParent();
	// FIXME � ���� ��� �� ��� � �����
	if ((null != parent) && (parent->getWidgetType() == "Tab")) MyGUI::castWidget<MyGUI::Tab>(parent)->removeSheet(MyGUI::castWidget<MyGUI::Sheet>(_widget));
	else MyGUI::Gui::getInstance().destroyWidget(_widget);

	if (null != _container)
	{
		widgets.erase(std::find(widgets.begin(), widgets.end(), _container));
		delete _container;
	}
}

void EditorWidgets::clear()
{
	while (!widgets.empty())
	{
		remove(widgets[widgets.size()-1]->widget);
	}
}

WidgetContainer * EditorWidgets::find(MyGUI::WidgetPtr _widget)
{
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
	if (_name.empty()) return null;
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
	if (_widget->findAttribute("position_real", container->position_real)) coord = MyGUI::Gui::getInstance().convertRelativeToInt(MyGUI::FloatCoord::parse(container->position_real), _parent);

	// � ��� �� 2 ����������� ������� �������� � ������, � � ��� ����� ������ ���������������
	if (false == container->name.empty())
	{
		WidgetContainer * iter = find(container->name);
		if (null != iter)
		{
			LOGGING(LogSection, Warning, "widget with same name name '" << container->name << "'. Renamed");
			static long renameN=0;
			container->name = MyGUI::utility::toString(container->name, renameN++);
		}
	}

	std::string tmpname = container->name;
	if (tmpname.empty()) 
	{
		tmpname = MyGUI::utility::toString(container->type, global_counter);
		global_counter++;
	}

	//����� � �� �����
	tmpname = "LayoutEditor_" + tmpname;

	if (null == _parent) {
		container->widget = MyGUI::Gui::getInstance().createWidgetT(container->type, container->skin, coord, align, container->layer, tmpname);
		add(container);
	}
	else
	{
		container->widget = _parent->createWidgetT(container->type, container->skin, coord, align, tmpname);
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
			if (("Message_Modal" != key) && ("Window_AutoAlpha" != key)) MyGUI::WidgetManager::getInstance().parse(container->widget, key, value);
			container->mProperty.push_back(std::make_pair(key, value));
		}
		else if (widget->getName() == "UserString") {
			// ������ ��������
			if (false == widget->findAttribute("key", key)) continue;
			if (false == widget->findAttribute("value", value)) continue;
			container->mUserString.insert(std::make_pair(key, value));
		}

	};
}

void EditorWidgets::serialiseWidget(WidgetContainer * _container, MyGUI::xml::xmlNodePtr _node)
{
	MyGUI::xml::xmlNodePtr node = _node->createChild("Widget");

	node->addAttributes("type", _container->type);
	if ("" != _container->skin) node->addAttributes("skin", _container->skin);
	if ("" != _container->position) node->addAttributes("position", _container->position);
	if ("" != _container->position_real) node->addAttributes("position_real", _container->position_real);
	if ("" != _container->align) node->addAttributes("align", _container->align);
	if ("" != _container->layer) node->addAttributes("layer", _container->layer);
	if ("" != _container->name) node->addAttributes("name", _container->name);

	for (StringPairs::iterator iter = _container->mProperty.begin(); iter != _container->mProperty.end(); ++iter)
	{
		MyGUI::xml::xmlNodePtr nodeProp = node->createChild("Property");
		nodeProp->addAttributes("key", iter->first);
		nodeProp->addAttributes("value", iter->second);
	}

	for (MapString::iterator iter = _container->mUserString.begin(); iter != _container->mUserString.end(); ++iter)
	{
		MyGUI::xml::xmlNodePtr nodeProp = node->createChild("UserString");
		nodeProp->addAttributes("key", iter->first);
		nodeProp->addAttributes("value", iter->second);
	}

	// ����� ���������, �.�. ������������ ���������
	for (std::vector<WidgetContainer*>::iterator iter = widgets.begin(); iter != widgets.end(); ++iter)
	{
		MyGUI::WidgetPtr parent = (*iter)->widget->getParent();
		// ����� - ��� ��?
		if (_container->widget->getWidgetType() == "Window"){
			if (null != parent)
				if (_container->widget == parent->getParent()) serialiseWidget(*iter, node);
		}else if (_container->widget == parent) serialiseWidget(*iter, node);
	}
}
