%module events
%{
    #include "../events/event.h"
    #include "../events/gui/colorchange.h"

    ColorChangeEvent* NewEventByName(std::string e){
        //if(e == "event" )
         //   return new Event();
        //else if (e == "changecolor")
            return new ColorChangeEvent();
    }
%}

%include <std_string.i>
%include "../events/event.h"
%include "../events/gui/colorchange.h"

ColorChangeEvent* NewEventByName(std::string);