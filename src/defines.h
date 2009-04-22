/*   This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	long with this program.  If not, see <http://www.gnu.org/licenses/>

 * 	Copyright 2008,2009 James Brandon Stevenson
 */
#ifndef DEFINES_H
#define DEFINES_H

//system names
#define SYSTEM_ENGINE 	0
#define SYSTEM_INPUT 	1
#define SYSTEM_DISPLAY 	2

//functions start at 1k so they dont mess with other defines

//global messages
#define QUIT 1000
#define TIMER_DONE 1001

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
