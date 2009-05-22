#ifndef SLIDER_H
#define	SLIDER_H

#include "control.h"

class Slider : public Control {
public:
	Slider(std::string, Window*, Control* = NULL, LayerT = DEFAULT_LAYER, float = 0, float = 0);
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

	Control* Thumb;

	boost::function<void(float)> Callback;
};

#endif	/* SLIDER_H */