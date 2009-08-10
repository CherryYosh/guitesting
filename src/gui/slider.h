#ifndef SLIDER_H
#define	SLIDER_H

#include "Widget.h"

class Slider : public Widget {
public:
	Slider(Window* = NULL, Widget* = NULL, LayerT = DEFAULT_LAYER, float = 0, float = 0);
	virtual ~Slider();

	bool OnMouseMotion(float, float, unsigned short);
	void SetValueByPosition(float);
	void SetValue(float);
	void SetThumbPosition();

	void Scroll(float);

	float Range(float);
	void SetRange(float,float);

	float GetStep();
	void UpdateStep();

	void SetCallback(boost::function<void(float)>);

private:
	float step;
	float value;
	float minValue, maxValue;

	Widget* Thumb;

	boost::function<void(float)> Callback;
};

#endif	/* SLIDER_H */