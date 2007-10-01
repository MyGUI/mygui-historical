#ifndef _SCROLLPARSER_H_
#define _SCROLLPARSER_H_

#include "Prerequest.h"
#include "delegate.h"
#include "ParserManager.h"
#include "ScrollBase.h"

namespace MyGUI
{

	namespace parser
	{

		class _MyGUIExport ScrollParser
		{
		public:
			ScrollParser()
			{
				// �������� ���� �������� �������
				ParserManager & parser = ParserManager::getInstance();

				parser.registerDelegate("ScrollRange") = newDelegate(this, &ScrollParser::ScrollRange);
				parser.registerDelegate("ScrollPosition") = newDelegate(this, &ScrollParser::ScrollPosition);
				parser.registerDelegate("ScrollPage") = newDelegate(this, &ScrollParser::ScrollPage);
			}

			void ScrollRange(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(ScrollBasePtr, _widget);
				static_cast<ScrollBasePtr>(_widget)->setScrollRange(util::parseSizeT(_value));
			}

			void ScrollPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(ScrollBasePtr, _widget);
				static_cast<ScrollBasePtr>(_widget)->setScrollPosition(util::parseSizeT(_value));
			}

			void ScrollPage(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				TYPE(ScrollBasePtr, _widget);
				static_cast<ScrollBasePtr>(_widget)->setScrollPage(util::parseSizeT(_value));
			}

		};

	} // namespace parser

} // namespace MyGUI


#endif // _SCROLLPARSER_H_