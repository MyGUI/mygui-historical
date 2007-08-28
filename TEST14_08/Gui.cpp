
#include "Gui.h"
#include "LayoutManager.h"

namespace widget
{

	Gui::Gui()
		: BasisWidget(-100000, -100000, 100000, 100000, 0, 0),
		m_widgetManagerInstance(WidgetManager::getInstance()),
		m_skinManagerInstance(SkinManager::getInstance()),
		m_layerManagerInstance(LayerManager::getInstance()),
		m_height(1), m_width(1)
	{
		// ��������� ������ � �������� �������
		m_layerManagerInstance.load("main.layer");
	}

	void Gui::initialise(Ogre::RenderWindow* _window)
	{
		Ogre::Viewport * port = _window->getViewport(0);
		m_height = port->getActualHeight();
		m_width = port->getActualWidth();
	}

	WidgetPtr Gui::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, char _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = m_widgetManagerInstance.createWidget(_type, _skin, _x, _y, _cx, _cy, _align, this, _name);
		m_widgetChild.push_back(widget);
		// ������������ ������ � ������
		m_layerManagerInstance.attachWidget(widget, _layer);
		return widget;
	}

	void Gui::destroyWidget(WidgetPtr & _widget)
	{
		// ����������� ������ �� ������
		m_layerManagerInstance.detachWidget(_widget);
		// � �������
		m_widgetManagerInstance.destroyWidget(_widget);
	}

	void Gui::attach(BasisWidgetPtr _basis, bool _child)
	{
		// ������ ������������ ������� � �������
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		Overlay * overlay;
		static long num=0;
		overlay = overlayManager.create(Ogre::StringConverter::toString(num++) + "_WidgetOverlay");
		overlay->setZOrder(0);
		overlay->show();
		overlay->add2D(static_cast<Ogre::OverlayContainer*>(_basis->getOverlayElement()));
		overlay->hide();

		// ��������� ����� =(
		LayerItemInfoPtr item = dynamic_cast<LayerItemInfoPtr>(_basis->getParent());
		assert(item);
		item->m_overlayInfo = overlay;

	}

	void Gui::loadLayout(const std::string & _file)
	{
		// ����� � ������
		LayoutManager::getInstance().loadLayout(_file);
	}


} // namespace widget