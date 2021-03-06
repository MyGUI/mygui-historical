/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_PROGRESS_H__
#define __MYGUI_PROGRESS_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_FrameListener.h"

namespace MyGUI
{

	class _MyGUIExport Progress : public Widget, public FrameListener
	{
		// ��� ������ ��������� ������������
		friend class factory::ProgressFactory;

	protected:
		Progress(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);
		~Progress();

		void _frameEntered(float _time);

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("Progress"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		void setProgressRange(size_t _range);
		inline size_t getProgressRange() {return mRange;}

		void setProgressPosition(size_t _pos);
		inline size_t getProgressPosition() {return mStartPosition;}

		void setProgressAutoTrack(bool _auto);

		void setPosition(const IntCoord& _coord);
		void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

	private:
		void updateTrack();

	private:
		std::string mTrackSkin;
		int mTrackWidth;
		WidgetPtr mClient;

		VectorWidgetPtr mVectorTrack;
		size_t mRange;
		size_t mStartPosition, mEndPosition;
		float mAutoPosition;
		bool mAutoTrack;

	}; // class _MyGUIExport Progress : public Widget

} // namespace MyGUI

#endif // __MYGUI_PROGRESS_H__
