/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#ifndef __MYGUI_RENDER_BOX_H__
#define __MYGUI_RENDER_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_FrameListener.h"

namespace MyGUI
{
	/** default RenderBox autorotation speed (if enabled) */
	const size_t RENDER_BOX_AUTO_ROTATION_SPEED = 20;

	/** Widget that show one entity or anything from viewport.
	*/
	class _MyGUIExport RenderBox : public Widget, public FrameListener
	{
		// ��� ������ ��������� ������������
		friend class factory::RenderBoxFactory;

	protected:
		RenderBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~RenderBox();

	public:
		// ��� ������� �������
		inline static const Ogre::String & _getType() {static Ogre::String type("RenderBox"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/** Add mesh to scene and remove previous one
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
			@param
				_meshName The name of the Mesh it is to be based on (e.g. 'ogrehead.mesh').
		*/
		void injectObject(const Ogre::String& _meshName);

		/** Clear scene */
		void clear();

		/** Set speed of entity rotation.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
			@param
				_speed of rotation in degrees per second.
		*/
		void setAutoRotationSpeed(int _speed = RENDER_BOX_AUTO_ROTATION_SPEED);
		/** Get speed of entity rotation.*/
		inline int getAutoRotationSpeed() {return mRotationSpeed;};

		/** Enable or disable auto rotation
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setAutoRotation(bool _auto);
		inline bool getAutoRotation() {return mAutoRotation;}

		/** Set colour behind entity.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setBackgroungColour(const Ogre::ColourValue & _backgroundColour);
		/** Get colour behind entity.*/
		inline const Ogre::ColourValue & getBackgroungColour() {return mBackgroungColour;};

		/** Set start rotation angle of entity.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setRotationAngle(const Ogre::Degree & _rotationAngle);

		/** Set possibility to rotate mesh by mouse drag.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setMouseRotation(bool _enable);

		/** Set any user created Camera instead of showing one mesh*/
		void setRenderTarget(Ogre::Camera * _camera);

		void setPosition(const IntCoord& _coord);
		void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

	protected:
		void _frameEntered(float _time);

		void _onMouseDrag(int _left, int _top);
		void _onMouseButtonPressed(bool _left);
		void _onMouseButtonReleased(bool _left);

	private:
		void createRenderTexture();
		void updateViewport();

		bool mUserViewport;
		// ���, ��� �������� �����
		Ogre::SceneManager * mScene;
		Ogre::Entity * mEntity;
		Ogre::SceneNode * mNode;
		Ogre::RenderTexture* mTexture;

		Ogre::Camera* mRttCam;
		Ogre::SceneNode* mCamNode;

		int mRotationSpeed;
		Ogre::ColourValue mBackgroungColour;
		bool mMouseRotation;
		int mLastPointerX;

		std::string mPointerKeeper;
		bool mLeftPressed;

		bool mAutoRotation;

	}; // class RenderBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_RENDER_BOX_H__
