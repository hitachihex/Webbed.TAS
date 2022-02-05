# Webbed.TAS
TAS tools for the game Webbed

# NOTE: CURRENTLY ONLY WORKS ON GAME VERSION 1.3 
# NOTE: PLAYBACK ASSUMES 1920x1080 RESOLUTION FOR MOUSE PLAYBACK TO WORK PROPERLY. 
# IT HARDCODES THE CENTER POINT FOR X AND Y IN THE FUNCTIONS window_mouse_get_y_Hook AND window_mouse_get_x_Hook in Hooks.cpp WHEN THERE IS NO MOUSE INPUT.
# YOU CAN CHANGE IT IF YOU WISH. LINE 332 AND LINE 360. VALUES 540 AND 960. FOR LINE 332 IT IS Y RESOLUTION / 2 , AND FOR LINE 360 IT IS X RESOLUTION / 2

# Note: place Webbed.PEPatch.exe in same directory as game executable (webbed.exe)
To use the tool, run Webbed.PEPatch.exe (before running the game, make a backup of the webbed.exe file) then run Webbed.Injector after running the game executable, make sure Webbed.TAS.dll is in the same directory as the injector executable.
If the game is ran as administrator, you will need to run the injector as administrator, otherwise it doesn't matter what privilege it is ran as.

Default hotkeys are:

F1 - To toggle pause\unpause

F2 - Lock cursor ingame so you can't interfere with playback on accident, toggle this before doing playback.

F3 - Toggle playback, read from Webbed.rec in the directory where the game executable is located.

Numpad + - Increase game speed

Numpad - - Decrease game speed

Numpad / - Set game speed to default (normal speed)

[ - To step one frame (this also reloads the input file so you can make changes to the inputs while framestepping.)<br/>
Note: It won't understep (e.g go backwards and redo) you can only edit a current input or inputs currently not being
      played back.


# Note: You may use multiple input files. Create a folder called Includes in the directory where webbed.exe is. Name them anything with an extension of .rec
# But note that multi-level applies from the main input file only, so included input files may not read from another file.

--------------------------------------------------------


#NOTE: 
Commands read from input file are in the format of:
   frames, Action
   
Commnds are case-insensitive.
   
Accepted commands are: <br />
    Runto - to be placed under a line you want to fast forward playback to, then the game will pause (note, no frames for this command)<br />
    Walkto - to be placed under a line you want to normal speed playback to, then the game will pause (note, no frames for this command)<br />
    LeftP  - single frame press left <br />
    RightP - single frame press right <br />
    UpP - single frame press up <br />
    DownP - single frame press down <br />
    Dance <br />
    Left <br />
    Right <br />
    Up <br />
    Down <br />
    Jump <br />
    Grapple <br />
    Weave <br />
    Journal <br />
    Esc <br />
    Mouse - set mouse pos e.g Mouse, xpos, ypos -- values are delta and based on window resolution. right now, it assumes 1920 x 1080 for correct playback... <br />
    Laser <br />
    Pew (alias for laser) <br />
    Slow  - set this on inputs you don't want runto to fast forward during. Can be useful to combat desynchs via finding offending frames that work poorly in runto. <br />
    Read - accepts a sub command for the input file to read from. e.g Read, File.rec. It assumes they are in the Includes folder. <br />
