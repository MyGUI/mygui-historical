/*!
	@file
	@author		Denis Koronchik
	@author		Georgiy Evmenov
	@date		09/2007
*/

#ifndef __MYGUI_PREREQUEST_H__
#define __MYGUI_PREREQUEST_H__

#include "MyGUI_Platform.h"

#include <string>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include "utility.h"
#include "delegate.h"

namespace MyGUI
{
	class Gui;
	class DynLib;
	class Plugin;

	class WidgetSkinInfo;
	using delegates::newDelegate;
	class MaskPeekInfo;
	class PanelAlphaOverlayElement;

	// managers
	class InputManager;
	class SubWidgetManager;
	class ClipboardManager;
	class LayerManager;
	class SkinManager;
	class WidgetManager;
	class LayoutManager;
	class FontManager;
	class PointerManager;
	class DynLibManager;
	class PluginManager;
	class ControllerManager;

	// widgets
	class Button;
	class ComboBox;
	class Edit;
	class HScroll;
	class List;
	class Sheet;
	class StaticImage;
	class StaticText;
	class Tab;
	class VScroll;
	class Window;

	// widget pointers
	typedef Button* ButtonPtr;
	typedef ComboBox* ComboBoxPtr;
	typedef Edit * EditPtr;
	typedef HScroll * HScrollPtr;
	typedef List * ListPtr;
	typedef Sheet * SheetPtr;
	typedef StaticImage * StaticImagePtr;
	typedef StaticText* StaticTextPtr;
	typedef Tab* TabPtr;
	typedef VScroll* VScrollPtr;
	typedef Window * WindowPtr;

	class WidgetFactoryInterface;
	namespace factory{
		class WidgetFactory;
		class ButtonFactory;
		class ComboBoxFactory;
		class EditFactory;
		class HScrollFactory;
		class ListFactory;
		class SheetFactory;
		class StaticImageFactory;
		class StaticTextFactory;
		class TabFactory;
		class VScrollFactory;
		class WidgetFactory;
		class WindowFactory;
		class StrangeButtonFactory;
	}

	// Define version
    #define MYGUI_VERSION_MAJOR 2
    #define MYGUI_VERSION_MINOR 0
    #define MYGUI_VERSION_PATCH 1

    #define MYGUI_VERSION    ((MYGUI_VERSION_MAJOR << 16) | (MYGUI_VERSION_MINOR << 8) | MYGUI_VERSION_PATCH)

	// Disable warnings for MSVC compiler
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC

// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4786)

// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4503)

// disable: "conversion from 'double' to 'float', possible loss of data
#   pragma warning (disable : 4244)

// disable: "truncation from 'double' to 'float'
#   pragma warning (disable : 4305)

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251)

// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
#   pragma warning (disable : 4275)

// disable: "C++ Exception Specification ignored"
// This is because MSVC 6 did not implement all the C++ exception
// specifications in the ANSI C++ draft.
#   pragma warning( disable : 4290 )

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#   pragma warning( disable: 4661)

// disable: deprecation warnings when using CRT calls in VC8
// These show up on all C runtime lib code in VC8, disable since they clutter
// the warnings with things we may not be able to do anything about (e.g.
// generated code from nvparse etc). I doubt very much that these calls
// will ever be actually removed from VC anyway, it would break too much code.
#	pragma warning( disable: 4996)


#endif

} // namespace MyGUI

#endif // __MYGUI_PREREQUEST_H__
