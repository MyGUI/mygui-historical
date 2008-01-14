/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Widget.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	const std::string XML_TYPE("List");

	INSTANCE_IMPLEMENT(Gui);

	void Gui::initialise(Ogre::RenderWindow* _window, const std::string& _core)
	{
		// ����� ������ ���
		LogManager::registerSection(MYGUI_LOG_SECTION, MYGUI_LOG_FILENAME);

		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");

		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);
		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH);

		Ogre::Viewport * port = _window->getViewport(0);
		mViewSize.set(port->getActualWidth(), port->getActualHeight());

		registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &Gui::_load);

		// ������������ ������� ������ � ������
		Ogre::OverlayManager &manager = Ogre::OverlayManager::getSingleton();
		mFactoryTextSimpleOverlay = new TextSimpleOverlayElementFactory();
		mFactoryTextEditOverlay = new TextEditOverlayElementFactory();
		mFactoryPanelAlphaOverlay = new PanelAlphaOverlayElementFactory();
		mFactorySharedPanelAlphaOverlay = new SharedPanelAlphaOverlayElementFactory();

		manager.addOverlayElementFactory(mFactoryTextSimpleOverlay);
		manager.addOverlayElementFactory(mFactoryTextEditOverlay);
		manager.addOverlayElementFactory(mFactoryPanelAlphaOverlay);
		manager.addOverlayElementFactory(mFactorySharedPanelAlphaOverlay);

		// ������� � �������������� ���������
		mWidgetManager = new WidgetManager();
		mInputManager = new InputManager();
		mCroppedRectangleManager = new SubWidgetManager();
		mClipboardManager = new ClipboardManager();
		mLayerManager = new LayerManager();
		mSkinManager = new SkinManager();
		mLayoutManager = new LayoutManager();
		mFontManager = new FontManager();
		mPointerManager = new PointerManager();
		mDynLibManager = new DynLibManager();
		mPluginManager = new PluginManager();
		mControllerManager = new ControllerManager();

		mWidgetManager->initialise();
		mInputManager->initialise();
		mCroppedRectangleManager->initialise();
		mClipboardManager->initialise();
		mLayerManager->initialise();
		mSkinManager->initialise();
		mLayoutManager->initialise();
		mFontManager->initialise();
		mPointerManager->initialise();
		mDynLibManager->initialise();
		mPluginManager->initialise();
		mControllerManager->initialise();

		// ��������� ��������� ���������
		load(_core);

		// ���������� ������
		mPointerManager->show();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		unregisterLoadXmlDelegate(XML_TYPE);
		mListFrameListener.clear();
		mMapLoadXmlDelegate.clear();

		_destroyAllChildWidget();

		// ���������������� � ������� ���������
		mPluginManager->shutdown();
		mDynLibManager->shutdown();
		mWidgetManager->shutdown();
		mPointerManager->shutdown();
		mLayerManager->shutdown();
		mFontManager->shutdown();
		mLayoutManager->shutdown();
		mInputManager->shutdown();
		mSkinManager->shutdown();
		mCroppedRectangleManager->shutdown();
		mClipboardManager->shutdown();
		mControllerManager->shutdown();

		delete mPluginManager;
		delete mDynLibManager;
		delete mWidgetManager;
		delete mPointerManager;
		delete mLayerManager;
		delete mFontManager;
		delete mLayoutManager;
		delete mInputManager;
		delete mSkinManager;
		delete mCroppedRectangleManager;
		delete mClipboardManager;
		delete mControllerManager;

		delete mFactoryTextSimpleOverlay;
		delete mFactoryTextEditOverlay;
		delete mFactoryPanelAlphaOverlay;
		delete mFactorySharedPanelAlphaOverlay;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");

		// ����� ��������� ���
		LogManager::shutdown();

		mIsInitialise = false;
	}

	WidgetPtr Gui::createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _coord, _align, 0, _name);
		mWidgetChild.push_back(widget);
		// ������������ ������ � ������
		LayerManager::getInstance().attachItem(widget, _layer, true);
		return widget;
	}

	WidgetPtr Gui::findWidgetT(const std::string& _name)
    {
        return mWidgetManager->findWidgetT(_name);
    }

	void Gui::injectFrameEntered(Ogre::Real timeSinceLastFrame)
	{
		// ������� ���������
		ListFrameListener::iterator iter=mListFrameListener.begin();
		while (iter != mListFrameListener.end()) {
			if (null == (*iter)) iter = mListFrameListener.erase(iter);
			else {
				(*iter)->_frameEntered(timeSinceLastFrame);
				++iter;
			}
		};
	}

	void Gui::addFrameListener(FrameListener * _listener)
	{
		if (null == _listener) return;

		removeFrameListener(_listener);
		mListFrameListener.push_back(_listener);
	}

	void Gui::removeFrameListener(FrameListener * _listener)
	{
		if (null == _listener) return;

		for (ListFrameListener::iterator iter=mListFrameListener.begin(); iter!=mListFrameListener.end(); ++iter) {
			if ((*iter) == _listener) {
				(*iter) = null;
				return;
			}
		}
	}

	LoadXmlDelegate & Gui::registerLoadXmlDelegate(const Ogre::String & _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		MYGUI_ASSERT(iter == mMapLoadXmlDelegate.end(), "name delegate is exist");
		return (mMapLoadXmlDelegate[_key] = LoadXmlDelegate());
	}

	void Gui::unregisterLoadXmlDelegate(const Ogre::String & _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		if (iter != mMapLoadXmlDelegate.end()) mMapLoadXmlDelegate.erase(iter);
	}

	// ������� ������ �������� ������� ��������
	void Gui::_destroyChildWidget(WidgetPtr _widget)
	{
		for (size_t index = 0; index < mWidgetChild.size(); index++) {
			WidgetPtr widget = mWidgetChild[index];
			if (_widget == widget) {
				delete _widget;
				// ������� �� ������
				mWidgetChild[index] = mWidgetChild[mWidgetChild.size()-1];
				mWidgetChild.pop_back();
				return;
			}
		}
		MYGUI_EXCEPT("Widget not find");
	}

	// ������� ���� �����
	void Gui::_destroyAllChildWidget()
	{
		while (false == mWidgetChild.empty()) {
			// �������� ������, ��� ��� �� ������� �������� � �������
			// � �������� � ������� ������������ �������������, �.�. ������
			WidgetPtr widget = mWidgetChild.front();
			WidgetManager::getInstance().destroyWidget(widget);
		}
	}

	void Gui::destroyAllWidget()
	{
		mWidgetManager->destroyAllWidget();
	}

	void Gui::destroyWidget(WidgetPtr _widget)
	{
		mWidgetManager->destroyWidget(_widget);
	}

	bool Gui::load(const std::string & _file, bool _resource)
	{
		return _loadImplement(_file, _resource, false, "", INSTANCE_TYPE_NAME);
	}

	bool Gui::_loadImplement(const std::string & _file, bool _resource, bool _match, const std::string & _type, const std::string & _instance)
	{
		xml::xmlDocument doc;
		std::string file = (_resource ? helper::getResourcePath(_file) : _file).c_str();
		if (file.empty()) {
			MYGUI_LOG(Error, _instance << " : '" << _file << "' not found");
			return false;
		}
		if (false == doc.open(file)) {
			MYGUI_LOG(Error, _instance << " : " << doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (null == root) || (root->getName() != "MyGUI") ) {
			MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'MyGUI' not found");
			return false;
		}

		std::string type;
		if (root->findAttribute("type", type)) {
			MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
			if (iter != mMapLoadXmlDelegate.end()) {
				if ((false == _match) || (type == _type)) (*iter).second(root, file);
				else {
					MYGUI_LOG(Error, _instance << " : '" << _file << "', type '" << _type << "' not find");
					return false;
				}
			}
			else {
				MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
				return false;
			}
		}
		// ������������ ��� ����� ���������
		else if (false == _match) {
			xml::xmlNodeIterator node = root->getNodeIterator();
			while (node.nextNode("MyGUI")) {
				if (node->findAttribute("type", type)) {
					MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
					if (iter != mMapLoadXmlDelegate.end()) {
						(*iter).second(node.currentNode(), file);
					}
					else {
						MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
					}
				}
				else {
					MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'type' not find");
				}
			}
		}

		return true;
	}

	void Gui::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		// ����� ����� � ��������, �������� ����
		xml::xmlNodeIterator node = _node->getNodeIterator();
		while (node.nextNode(XML_TYPE)) {
			std::string source, resource;
			if (false == node->findAttribute("file", source)) continue;
			resource = node->findAttribute("resource");
			bool res = resource.empty() ? true : util::parseBool(resource);
			MYGUI_LOG(Info, "Load ini file '" << source << "' from " << (res ? "resource" : "current") << " path");
			_loadImplement(source, res, false, "", INSTANCE_TYPE_NAME);
		};
	}

} // namespace MyGUI
