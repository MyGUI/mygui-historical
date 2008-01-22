/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Widget.h"

#include "MyGUI_InputManager.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_DynLibManager.h"
#include "MyGUI_ControllerManager.h"

namespace MyGUI
{

	const std::string XML_TYPE("List");

	INSTANCE_IMPLEMENT(Gui);

	void Gui::initialise(Ogre::RenderWindow* _window, const std::string& _core, const Ogre::String & _group)
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

		MYGUI_LOG(Info, "Viewport : " << mViewSize.print());

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
		load(_core, _group);

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

	bool Gui::injectMouseMove( const OIS::MouseEvent & _arg) {return mInputManager->injectMouseMove(_arg);}
	bool Gui::injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id ) {return mInputManager->injectMousePress(_arg, _id);}
	bool Gui::injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id ) {return mInputManager->injectMouseRelease(_arg, _id);}

	bool Gui::injectKeyPress(const OIS::KeyEvent & _arg) {return mInputManager->injectKeyPress(_arg);}
	bool Gui::injectKeyRelease(const OIS::KeyEvent & _arg) {return mInputManager->injectKeyRelease(_arg);}

	WidgetPtr Gui::createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _coord, _align, null, _name);
		mWidgetChild.push_back(widget);
		// ������������ ������ � ������
		if (false == _layer.empty()) LayerManager::getInstance().attachItem(widget, _layer, true);
		return widget;
	}

	WidgetPtr Gui::findWidgetT(const std::string& _name)
	{
		return mWidgetManager->findWidgetT(_name);
	}

	void Gui::showPointer() {mPointerManager->show();}
	void Gui::hidePointer() {mPointerManager->hide();}
	bool Gui::isShowPointer() {return mPointerManager->isShow();}

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
		MYGUI_EXCEPT("Widget not found");
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

	bool Gui::load(const std::string & _file, const std::string & _group)
	{
		return _loadImplement(_file, _group, false, "", INSTANCE_TYPE_NAME);
	}

	bool Gui::_loadImplement(const std::string & _file, const std::string & _group, bool _match, const std::string & _type, const std::string & _instance)
	{
		xml::xmlDocument doc;
		std::string file(_group.empty() ? _file : helper::getResourcePath(_file, _group));
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
					MYGUI_LOG(Error, _instance << " : '" << _file << "', type '" << _type << "' not found");
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
					MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'type' not found");
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
			std::string source;
			if (false == node->findAttribute("file", source)) continue;
			std::string group = node->findAttribute("group");
			MYGUI_LOG(Info, "Load ini file '" << source << "' from " << (group.empty() ? "current path" : "resource group : ") << group);
			_loadImplement(source, group, false, "", INSTANCE_TYPE_NAME);
		};
	}

	VectorWidgetPtr Gui::loadLayout(const std::string & _file, const std::string & _group)
	{
		return mLayoutManager->load(_file, _group);
	}


} // namespace MyGUI
