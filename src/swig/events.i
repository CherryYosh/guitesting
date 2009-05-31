%module events
%{
    #include "../events/event.h"
    #include "../events/gui/guievent.h"
    #include "../events/gui/colorchange.h"
    #include "../events/gui/move.h"
%}

%include <std_string.i>
%include "../events/event.h"
%include "../events/gui/guievent.h"
%include "../events/gui/colorchange.h"
%include "../events/gui/move.h"