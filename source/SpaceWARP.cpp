#include "game.h"

// updates per second
#define UPS 30

// throttle frame time at 10 fps (i.e. the game will slow down rather
// than having very low frame rate)
#define MAX_UPDATES 6

int GetUpdateFrame()
{
    return (int)(s3eTimerGetMs() / (1000/UPS));
}


int32 device_pause(void* system, void* user)
{
	if(newgame->get_page() == 1)
	{
		newgame->set_page();
	}

	return 0;
}

// Main entry point for the application
int main()
{
    //Initialise graphics system(s)
    Iw2DInit();

	CIw2DImage *ss = Iw2DCreateImage("Splash/splashscreen.png");
	Iw2DSurfaceClear(0xff00ff00);
	Iw2DSetColour(0xffffffff);
	Iw2DDrawImage(ss,CIwFVec2(0,0),CIwFVec2((float)Iw2DGetSurfaceWidth(),(float)Iw2DGetSurfaceHeight()));
	Iw2DSurfaceShow();
	if(s3eDeviceGetInt(S3E_DEVICE_OS)==S3E_OS_ID_WP8)
	{
		Iw2DSurfaceShow();
	}
	

    getresource = new Resources();
	newgame = new gamePlay();
	s3eDeviceRegister(S3E_DEVICE_PAUSE,(s3eCallback)device_pause,0);

    int currentUpdate = GetUpdateFrame();
    int nextUpdate = currentUpdate;

    // to exit correctly, applications should poll for quit requests
    while(!s3eDeviceCheckQuitRequest())
    {

		// run logic at a fixed frame rate (defined by UPS)

        // block until the next frame (don't render unless at
        // least one update has occurred)
        while(!s3eDeviceCheckQuitRequest())
        {
            nextUpdate = GetUpdateFrame();
            if( nextUpdate != currentUpdate )
                break;
            s3eDeviceYield(0);
        }

        // execute update steps
        int frames = nextUpdate - currentUpdate;
        frames = MIN(MAX_UPDATES, frames);
        while(frames--)
        {
			newgame->Update();
			IwGetSoundManager()->Update();
        }
        currentUpdate = nextUpdate;

        // render the results
		newgame->Render();

        // if an application uses polling input the application
        // must call update once per frame
        s3ePointerUpdate();
        s3eKeyboardUpdate();

        // S3E applications should yield frequently
        s3eDeviceYield(0);
    }
	delete getresource;
	delete newgame;
	delete ss;
    Iw2DTerminate();
    
    // Return
    return 0;
}