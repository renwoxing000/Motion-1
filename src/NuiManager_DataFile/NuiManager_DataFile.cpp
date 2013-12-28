#include "NuiManager_DataFile.h"

const Ogre::String sPluginName = "NuiManager_DataFile";

const Ogre::String sOgreBoneNames[] = { "mPelvis","mTorso","mChest","mHead",
	"mCollarLeft","mShoulderLeft","mElbowLeft","mWristLeft","mCollarRight","mShoulderRight","mElbowRight","mWristRight",
	"mHipLeft","mKneeLeft","mAnkleLeft","mFootLeft","mHipRight","mKneeRight","mAnkleRight","mFootRight"};

NuiManager_DataFile::NuiManager_DataFile():NuiManager()
{
}

NuiManager_DataFile::~NuiManager_DataFile()
{
	for (std::vector<Ogre::Quaternion*>::iterator it=mOrientationData.begin(); it != mOrientationData.end(); ++it)
		delete *it;
}

const Ogre::String& NuiManager_DataFile::getName() const
{
  return sPluginName;
}

void NuiManager_DataFile::install()
{
	std::ifstream dataFile("orientation.txt", std::ifstream::in);
	if(!dataFile.is_open())
	{
		mState = NUI_MANAGER_NO_SENSOR;
		return;
	}

	while(!dataFile.eof())
	{
		mOrientationData.push_back(new Ogre::Quaternion[NUI_SKELETON_POSITION_COUNT]);
		for(int i=0; i<NUI_SKELETON_POSITION_COUNT; i++)
		{
			dataFile>>mOrientationData.back()[i].x;
			dataFile>>mOrientationData.back()[i].y;
			dataFile>>mOrientationData.back()[i].z;
			dataFile>>mOrientationData.back()[i].w;
		}
	}
	
	// Start the Nui processing thread
	LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -10000000LL;

	mNextSkeletonEvent = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(mNextSkeletonEvent, &liDueTime, 30, NULL, NULL, 0);
	mEvNuiProcessStop = CreateEvent( NULL, FALSE, FALSE, NULL );
	mThNuiProcess = CreateThread( NULL, 0, nuiProcessThread, this, 0, NULL );

	mState = NUI_MANAGER_OK;
	dataFile.close();
}

void NuiManager_DataFile::uninstall()
{
	// stop the Nui processing thread
	if ( mEvNuiProcessStop != NULL )
	{
		// Signal the thread
		SetEvent(mEvNuiProcessStop);

		// Wait for thread to stop
		if ( mThNuiProcess != NULL )
		{
			WaitForSingleObject( mThNuiProcess, INFINITE );
			CloseHandle( mThNuiProcess );
		}
		CloseHandle( mEvNuiProcessStop );
	}

	if (mNextSkeletonEvent && (mNextSkeletonEvent != INVALID_HANDLE_VALUE))
	{
		CancelWaitableTimer(mNextSkeletonEvent);
		CloseHandle(mNextSkeletonEvent);
		mNextSkeletonEvent = NULL;
	}
}

/*----------------------------------------------------------------------------
| Thread to dispatch
----------------------------------------------------------------------------*/
DWORD WINAPI NuiManager_DataFile::nuiProcessThread(LPVOID pParam)
{
	NuiManager_DataFile *mgr = (NuiManager_DataFile *) pParam;
	return mgr->nuiProcessThread();
}

/*----------------------------------------------------------------------------
| Thread to handle Kinect processing
----------------------------------------------------------------------------*/
DWORD WINAPI NuiManager_DataFile::nuiProcessThread()
{
	const int numEvents = 2;
	HANDLE hEvents[numEvents] = { mEvNuiProcessStop, mNextSkeletonEvent };
	int    nEventIdx;
	DWORD  t;

	DWORD lastFrameTime = timeGetTime();
	int   framesTotal = 0;
	int   lastFramesTotal = 0;

	// thread loop
	bool continueProcessing = true;
	while(continueProcessing)
	{
		// wait for any of the events to be signaled
		nEventIdx = WaitForMultipleObjects(numEvents, hEvents, FALSE, 100);

		// process signal events
		if(nEventIdx == WAIT_TIMEOUT)
		{
			continue;
		}
		else if(nEventIdx == WAIT_OBJECT_0)
		{
			continueProcessing = false;
			continue;
		}
		else if(nEventIdx == (WAIT_OBJECT_0 + 1))
		{
			mBoneOrientationsA = mOrientationData[framesTotal % (int)mOrientationData.size()];
			mBoneOrientationsH = mOrientationData[framesTotal % (int)mOrientationData.size()];
			++framesTotal;
		}

		t = timeGetTime();
		if((t - lastFrameTime) > 1000)
		{
			mFrameRate = ((framesTotal - lastFramesTotal) * 1000 + 500) / (t - lastFrameTime);
			lastFramesTotal = framesTotal;
			lastFrameTime = t;
		}
	}

	return 0;
}

const Ogre::String& NuiManager_DataFile::getCorrespondingOgreBone(int index, bool mirror)
{
	if (mirror)
	{
		if (index < 4){
			return sOgreBoneNames[index];
		} else if (index >= 4 && index <= 7) {
			return sOgreBoneNames[index+4];
		} else if (index >= 8 && index <= 11) {
			return sOgreBoneNames[index-4];
		} else if (index >= 12 && index <= 15) {
			return sOgreBoneNames[index+4];
		} else { // if (index >= 16 && index <= 19)
			return sOgreBoneNames[index-4];
		}
	}
	else
	{
		return sOgreBoneNames[index];
	}
}
