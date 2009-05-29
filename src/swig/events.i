%module events
%{
    #include "../events/event.h"
    #include "../events/gui/changecolor.h"

    Event* NewEventByName(std::string e){
        if(e == "event" )
            return new Event();
        else if (e == "changecolor")
            return new changecolor();
    }
%}

%include <std_string.i>
%include "../events/event.h"
%include "../events/gui/changecolor.h"

Event* NewEventByName(std::string);