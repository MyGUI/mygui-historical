
#include "PointerManager.h"
#include "SkinManager.h"
#include "xmlDocument.h"

namespace widget
{

	PointerManager::PointerManager()
	{
		// ������� ���� �������
		setOverlay(LayerManager::getInstance().createOverlay());

		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
		m_overlayElement = static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement(
			"Panel", Ogre::StringConverter::toString((int)this) + "_PointerManager" ));
		m_overlayElement->setMetricsMode(Ogre::GMM_PIXELS);

		layerItem_attachElement(m_overlayElement);
	}

	void PointerManager::load(const std::string & _file)
	{
		clear();

		xml::xmlDocument doc;
		if (!doc.load(_file)) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if (xml_root == 0) return;
		if (xml_root->getName() != "MyGUI_PointerInfo") return;

		// ����� ����� � ��������, �������� ����
		xml::VectorNode & pointers = xml_root->getChilds();
		for (size_t i=0; i<pointers.size(); i++) {
			xml::xmlNodePtr pointInfo = pointers[i];
			if (pointInfo->getName() != "Pointer") continue;


			// �������� ����������
			std::string layer, material, defaultPointer;
			int size;
			// ������ ��������
			const xml::VectorAttributes & attrib = pointInfo->getAttributes();
			for (size_t ia=0; ia<attrib.size(); ia++) {
				// ������� ���� ������� - ��������
				const xml::PairAttributes & pairAttributes = attrib[ia];

				if (pairAttributes.first == "Layer") layer = pairAttributes.second;
				else if (pairAttributes.first == "Material") material = pairAttributes.second;
				else if (pairAttributes.first == "Default") defaultPointer = pairAttributes.second;
				else if (pairAttributes.first == "Size") size = parseInt(pairAttributes.second);

			}

			// ������������� ����� ���������
			m_overlayElement->setMaterialName(material);
			m_overlayElement->setDimensions(size, size);
			m_defaultPointer = defaultPointer;
			m_layer = layer;
			floatSize materialSize = SkinManager::getMaterialSize(material);

			
			// ����� ����� � ��������, �������� ����
			xml::VectorNode & info = pointInfo->getChilds();
			for (size_t j=0; j<info.size(); j++) {
				xml::xmlNodePtr infoInfo = info[j];
				if (infoInfo->getName() != "Info") continue;

				// �������� ����������
				std::string name;
				floatRect offset;
				intSize point;
				// ������ ��������
				const xml::VectorAttributes & attrib = infoInfo->getAttributes();
				for (size_t ia=0; ia<attrib.size(); ia++) {
					// ������� ���� ������� - ��������
					const xml::PairAttributes & pairAttributes = attrib[ia];

					if (pairAttributes.first == "Name") name = pairAttributes.second;
					else if (pairAttributes.first == "Point") point = intSize::parse(pairAttributes.second);
					else if (pairAttributes.first == "Offset") offset = SkinManager::convertMaterialCoord(floatRect::parse(pairAttributes.second), materialSize);
				}

				// ��������� ������
				m_mapPointers[name] = PointerInfo(offset, point);

			}

			// ��������� � ��������������
			if (m_defaultPointer.empty() && !m_mapPointers.empty()) m_defaultPointer = m_mapPointers.begin()->first;
			// ������������ � �������
			LayerManager::getInstance().attachItem(this, m_layer);

			this->defaultPointer();

			return; // ��� ����� ������ ���� �����

		} // for (size_t i=0; i<pointers.size(); i++) {
	}

	void PointerManager::clear()
	{
		m_layer.clear();
		m_defaultPointer.clear();
		m_material.clear();
		m_mapPointers.clear();
	}

	void PointerManager::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
	}

	void PointerManager::setPointer(const std::string & _name)
	{
		MapPointerInfo::iterator iter = m_mapPointers.find(_name);
		if (iter == m_mapPointers.end()) return;
		const floatRect & rect = iter->second.offset;
		m_overlayElement->setUV(rect.left, rect.top, rect.right, rect.bottom);
	}

} // namespace widget	