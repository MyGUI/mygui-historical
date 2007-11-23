#ifndef __MYGUI_FONT_MANAGER_H__
#define __MYGUI_FONT_MANAGER_H__

#include "Prerequest.h"
#include "OgrePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreResourceManager.h"
#include "MyGUI_Font.h"
#include "Instance.h"

namespace MyGUI
{

	class _MyGUIExport FontManager : public Ogre::ResourceManager
	{
		INSTANCE_HEADER(FontManager);

	public:
		bool load(const std::string & _file);

    protected:

        /// Internal methods
		Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
			const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
			const Ogre::NameValuePairList* params);

	}; // class _MyGUIExport FontManager : public Ogre::ResourceManager

} // namespace MyGUI

#endif // __MYGUI_FONT_MANAGER_H__