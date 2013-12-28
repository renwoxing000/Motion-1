#include <OGRE\OgreRoot.h>
#include "NuiManager_DataFile.h"
 
NuiManager_DataFile* nuiMgr;
 
extern "C" void _NuiManagerExport dllStartPlugin(void)
{
  // Create plugin
  nuiMgr = new NuiManager_DataFile();
 
  // Register
  Ogre::Root::getSingleton().installPlugin(nuiMgr);
}

extern "C" void _NuiManagerExport dllStopPlugin(void)
{
  Ogre::Root::getSingleton().uninstallPlugin(nuiMgr);
  delete nuiMgr;
}