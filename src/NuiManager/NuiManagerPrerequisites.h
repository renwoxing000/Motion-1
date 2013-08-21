#ifndef __NuiManagerPrerequisites_H__
#define __NuiManagerPrerequisites_H__

#include <OGRE\OgrePrerequisites.h>
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   ifdef NUIMANAGER__EXPORTS
#       define _NuiManagerExport __declspec(dllexport) 
#   else 
#       define _NuiManagerExport __declspec(dllimport) 
#   endif 
#else 
#   define _NuiManagerExport 
#endif
#endif