require "theme"
require "events"
require "color"
require "lxp"

local t = theme.Theme()
local curWidget = nil
local curChild = nul
local isInEvent = false;

--load the utils file for GetEventByName, cant use swig for this sadly
dofile("scripts/utils.lua")

local function import(name, args)
    main(args["file"]) --load the new file
end

local function textures(name, args)
    if not theme.Theme_SetImage("themes/" .. args["file"]) then
	if not theme.Theme_SetImage(args["file"]) then error("Could not find image " .. args["file"]) end
    end
end

local function texture(name, args)
    t : AddTextureData( args["name"], args["x"], args["y"], args["width"], args["height"]);
end

local function widget(name, args)
    curWidget = t : NewWidget(args["name"])
end

local function widgetEnd(name)
    curWidget = nil
end

local function child(name, args)
    if curWidget then
	if args["layer"] then
	    curChild = curWidget : NewChild(args["type"], args["x"], args["y"], ToLayer(args["layer"]))
	else
	    curChild = curWidget : NewChild(args["type"], args["x"], args["y"])
	end
    end
end

local function childEnd(name)
    curChild = nil
end

local function eventsStart(name, args)
    isInEvent = true
end

local function eventsEnd(name)
    isInEvent = false
end

local function dialog(name, args)
	if args["layer"] then
	    temp = curWidget : NewChild(args["type"], args["x"], args["y"], ToLayer(args["layer"]))
	else
	    temp = curWidget : NewChild(args["type"], args["x"], args["y"])
	end

	curChild : ToEditbox() : SetDialog( temp : ToEditbox() )
end

local function events(name, args)
	if curChild then
	    control = curChild
	elseif curWidget then
	    control = curWidget
	else error() end

	print("Events, TODO")
end

local callbacks = {
	StartElement = function (parser, name, attribs)
		if name == "textures" then
			textures(name, attribs)
		elseif name == "texture" then
			texture(name, attribs)
		elseif name == "widget" then
			widget(name, attribs)
		elseif name == "child" then
			child(name, attribs)
		elseif name == "import" then
			import(name, attribs)
		elseif name == "events" then
			eventsStart(name, attribs)
		elseif name == "dialog" then
			dialog(name, attribs)
		elseif isInEvent then
			events(name, attribs)
		end
	end,

	EndElement = function(parser, name)
		if name == "widget" then
			widgetEnd(name)
		elseif name == "child" then
			childEnd(name)
		elseif name == "effects" then
			eventsEnd(name)
		end
	end
}

-- the main function, used to allow argument passing
function main(filepath)
    local parser = lxp.new(callbacks)
    local file = io.open(filepath, "r")

    for line in file:lines() do
	parser:parse(line)
    end

    file:close()
end
