#include <OGRE/Ogre.h>
#include <OGRE/OgrePlugin.h>
#include <NuiApi.h>
#include <fstream>
#include <vector>
#include "../NuiManager/NuiManager.h"
#include "NuiDataType_DataFile.h"

class NuiManager_DataFile : public NuiManager
{
public:
	NuiManager_DataFile();
	~NuiManager_DataFile();
	const Ogre::String& getName() const;
	void install();
	void initialise() {}
	void shutdown() {}
	void uninstall();

	const Ogre::String& getCorrespondingOgreBone(int index, bool mirror=false);

private:
	static DWORD WINAPI     nuiProcessThread(LPVOID pParam);
	DWORD WINAPI            nuiProcessThread();

	std::vector<Ogre::Quaternion*> mOrientationData;

	HANDLE mThNuiProcess;
	HANDLE mEvNuiProcessStop;
	HANDLE mNextSkeletonEvent;
};