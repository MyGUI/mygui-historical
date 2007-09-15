#include "MyGUI_GUI.h"
#include "MyGUI_WidgetFactory.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_AssetManager.h"

using namespace Ogre;
using namespace std;

namespace MyGUI
{

	class GUI;

	StaticText::StaticText(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, strMaterialElement, uOverlay, pWindowFother)
	{
	}
	
	StaticText *StaticText::createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
    {
        
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		StaticText * pWindow = new StaticText(pSkin->subSkins[0],
			pSkin->SkinElement,
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
		    
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // ������� �������� ���� �����
			Window *pChild = new Window(pSkin->subSkins[pos], pSkin->SkinElement, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
		}
		pWindow->m_uAlign |= (uAlign | WAT_CUT_RIGHT);
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		
		pWindow->move(PosX, PosY);
		pWindow->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
		return pWindow;	
    }

	// ������������ �������
	REGISTRY_WIDGET_FACTORY(WIDGET_STATIC_TEXT, StaticText);

} // namespace MyGUI