#ifndef __CharacterContoller_H__
#define __CharacterContoller_H__

#include <OGRE\Ogre.h>
#include <OIS\OIS.h>
#include "../NuiManager/NuiManager.h"
#include "../NuiManager_KinectSDK/NuiDataType_KinectSDK.h"

using namespace Ogre;

#define CHAR_HEIGHT 5          // height of character's center of mass above ground

#define MODELBONENUMS 26
const Ogre::String sModelBoneNames[] = { "mPelvis","mTorso","mChest","mHead",
	"mCollarLeft","mShoulderLeft","mElbowLeft","mWristLeft","mCollarRight","mShoulderRight","mElbowRight","mWristRight",
	"mHipLeft","mKneeLeft","mAnkleLeft","mFootLeft","mHipRight","mKneeRight","mAnkleRight","mFootRight",
	"mToeLeft","mToeRight","mNeck","mSkull","mEyeLeft","mCollarLeft"};

class CharacterController
{
public:
	
	CharacterController(SceneManager* sceneMgr)
	{
		mNuiMgr = NULL;

		// Create main model
		mCharaNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * CHAR_HEIGHT);
		mCharaEnt = sceneMgr->createEntity("RTBody", "slrig_model001.mesh");
		mCharaNode->setPosition(0,9,0);
		mCharaSkl = mCharaEnt->getSkeleton();
		mCharaNode->attachObject(mCharaEnt);

		// Get NuiManager
		Root::PluginInstanceList list = Root::getSingletonPtr()->getInstalledPlugins();
		for(int i=0; i<(int)list.size(); i++)
		{
			//if (list[i]->getName() == "NuiManager_KinectSDK")
			if (list[i]->getName() == "NuiManager_DataFile")
			{
				mNuiMgr = dynamic_cast<NuiManager*>(list[i]);
				break;
			}
		}
		if (mNuiMgr == NULL)
		{
			return;
		}

	}

	void update(Real deltaTime)
	{
		if (mNuiMgr)
		{
			
				
				setupBone(sModelBoneNames[0],Degree(0),Degree(90),Degree(0));
				setupBone(sModelBoneNames[1],Degree(0),Degree(-10),Degree(0));
				setupBone(sModelBoneNames[2],Degree(0),Degree(-10),Degree(0));
				setupBone(sModelBoneNames[3],Degree(0),Degree(30),Degree(0));
				setupBone(sModelBoneNames[5],Degree(0),Degree(0),Degree(-90));
				setupBone(sModelBoneNames[6],Degree(0),Degree(0),Degree(-90));
				setupBone(sModelBoneNames[7],Degree(0),Degree(0),Degree(-90));
				setupBone(sModelBoneNames[9],Degree(0),Degree(0),Degree(90));
				setupBone(sModelBoneNames[10],Degree(0),Degree(0),Degree(90));
				setupBone(sModelBoneNames[11],Degree(0),Degree(0),Degree(90));
				setupBone(sModelBoneNames[12],Degree(0),Degree(180),Degree(0));
				setupBone(sModelBoneNames[13],Degree(180),Degree(180),Degree(0));
				setupBone(sModelBoneNames[14],Degree(0),Degree(110),Degree(0));
				setupBone(sModelBoneNames[15],Degree(0),Degree(80),Degree(0));
				setupBone(sModelBoneNames[16],Degree(0),Degree(180),Degree(0));
				setupBone(sModelBoneNames[17],Degree(180),Degree(180),Degree(0));
				setupBone(sModelBoneNames[18],Degree(0),Degree(110),Degree(0));
				setupBone(sModelBoneNames[19],Degree(0),Degree(80),Degree(0));

			/*	Quaternion q1(Degree(-90), Vector3::UNIT_X );
				Quaternion q2(Degree(180), Vector3::UNIT_Y );
				Quaternion q3(Degree(180), Vector3::UNIT_Z );*/
				Quaternion q1(Degree(90), Vector3::UNIT_X );
				Quaternion q(Degree(180), Vector3::UNIT_Y );
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(0))->setOrientation(mNuiMgr->getBoneOrientationsA()[0]*q1);
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(1))->setOrientation(mNuiMgr->getBoneOrientationsA()[1]);
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(2))->setOrientation(mNuiMgr->getBoneOrientationsA()[2]);
				Quaternion q2(Degree(90), Vector3::UNIT_X );
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(3))->setOrientation(mNuiMgr->getBoneOrientationsA()[3]*q2);
				Quaternion q3(Degree(-15), Vector3::UNIT_Z );
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(5))->setOrientation(mNuiMgr->getBoneOrientationsA()[5]*q3);
				//Quaternion q5(Degree(180), Vector3::UNIT_Y );
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(6))->setOrientation(mNuiMgr->getBoneOrientationsA()[6]);
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(7))->setOrientation(mNuiMgr->getBoneOrientationsA()[7]);
				Quaternion q8(Degree(15), Vector3::UNIT_Z );
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(9))->setOrientation(mNuiMgr->getBoneOrientationsA()[9]*q8);
				//Quaternion q9(Degree(180), Vector3::UNIT_Y );
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(10))->setOrientation(mNuiMgr->getBoneOrientationsA()[10]);
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(11))->setOrientation(mNuiMgr->getBoneOrientationsA()[11]);
				Quaternion q12(Degree(-45), Vector3::UNIT_Z );
				Quaternion q122(Degree(180), Vector3::UNIT_Y );
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(12))->setOrientation(mNuiMgr->getBoneOrientationsA()[13]*q122);
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(13))->setOrientation(mNuiMgr->getBoneOrientationsA()[14]);
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(14))->setOrientation(mNuiMgr->getBoneOrientationsA()[15]);
				//mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(15))->setOrientation(mNuiMgr->getBoneOrientationsA()[15]);
				Quaternion q16(Degree(45), Vector3::UNIT_Z );
				Quaternion q162(Degree(180), Vector3::UNIT_Y );
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(16))->setOrientation(mNuiMgr->getBoneOrientationsA()[17]*q162);
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(17))->setOrientation(mNuiMgr->getBoneOrientationsA()[18]);
				mCharaSkl->getBone(mNuiMgr->getCorrespondingOgreBone(18))->setOrientation(mNuiMgr->getBoneOrientationsA()[19]);
				
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

	void setupBone(const String& name,const Ogre::Radian& angle, const Vector3 axis)
	{
		
		Quaternion q;
		q.FromAngleAxis(angle,axis);	 
		setupBone(name, q);

	}
	void setupBone(const String& name,const Degree& yaw,const Degree& pitch,const Degree& roll)
	{
		Ogre::Bone* bone = mCharaEnt->getSkeleton()->getBone(name);
		bone->setManuallyControlled(true);
		bone->setInheritOrientation(false);
		
		bone->resetOrientation();
		
		bone->yaw(yaw);
		bone->pitch(pitch);
		bone->roll(roll);
	
		//Matrix3 mat = bone->getLocalAxes();
		bone->setInitialState();

	}
	void setupBone(const String& name,const Ogre::Quaternion& q)
	{
		Ogre::Bone* bone = mCharaEnt->getSkeleton()->getBone(name);
		bone->setManuallyControlled(true);
		bone->setInheritOrientation(false);
		
		bone->resetOrientation();
		bone->setOrientation(q);
	
		bone->setInitialState();
	}


};

#endif
