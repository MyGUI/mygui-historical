/*!
	@file		DynLibManager.cpp
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#include "DynLibManager.h"


namespace MyGUI
{

	INSTANCE_IMPLEMENT(DynLibManager);

	void DynLibManager::initialise()
	{
		assert(!mIsInitialise);

		LOG("* Initialize: Dynamic Library Manager");

		LOG("Manager successfully initialized");

		mIsInitialise = true;
	}

	void DynLibManager::shutdown()
	{
		if (!mIsInitialise) return;

		LOG("* Shut down: Dynamic Library Manager");

		StringDynLibMap::iterator it;

		// unload and delete resources
		for (it = mLibsMap.begin(); it != mLibsMap.end(); it++)
		{
			it->second->unload();
			delete it->second;
		}

		// Empty the list
		mLibsMap.clear();

		mIsInitialise = false;
	}

	DynLib* DynLibManager::load(const std::string &fileName)
	{
		StringDynLibMap::iterator it = mLibsMap.find(fileName);

		if (it != mLibsMap.end())
			return it->second;
		else
			{
				DynLib *pLib = new DynLib(fileName);
				pLib->load();
				mLibsMap[fileName] = pLib;
				return pLib;
			}

		return 0;
	}

	void DynLibManager::unload(DynLib *library)
	{
		StringDynLibMap::iterator it = mLibsMap.find(library->getName());

		if (it != mLibsMap.end())
			mLibsMap.erase(it);

		library->unload();
		delete library;
	}

}