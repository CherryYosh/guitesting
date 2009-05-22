%module test
%{
#include "thememgr.h"

enum LayerT { BOTTOM_LAYER, DEFAULT_LAYER, TOP_LAYER };

struct WidgetData{
        std::string name;
        float x,y;
        float width,height;
};

struct ChildData {
    	void SetLayer(std::string l){
		if(l == "top")
			layer = TOP_LAYER;
		else if(l == "default")
			layer = DEFAULT_LAYER;
		else if(l == "bottom")
			layer = BOTTOM_LAYER;
	}

	WidgetData* Data;
	std::string Callback;
        LayerT layer;
	float x, y, z;
};
%}

%include <std_string.i> 

%include "thememgr.h"

enum LayerT { BOTTOM_LAYER, DEFAULT_LAYER, TOP_LAYER };

struct WidgetData{
        std::string name;
        float x,y;
        float width,height;
};

struct ChildData {
    	void SetLayer(std::string l);
	WidgetData* Data;
	std::string Callback;
        LayerT layer;
	float x, y, z;
};
