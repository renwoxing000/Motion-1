#include <OGRE/Ogre.h>
#include <OGRE/OgrePlugin.h>
#include <NuiApi.h>
#include "NuiManager.h"
#include "NuiDataType_KinectSDK.h"

class NuiManager_KinectSDK : public NuiManager
{
public:
	NuiManager_KinectSDK();
	~NuiManager_KinectSDK();
	const Ogre::String& getName() const;
	void install();
	void initialise() {}
	void shutdown() {}
	void uninstall();

	const Ogre::String& getCorrespondingOgreBone(int index, bool mirror=false);

private:
	static DWORD WINAPI     nuiProcessThread(LPVOID pParam);
	DWORD WINAPI            nuiProcessThread();

	bool trackSkeleton();
	
	INuiSensor*     mNuiSensor;
	int				mPlayerIndex;

	HANDLE mThNuiProcess;
	HANDLE mEvNuiProcessStop;
	HANDLE mNextSkeletonEvent;
};