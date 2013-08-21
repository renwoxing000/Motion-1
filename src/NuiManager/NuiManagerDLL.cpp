#include <OGRE\OgreRoot.h>
#include "NuiManager_KinectSDK.h"
 
NuiManager_KinectSDK* nuiMgr;
 
extern "C" void _NuiManagerExport dllStartPlugin(void)
{
  // Create plugin
  nuiMgr = new NuiManager_KinectSDK();
 
  // Register
  Ogre::Root::getSingleton().installPlugin(nuiMgr);
}

extern "C" void _NuiManagerExport dllStopPlugin(void)
{
  Ogre::Root::getSingleton().uninstallPlugin(nuiMgr);
  delete nuiMgr;
}