%module widgets
%{

#include "../gui/control.h"
#include "../gui/window.h"
#include "../gui/button.h"
#include "../gui/rule.h"
#include "../gui/label.h"
#include "../gui/editbox.h"
#include "../gui/checkbox.h"
#include "../gui/slider.h"
#include "../gui/edge.h"

%}

%include <std_string.i>
%include "../gui/control.h"
%include "../gui/window.h"
%include "../gui/button.h"
%include "../gui/rule.h"
%include "../gui/label.h"
%include "../gui/editbox.h"
%include "../gui/checkbox.h"
%include "../gui/slider.h"
%include "../gui/edge.h"

%extend Control{
    Label* ToLabel(){
        return (Label*)$self;
    }

    Editbox* ToEditbox(){
        return (Editbox*)$self;
    }
};