%module test
%{
#include "thememgr.h"

struct WidgetData{
        std::string name;
        float x,y;
        float width,height;
};
%}

%include <std_string.i> 

%include "thememgr.h"

struct WidgetData{
        std::string name;
        float x,y;
        float width,height;
};
