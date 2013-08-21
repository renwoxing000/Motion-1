Motion
======

Motion Studio built on Ogre SDK and Kinect sensor.


## Develop Environment

* Kinect SDK 1.7
* OgreSDK 1.9


## Working Directory Overview

* bin
    * Debug : All debug-version dlls and exes are built here.
    * Release : All release-version dlls and exes are built here.
* src
    * Motion : An Ogre application as the entry.
    * Activity : Class based on OgreBites::SdkSample to realize real functions and to be loaded by Motion.
    * NuiManager : A class based on Ogre::Plugin providing an interface to fetch skeleton data from Kinect.
    * NuiManager_xx : Class based on NuiManager to realize functions based on the API of Kinect drivers such as KinectSDK or OpenNI.
* media
    * thumbnails : thumbnails of activites are put here.


## Working Environment Setup

**IMPORTANT** : To make all building activities right, you should finish this setup at first.

* bin
    * Debug : Copy all Ogre and its plugins's debug-version dlls here. (and Sample_Character_d.dll, sample_character_d.pdb to test)
    * Release : Copy all Ogre and its plugins's debug-version dlls here. (and Sample_Character.dll, sample_character.pdb to test)
* media : Copy media\\* in OgreSDK directory here
* Visio Studio user macro setup
    * After opening any project in VS, choose menu -> view -> Property Manager;
    * Find and open Microsoft.Cpp.Win32.user, add a user macro like this
        * Macro : "OgreDir"
        * Value : "Path to Ogre" (e.g. "E:\lib\OgreSDK_vc11_v1-9-0unstable\")


## License

Copyright 2013 Aidi Stan under the GPL 2.0 license.



