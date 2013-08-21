#include <OGRE/Ogre.h>
#include <OGRE/OgrePlugin.h>
#include "NuiManagerPrerequisites.h"
#include "NuiDataType.h"

class NuiManager : public Ogre::Plugin
{
public:
	NuiManager():mState(NUI_MANAGER_UNINITIATED),mFrameRate(0) {}
	virtual ~NuiManager() {}
	virtual const Ogre::String& getName() const = 0;
	virtual void install() = 0;
	virtual void initialise() = 0;
	virtual void shutdown() = 0;
	virtual void uninstall() = 0;

	virtual Ogre::Quaternion* getBoneOrientations() { return mBoneOrientationsH; }
	virtual Ogre::Quaternion* getBoneOrientationsH() { return mBoneOrientationsH; } // Hierarchical
	virtual Ogre::Quaternion* getBoneOrientationsA() { return mBoneOrientationsA; } // Absolute

	virtual const Ogre::String& getCorrespondingOgreBone(int index, bool mirror=false) = 0;

	NuiManagerState getState() { return mState; }
	int getFrameRate() { return mFrameRate; }

protected:

	NuiManagerState mState;
	int mFrameRate;
	Ogre::Quaternion* mBoneOrientationsH;
	Ogre::Quaternion* mBoneOrientationsA;
};