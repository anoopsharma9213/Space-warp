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

// Main entry point for the application
int main()
{
    //Initialise graphics system(s)
    Iw2DInit();

    getresource = new Resources();
	newgame = new gamePlay();

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
            s3eDeviceYield(1);
        }

        // execute update steps
        int frames = nextUpdate - currentUpdate;
        frames = MIN(MAX_UPDATES, frames);
        while(frames--)
        {
			newgame->Update();
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
    Iw2DTerminate();
    
    // Return
    return 0;
}
