#ifndef __DynamicRebuilding_H__
#define __DynamicRebuilding_H__

#include <OGRE\SdkSample.h>
#include "CharacterController.h"

using namespace Ogre;
using namespace OgreBites;

#define CAMERA_DISTANCE_MIN  15
#define CAMERA_DISTANCE_MAX  40

class _OgreSampleClassExport Activity_DynamicRebuilding : public SdkSample
{
public:

	Activity_DynamicRebuilding()
	{
		mInfo["Title"] = "Dynamic Rebuilding";
		mInfo["Description"] = "A demo showing 3rd-person character and use of Kinect.";
		mInfo["Thumbnail"] = "thumb_DynamicRebuilding.png";
		mInfo["Category"] = "Animation";
		mInfo["Help"] = "Link your Kinect before starting the activity.";
	}

	bool frameRenderingQueued(const FrameEvent& evt)
	{
		// let character update animations and camera
		mChara->update(evt.timeSinceLastFrame);
		return SdkSample::frameRenderingQueued(evt);
	}
	
	bool keyPressed(const OIS::KeyEvent& evt)
	{
		// key filter
		switch (evt.key)
		{
		// keys we don't want to pass to SdkSample
		//case OIS::KC_R:
		//case OIS::KC_T:
		case OIS::KC_F2:
		case OIS::KC_F3:
		case OIS::KC_F4:
		case OIS::KC_F5:
			return true;
		}
		
		// otherwise
		return SdkSample::keyPressed(evt);
	}

	bool mouseMoved(const OIS::MouseEvent& evt)
	{
		bool result = SdkSample::mouseMoved(evt);
		limitCamera();
		return result;
	}

	/*========================================================================
	| Widgets
	========================================================================*/
	void sliderMoved(Slider* slider)
	{
		// use the values from the sliders to update the skydome properties
		mSceneMgr->setSkyDome(true, "Examples/CloudySky", mCurvatureSlider->getValue(), mTilingSlider->getValue());
	}

protected:

	void setupContent()
	{   
		// setup some basic lighting for our scene
        mSceneMgr->setAmbientLight(ColourValue(0.4f, 0.4f, 0.4f));
        mSceneMgr->createLight()->setPosition(-20, 80, -50);

		// create the model
		mChara = new CharacterController(mSceneMgr);

		// set our camera to orbit around the model
		mCameraMan->setStyle(CS_ORBIT);
		mCameraMan->setTarget(mChara->getBodyNode());
		mCameraMan->setYawPitchDist(Degree(180), Degree(10), 25);
		mCameraMan->setTopSpeed(20);

		// set TrayMgr up
		mTrayMgr->showCursor();
		//mTrayMgr->hideFrameStats();

		// create a floor mesh resource
		MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Vector3::UNIT_Y, 0), 1000, 1000, 100, 100, true, 1, 100, 100, Vector3::UNIT_Z);

		// create a floor entity, give it a material, and place it at the origin
        Entity* floor = mSceneMgr->createEntity("Floor", "floor");
        floor->setMaterialName("Examples/GrassFloor");
		floor->setCastShadows(false);
        mSceneMgr->getRootSceneNode()->attachObject(floor);

		setupControls();
	}

	void setupControls()
	{
		// create slider bars to control the dome curvature and texture tiling
		mCurvatureSlider = mTrayMgr->createThickSlider(TL_TOPLEFT, "Curvature", "Dome Curvature", 200, 60, 0, 50, 11);
		mTilingSlider = mTrayMgr->createThickSlider(TL_TOPLEFT, "Tiling", "Dome Tiling", 200, 60, 1, 20, 191);
		//mTestingSlider = mTrayMgr->createThickSlider(TL_TOPLEFT, "Testing", "Test Value", 200, 60, 1, 200, 200);

		/* Here, we set default values for our sliders. We do not need to setup a skydome here, because when
		slider values change, the sliderMoved callback is invoked, and we setup the skydome with the appropriate
		values in there. See its definition above. */
		mCurvatureSlider->setValue(10);
		mTilingSlider->setValue(8);

		// Temporarily
		mTrayMgr->removeWidgetFromTray(mCurvatureSlider);
		mCurvatureSlider->hide();
		mTrayMgr->removeWidgetFromTray(mTilingSlider);
		mTilingSlider->hide();
	}

	void cleanupContent()
	{
		// clean up character controller and the floor mesh
		if (mChara)
		{
			delete mChara;
			mChara = 0;
		}
		MeshManager::getSingleton().remove("floor");
	}

	/*------------------------------------------------------------------------
	| Keep distance between Camera and Character
	------------------------------------------------------------------------*/
	void limitCamera()
	{
		// keep distance
		Ogre::Real dist = (mCameraMan->getCamera()->getPosition() - mCameraMan->getTarget()->_getDerivedPosition()).length();
		if (dist > CAMERA_DISTANCE_MAX)
		{
			mCameraMan->getCamera()->moveRelative(Ogre::Vector3(0, 0, CAMERA_DISTANCE_MAX-dist));
		}
		else if (dist < CAMERA_DISTANCE_MIN)
		{
			mCameraMan->getCamera()->moveRelative(Ogre::Vector3(0, 0, CAMERA_DISTANCE_MIN-dist));
		}
	}

	Slider* mCurvatureSlider;
	Slider* mTilingSlider;

	Slider* mTestingSlider;

	CharacterController *mChara;
};

#endif