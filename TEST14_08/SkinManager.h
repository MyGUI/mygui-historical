#pragma once

#include <Ogre.h>
#include <string>
#include "Instance.h"
#include "WidgetDefines.h"


namespace widget
{

	class SkinManager
	{
	private:
		SkinManager();
		~SkinManager();

	public:
		INSTANCE(SkinManager)

		char parseAlign(const std::string & _value);
		WidgetSkinInfo * getSkin(const Ogre::String & _name);

		//	��� ������� �������� �����
		WidgetSkinInfo * create(const Ogre::String & _name);

		void loadSkin(const std::string & _file);

//		static bool getMaterialSize(float & _cx, float & _cy, const std::string & _material);
		static floatSize getMaterialSize(const std::string & _material);
//		static floatRect convertMaterialCoord(const floatRect & _source, float _cx, float _cy);
		static floatRect convertMaterialCoord(const floatRect & _source, const floatSize & _materialSize);

	private:
		void initialise();
		void createDefault();

	private:
		SkinInfo m_skins;
		MapFlags m_mapAlign;

	}; // class SkinManager

} // namespace widget