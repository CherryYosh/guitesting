#ifndef DEFINES_H
#define DEFINES_H

//system names
#define SYSTEM_ENGINE 	0
#define SYSTEM_INPUT 	1
#define SYSTEM_DISPLAY 	2

//functions start at 1k so they dont mess with other defines

//global messages
#define QUIT 1000

//display messages
#define MOUSE_PRESS 	1103
#define MOUSE_RELEASE 	1104
#define MOUSE_MOTION 	1105
#define WINDOW_RESIZE   1106
#define CAMERA_MOVE	1107
#define INPUT_KEYPRESS	1108

//input messages
#define INPUT_GET_MOUSE 1203
#define INPUT_CHANGE_PROFILE	1204
#endif
