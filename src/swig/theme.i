%module theme
%{
#include "../theme.h"

class Event;

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

        void AddEvent(std::string name, Event* event){
            Callbacks.insert(std::pair<std::string, Event*>(name, event));
        }

	WidgetData* Data;
	std::multimap<std::string, Event*> Callbacks;
        LayerT layer;
	float x, y, z;
};

struct WindowData {
        void AddEvent(std::string name, Event* event){
            Callbacks.insert(std::pair<std::string, Event*>(name, event));
        }

	std::vector<ChildData*> Children;
	std::multimap<std::string, Event*> Callbacks;
};

%}

%include <std_string.i>

%include "../theme.h"

enum LayerT { BOTTOM_LAYER, DEFAULT_LAYER, TOP_LAYER };

struct WidgetData{
        std::string name;
        float x,y;
        float width,height;
};

struct ChildData {
    	void SetLayer(std::string l);
        void AddEvent(std::string name, Event* event);

	WidgetData* Data;
	std::multimap<std::string, Event*> Callbacks;
        LayerT layer;
	float x, y, z;
};

struct WindowData {
        void AddEvent(std::string name, Event* event);

	std::vector<ChildData*> Children;
	std::multimap<std::string, Event*> Callbacks;
};
