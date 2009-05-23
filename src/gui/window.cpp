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
/* The window class
 * 	Binds several Controls together, if one moves they all move
 *
 * TODO:
 * 	Fix close so that the window will stay in memory for X seconds before being handled by GC
 *
 * James Brandon Stevenson
 *
 */

#include <GL/glew.h>

#include "window.h"

#include "controls.h"
#include "../gui.h"
#include "../camera.h"
#include <SDL/SDL.h>

struct AnimationType {
	unsigned int Type;
	unsigned int Interpolation;
	unsigned int EndTicks;
	unsigned int StartTicks;
	unsigned int LastTicks;
	unsigned int Duration; //end - start
	Control* Object;
	nv::vec4<float> data;
};

Window::Window(GUI* p, Renderer* r) : Control("", NULL) {
	ReciveInput = false;
	AnimationOrigin = nv::vec3<float>(0.0);
	Rotation.make_identity();

	gui = p;
	renderer = r;
}

Window::~Window() {
	gui->CloseWindow(this);
	MouseOverChild = NULL;
}

void Window::AddChild(Control *child) {
	//need to check if we are adding something that will make it wider or hight
	//NOTE:we assume the child's position is a offset..
	int delta;
	if ((delta = (child->GetX() + child->GetWidth()) - GetWidth()) > 0) {
		SetWidth(GetWidth() + delta);
	}
	if ((delta = (child->GetY() + child->GetHeight()) - GetHeight()) > 0) {
		SetHeight(GetHeight() + delta);
	}

	printf("width %f height %f\n", GetWidth(), GetHeight());

	size_t size = Children.size();
	for (unsigned int i = 0; i < size; i++) {
		if (Children[i]->HitTest(child->GetX(), child->GetY())) {
			Children[i]->AddChild(child);
			return;
		}
	}

	Children.push_back(child);
}

void Window::Move(float xChange, float yChange) {
	//allows for correct movement while rotated
	nv::vec4<float> change = nv::vec4<float>(xChange, yChange, 0.0, 1.0);
	change = inverse(Rotation) * change;

	x += change.x;
	y += change.y;

	size_t size = Children.size();
	for (unsigned int i = 0; i < size; i++) {
		Children[i]->Move(change.x, change.y);
	}
}

void Window::Close() {
	delete this;
}

void Window::UpdateControl(Control* control) {
	if (control == NULL)
		return;

	renderer->Update(control, RENDERER_REFRESH);
}

bool Window::IsRoot() {
	return true;
}

/**
 * Returns the total size of the control, in this case the same as TotalChildren
 */
unsigned int Window::Size() {
	return TotalChildren();
}

/** \brief Preforms a hit test on the window
 *
 * The functions finds the mouse coords in object space, thus working with 3d rotation,
 * and figures out if they are inside the window.
 *
 * @param mx the mouse's x coordinate
 * @param my the mouse's y coordinate
 * @param p  the projection matrix's array
 * @returns true if the hit test was succful
 * @throws nothing
 */
bool Window::HitTest(float mx, float my) {
	Unproject(mx, my, &mx, &my);

	if (mx > x && my > y &&
		mx < (x + Width) && my < (y + Height)) {

		if (MouseOverChild != NULL) {
			if (MouseOverChild->HitTest(mx, my)) {
				return true;
			} else {
				MouseOverChild->OnMouseLeave();
			}
		}

		//we iterate past the controls backwards so that we will get the top itmes
		//FILO order
		unsigned int i = Children.size();
		while (0 < i) {
			i--;

			if (Children[i] != MouseOverChild && Children[i]->HitTest(mx, my)) {
				MouseOverChild = Children[i];
				MouseOverChild->OnMouseEnter();
				return true;
			}
		}

		MouseOverChild = NULL;
		return true;
	}

	if (MouseOverChild != NULL) {
		MouseOverChild->OnMouseLeave();
		MouseOverChild = NULL;
	}

	return false;
}

void Window::OnKeyPress(unsigned short key) {
	if (ActiveChild != NULL)
		ActiveChild->OnKeyPress(key);
}

void Window::OnMousePress(unsigned short button, int mx, int my) {
	if (MouseOverChild != NULL) {
		ActiveChild = MouseOverChild;
		ActiveChild->OnMousePress(button, mx, my);
	}
}

bool Window::OnMouseClick(unsigned short num, bool final) {
	if (ActiveChild != NULL) {
		return ActiveChild->OnMouseClick(num, final);
	}

	return false;
}

/**
 * Creates a new animaion using a single float pushing it to the Animations list
 * @param type a enum for the tpye of animation to use
 * @param value	the values to use for the animation
 * @param start how long to way untill you wish for the animation to start
 * @param duration How long (in miliseconds) you wish the animation to take
 * @param interpolation (optional) the interpolation type you wish to use.
 * @param ptr (optional) the pointer of the object affected by the animtion, currently only supported for color
 */
void Window::Animate(AnimationT type, float value, unsigned int start, unsigned int duration, Interpolation interpolation, Control* ptr) {
	Animate(type, nv::vec4<float>(value, 0, 0, 0), start, duration, interpolation, ptr);
}

/**
 * Creates a new animaion using a vec2 pushing it to the Animations list
 * @param type a enum for the tpye of animation to use
 * @param value	the values to use for the animation
 * @param start how long to way untill you wish for the animation to start
 * @param duration How long (in miliseconds) you wish the animation to take
 * @param interpolation (optional) the interpolation type you wish to use.
 * @param ptr (optional) the pointer of the object affected by the animtion, currently only supported for color
 */
void Window::Animate(AnimationT type, nv::vec2<float> value, unsigned int start, unsigned int duration, Interpolation interpolation, Control* ptr) {
	Animate(type, nv::vec4<float>(value.x, value.y, 0, 0), start, duration, interpolation, ptr);
}

/**
 * Creates a new animaion using a vec3 pushing it to the Animations list
 * @param type a enum for the tpye of animation to use
 * @param value	the values to use for the animation
 * @param start how long to way untill you wish for the animation to start
 * @param duration How long (in miliseconds) you wish the animation to take
 * @param interpolation (optional) the interpolation type you wish to use.
 * @param ptr (optional) the pointer of the object affected by the animtion, currently only supported for color
 */
void Window::Animate(AnimationT type, nv::vec3<float> value, unsigned int start, unsigned int duration, Interpolation interpolation, Control* ptr) {
	if (type == ORIGIN) {
		AnimationOrigin = value;
		return;
	}

	Animate(type, nv::vec4<float>(value.x, value.y, value.z, 0), start, duration, interpolation, ptr);
}

/**
 * Creates a new animaion pushing it to the Animations list
 * @param type a enum for the tpye of animation to use
 * @param value	the values to use for the animation
 * @param start how long to way untill you wish for the animation to start
 * @param duration How long (in miliseconds) you wish the animation to take
 * @param interpolation (optional) the interpolation type you wish to use.
 * @param ptr (optional) the pointer of the object affected by the animtion, currently only supported for color
 */
void Window::Animate(AnimationT type, nv::vec4<float> value, unsigned int start, unsigned int duration, Interpolation interpolation, Control* ptr) {
	unsigned int ticks = SDL_GetTicks();

	AnimationType ani;
	ani.Type = type;
	ani.Interpolation = interpolation;
	ani.Object = ptr;
	ani.Duration = duration;
	ani.StartTicks = ticks + start;
	ani.LastTicks = ticks + start;
	ani.EndTicks = ticks + start + duration;
	ani.data = value;

	Animations.push_back(ani);
}

void Window::StepAnimation() {
	if (Animations.size() == 0)
		return;

	unsigned int ticks = SDL_GetTicks();
	unsigned int step = 0;
	nv::vec4<float> data;
	float scale;
	bool del;

	std::list<AnimationType>::iterator it = Animations.begin();
	while (it != Animations.end()) {
		del = false;

		if (ticks > it->StartTicks) {
			if (ticks > it->EndTicks) {
				step = it->EndTicks - it->LastTicks;
				del = true;
			} else {
				step = ticks - it->LastTicks;
				it->LastTicks = ticks;
			}

			scale = float(step) / float(it->Duration);
			data = it->data * scale;

			//Translation
			switch ((it->Type & TRANSLATEXYZ)) {
			case 0: break;
			case TRANSLATEX: Move(data.x, 0);
				break;
			case TRANSLATEY: Move(0, data.x);
				break;
			case TRANSLATEXY: Move(data.x, data.y);
				break;
			default: break;
			}

			//rotation
			if ((it->Type & ROTATEXYZ)) {
				if ((it->Type & ROTATEX)) {
					Rotation.rotate(data.x, 1.0, 0.0, 0.0);
				} else if ((it->Type & ROTATEY)) {
					Rotation.rotate(data.x, 0.0, 1.0, 0.0);
				} else if ((it->Type & ROTATEZ)) {
					Rotation.rotate(data.x, 0.0, 0.0, 1.0);
				}

				UpdateControl(this);
			} else if ((it->Type & ROTATEORGXYZ)) {
				if ((it->Type & ROTATEORGX)) {
					Rotation.rotateOrigin(data.x, 1.0, 0.0, 0.0, AnimationOrigin);
				} else if ((it->Type & ROTATEORGY)) {
					Rotation.rotateOrigin(data.x, 0.0, 1.0, 0.0, AnimationOrigin);
				} else if ((it->Type & ROTATEORGZ)) {
					Rotation.rotateOrigin(data.x, 0.0, 0.0, 1.0, AnimationOrigin);
				}

				UpdateControl(this);
			} else if ((it->Type & ROTATESCREENXYZ)) {
				if ((it->Type & ROTATESCREENX)) {
					Rotation.rotateScreen(data.x, 1.0, 0.0, 0.0, AnimationOrigin);
				} else if ((it->Type & ROTATESCREENY)) {
					Rotation.rotateScreen(data.x, 0.0, 1.0, 0.0, AnimationOrigin);
				} else if ((it->Type & ROTATESCREENZ)) {
					Rotation.rotateScreen(data.x, 0.0, 0.0, 1.0, AnimationOrigin);
				}

				UpdateControl(this);
			}

			//color
			if ((it->Type & RGBACHANNEL)) {
				if (it->Object != NULL) {
					it->Object->AddColor(data);
					UpdateControl(it->Object);
				}
			}
		}

		if (del) {
			it = Animations.erase(it);
		} else {
			it++;
		}
	}
}

//TODO: Change the way animations are handeld to make this cleaner
//and faster

void Window::RemoveAnimation(Control* c) {
	std::list<AnimationType>::iterator it;
	for (it = Animations.begin(); it != Animations.end(); it++) {
		if (it->Object == c) {
			Animations.erase(it);
			return;
		}
	}
}

void Window::Unproject(float winx, float winy, float* ox, float* oy) {
	GLint* view = renderer->GetViewport();

	nv::vec4<float> in = nv::vec4<float>(((winx - view[0]) * 2.0) / view[2] - 1.0,
		-(((winy - view[1]) * 2.0) / view[3] - 1.0),
		(2 * (z * 0.001)) - 1,
		1.0);

	nv::matrix4<float> pm;
	pm = (*renderer->GetCamera()->GetOrtho()) * Rotation;

	nv::vec4<float> ret = inverse(pm) * in;

	if (ret.w == 0.0)
		return;

	*ox = ret.x / ret.w;
	*oy = ret.y / ret.w;
}

nv::matrix4<float>* Window::GetRotation() {
	return &Rotation;
}

float* Window::GetRotationfv() {
	return Rotation._array;
}