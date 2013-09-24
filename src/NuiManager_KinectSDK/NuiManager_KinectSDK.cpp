#include "NuiManager_KinectSDK.h"

const Ogre::String sPluginName = "NuiManager_KinectSDK";

const Ogre::String sOgreBoneNames[] = { "mPelvis","mTorso","mChest","mHead",
	"mCollarLeft","mShoulderLeft","mElbowLeft","mWristLeft","mCollarRight","mShoulderRight","mElbowRight","mWristRight",
	"mHipLeft","mKneeLeft","mAnkleLeft","mFootLeft","mHipRight","mKneeRight","mAnkleRight","mFootRight"};


NuiManager_KinectSDK::NuiManager_KinectSDK():NuiManager()
{
	mNuiSensor = NULL;
	mPlayerIndex = 0;

	// Inherited
	mBoneOrientationsA = new Ogre::Quaternion[NUI_SKELETON_POSITION_COUNT];
	mBoneOrientationsH = new Ogre::Quaternion[NUI_SKELETON_POSITION_COUNT];
}

NuiManager_KinectSDK::~NuiManager_KinectSDK()
{
	delete[] mBoneOrientationsA;
	delete[] mBoneOrientationsH;
}

const Ogre::String& NuiManager_KinectSDK::getName() const
{
  return sPluginName;
}

void NuiManager_KinectSDK::install()
{
	HRESULT hr;

	// Get the count
	int iSensorCount = 0;
    hr = NuiGetSensorCount(&iSensorCount);
    if (FAILED(hr))
    {
		mState = NUI_MANAGER_API_ERROR;
        return;
    }
	else if (iSensorCount == 0)
	{
		mState = NUI_MANAGER_NO_SENSOR;
		return;
	}

	// Look at each Kinect sensor
	INuiSensor * pNuiSensor;
    for (int i = 0; i < iSensorCount; ++i)
    {
        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &pNuiSensor);
        if (FAILED(hr))
        {
            continue;
        }

        // Get the status of the sensor, and if connected, then we can initialize it
        hr = pNuiSensor->NuiStatus();
        if (S_OK == hr)
        {
            mNuiSensor = pNuiSensor;
            break;
        }

        // This sensor wasn't OK, so release it since we're not using it
        pNuiSensor->Release();
    }

	if (NULL != mNuiSensor)
    {
        // Initialize the Kinect and specify that we'll be using skeleton
        hr = mNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON); 
        if (SUCCEEDED(hr))
        {
            // Create an event that will be signaled when skeleton data is available
            mNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

            // Open a skeleton stream to receive skeleton data
            hr = mNuiSensor->NuiSkeletonTrackingEnable(mNextSkeletonEvent, 0); 
        }
    }

    if (NULL == mNuiSensor || FAILED(hr))
    {
        mState = NUI_MANAGER_NO_READY_SENSOR;
        return;
    }
	
	// Start the Nui processing thread
	mEvNuiProcessStop = CreateEvent( NULL, FALSE, FALSE, NULL );
	mThNuiProcess = CreateThread( NULL, 0, nuiProcessThread, this, 0, NULL );

	mState = NUI_MANAGER_OK;
}

void NuiManager_KinectSDK::uninstall()
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

	if (mNuiSensor) { mNuiSensor->NuiShutdown(); }

	if (mNextSkeletonEvent && (mNextSkeletonEvent != INVALID_HANDLE_VALUE))
	{
		CloseHandle(mNextSkeletonEvent);
		mNextSkeletonEvent = NULL;
	}

	if (mNuiSensor)
	{
		mNuiSensor->Release();
		mNuiSensor = NULL;
	}
}

/*----------------------------------------------------------------------------
| Thread to dispatch
----------------------------------------------------------------------------*/
DWORD WINAPI NuiManager_KinectSDK::nuiProcessThread(LPVOID pParam)
{
	NuiManager_KinectSDK *mgr = (NuiManager_KinectSDK *) pParam;
	return mgr->nuiProcessThread();
}

/*----------------------------------------------------------------------------
| Thread to handle Kinect processing
----------------------------------------------------------------------------*/
DWORD WINAPI NuiManager_KinectSDK::nuiProcessThread()
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
			++framesTotal;
			trackSkeleton();
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

bool NuiManager_KinectSDK::trackSkeleton(void)
{
	NUI_SKELETON_FRAME skeletonFrame = {0};
	bool foundSkeleton = false;

	if(SUCCEEDED(mNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame)))
	{
		for ( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )
		{
			if( skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
			{
				mPlayerIndex = i;
				foundSkeleton = true;
			}
		}
	}

	// no skeletons!
	if( !foundSkeleton ) { return false; }

	// smooth out the skeleton data
	HRESULT hr = mNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);
	if ( FAILED(hr) ) { return false; }

	NUI_SKELETON_DATA & skeleton = skeletonFrame.SkeletonData[mPlayerIndex];
	NUI_SKELETON_BONE_ORIENTATION boneOrientations[NUI_SKELETON_POSITION_COUNT];
    NuiSkeletonCalculateBoneOrientations(&skeleton, boneOrientations);

	for (int i=0; i<NUI_SKELETON_POSITION_COUNT; i++)
	{
		mBoneOrientationsH[i].x = boneOrientations[i].hierarchicalRotation.rotationQuaternion.x;
		mBoneOrientationsH[i].y = boneOrientations[i].hierarchicalRotation.rotationQuaternion.y;
		mBoneOrientationsH[i].z = boneOrientations[i].hierarchicalRotation.rotationQuaternion.z;
		mBoneOrientationsH[i].w = boneOrientations[i].hierarchicalRotation.rotationQuaternion.w;
		mBoneOrientationsA[i].x = boneOrientations[i].absoluteRotation.rotationQuaternion.x;
		mBoneOrientationsA[i].y = boneOrientations[i].absoluteRotation.rotationQuaternion.y;
		mBoneOrientationsA[i].z = boneOrientations[i].absoluteRotation.rotationQuaternion.z;
		mBoneOrientationsA[i].w = boneOrientations[i].absoluteRotation.rotationQuaternion.w;
	}
	
	return true;
}

const Ogre::String& NuiManager_KinectSDK::getCorrespondingOgreBone(int index, bool mirror)
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
