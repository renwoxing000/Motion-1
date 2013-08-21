#ifndef __CharacterContoller_H__
#define __CharacterContoller_H__

#include <OGRE\Ogre.h>
#include <OIS\OIS.h>
#include "../NuiManager/NuiManager.h"
#include "../NuiManager_KinectSDK/NuiDataType_KinectSDK.h"

using namespace Ogre;

#define CHAR_HEIGHT 5          // height of character's center of mass above ground

class CharacterController
{
public:
	
	CharacterController(SceneManager* sceneMgr)
	{
		mNuiMgr = NULL;

		// Create main model
		mCharaNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * CHAR_HEIGHT);
		mCharaEnt = sceneMgr->createEntity("SinbadBody", "Sinbad.mesh");
		mCharaSkl = mCharaEnt->getSkeleton();
		mCharaNode->attachObject(mCharaEnt);
		
		// Get NuiManager
		Root::PluginInstanceList list = Root::getSingletonPtr()->getInstalledPlugins();
		for(int i=0; i<(int)list.size(); i++)
		{
			if (list[i]->getName() == "NuiManager_KinectSDK")
			{
				mNuiMgr = dynamic_cast<NuiManager*>(list[i]);
				break;
			}
		}
		if (mNuiMgr == NULL)
		{
			return;
		}

		// Set skeleton manually controlled
		for(int i=0; i<NUI_MANAGER_BONE_COUNT; i++)
		{
			mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(i))->setManuallyControlled(true);
		}
	}

	void update(Real deltaTime)
	{
		if (mNuiMgr)
		{
			for(int i=1; i<NUI_MANAGER_BONE_COUNT; i++)
			{
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(i))->setOrientation( mNuiMgr->getBoneOrientationsH()[i] );
			}
		}
	}

	Ogre::SceneNode* getBodyNode()
	{
		return mCharaNode;
	}

private:

	SceneNode*  mCharaNode;
	Entity*     mCharaEnt;
	Skeleton*   mCharaSkl;

	NuiManager* mNuiMgr;
};

#endif
