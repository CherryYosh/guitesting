#include "slider.h"

//NOTE:Felt lazy, so a lot of this code is taken from TWL, a java gui

Slider::Slider(std::string t, Window* p, Control* c, LayerT l, float x, float y) : Control(t, p, c, l, x, y) {
	Thumb = new Control("slider-thumb", p, this, TOP_LAYER);

	SetRange(0, 100);
	UpdateStep();

	//Still must be put in to allow for rendering
	Children.push_back(Thumb);
}

Slider::~Slider() { }

bool Slider::OnMouseMotion(float nx, float ny, unsigned short button) {
	if (button == 1) {
		SetValueByPosition(nx);
		SetThumbPosition();
		return true;
	}

	return false;
}

void Slider::SetValueByPosition(float pos) {
	float delta = pos - x;
	float v = delta / step;
	SetValue(v);
}

void Slider::SetValue(float v) {
	v = Range(v);

	if (abs(value - v) > 0.001) { //epsilon
		value = v;
		SetThumbPosition();

		if (Callback != NULL)
			Callback(value);
	}
}

void Slider::SetThumbPosition() {
	float newX = x + (value * step);
	Thumb->SetPosition(newX, y);
}

void Slider::Scroll(float v) {
	value = Range(value + v);
}

float Slider::Range(float v) {
	if (v > maxValue) {
		return maxValue;
	} else if (v < minValue) {
		return minValue;
	}

	return v;
}

void Slider::SetRange(float min, float max) {
	if (min > max) {
		throw 5;
	}

	minValue = min;
	maxValue = max;
	value = Range(value);
	SetThumbPosition();
}

float Slider::GetStep() {
	return step;
}

void Slider::UpdateStep() {
	float delta = maxValue - minValue;
	step = (Width - Thumb->GetWidth()) / delta;
}

void Slider::SetCallback(boost::function<void(float) > callback) {
	Callback = callback;
}