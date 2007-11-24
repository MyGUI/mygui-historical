/*!
	@file		PluginManager.cpp
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#include "PluginManager.h"
#include "DynLibManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(PluginManager);

	void PluginManager::initialise()
	{
		assert(!mIsInitialise);

		LOG_MESSAGE("* Initialize: Plugin manager");

		LOG_MESSAGE("Manager successfully initialized");

		mIsInitialise = true;
	}

	void PluginManager::shutdown()
	{
		if (!mIsInitialise) return;

		LOG_MESSAGE("* Shut down: Plugin manager");
		unloadAllPlugins();

		mIsInitialise = false;
	}

	void PluginManager::loadPlugin(const std::string& _file)
	{
		// check initialise
		assert(mIsInitialise);
		// Load plugin library
		DynLib* lib = DynLibManager::getInstance().load( _file );
		// Store for later unload
		mLibs[_file] = lib;

		// Call startup function
		DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib->getSymbol("dllStartPlugin");

		/*Assert(pFunc, Exception::ERR_ITEM_NOT_FOUND, "Cannot find symbol dllStartPlugin in library " + fileName,
			"PluginManager::loadPlugin");*/

		assert(pFunc);// && ("Cannot find symbol dllStartPlugin in library " + fileName));

		// This must call installPlugin
		pFunc();
	}

	void PluginManager::unloadPlugin(const std::string& _file)
	{
		// check initialise
		assert(mIsInitialise);

		DynLibList::iterator it = mLibs.find(_file);
		if (it != mLibs.end()) {
			// Call plugin shutdown
			DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*it).second->getSymbol("dllStopPlugin");
			// this must call uninstallPlugin
			pFunc();
			// Unload library (destroyed by DynLibManager)
			DynLibManager::getInstance().unload((*it).second);
			mLibs.erase(it);
		}
	}

	void PluginManager::installPlugin(Plugin* _plugin)
	{
		// check initialise
		assert(mIsInitialise);

		LOG_MESSAGE("Installing plugin: " + _plugin->getName());

		mPlugins.insert(_plugin);
		_plugin->install();

		_plugin->initialize();
		
		LOG_MESSAGE("Plugin successfully installed");
	}

	void PluginManager::uninstallPlugin(Plugin* _plugin)
	{
		// check initialise
		assert(mIsInitialise);

		LOG_MESSAGE("Uninstalling plugin: " + _plugin->getName());
		PluginList::iterator it = mPlugins.find(_plugin);
		if (it != mPlugins.end())
		{
			_plugin->shutdown();
			_plugin->uninstall();
			mPlugins.erase(it);
		}
		LOG_MESSAGE("Plugin successfully uninstalled");
	}

	void PluginManager::unloadAllPlugins()
	{
		while (false == mLibs.empty())
			unloadPlugin((*mLibs.begin()).first);
	}

} // namespace MyGUI