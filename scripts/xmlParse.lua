require "theme"
require "events"
require "color"
require "lxp"

local Theme = theme.Theme()
local curWidget = nil
local curChild = nul
local isInEffects = false;

--load the utils file for GetEventByName, cant use swig for this sadly
dofile("scripts/utils.lua")

local function import(name, args)
	main(args["file"]) --load the new file
end

local function textures(name, args)
    theme.Theme_SetImage("themes/" .. args["file"])
end

local function texture(name, args)
    local widget = Theme : NewWidgetData(args["name"], nil)	

    for i=1,#args do
	if args[i] ~= "name" then
	    widget[args[i]] = tonumber(args[args[i]])
	end
    end
end

local function widget(name, args)
    curWidget = Theme : NewWindowData(args["name"], nil)
end

local function widgetEnd(name)
	curWidget = nil
end

local function child(name, args)
	curChild = Theme : PushWidget( curWidget, args["type"] )

	for i=1,#args do
		if args[i] ~= "type" then
			if args[i] == "layer" then
				curChild : SetLayer(args["layer"])
			else
				curChild[args[i]] = args[args[i]]
			end
		end
	end
end

local function childEnd(name)
	curChild = nil
end

local function effectsStart(name, args)
	isInEffects = true
end

local function effectsEnd(name)
	isInEffects = false
end

local function effects(name, args)
	if not isInEffects then
		return
	end

	local event = GetEventByName(args["class"])

	for i=1,#args do
		if args[i] ~= "class" then
			if args[i] == "color" then
				event : SetColor(args[args[i]])
			elseif args[i] == "translation" then
				event : Translation(string.match(args[args[i]], "([^,]*),(.*)"))
			else
				event[args[i]] = args[args[i]]
			end
		end
	end

	curChild : AddEvent(name, event)
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
		elseif name == "effects" then
			effectsStart(name, attribs)
		elseif isInEffects then
			effects(name, attribs)
		end
	end,

	EndElement = function(parser, name)
		if name == "widget" then
			widgetEnd(name)
		elseif name == "child" then
			childEnd(name)
		elseif name == "effects" then
			effectsEnd(name)
		end
	end
}

-- the main function, used to allow argument passing
function main(filepath)
    print("-- Loading File " .. filepath .. " --")

    local parser = lxp.new(callbacks)
    local file = io.open(filepath, "r")

    for line in file:lines() do
	parser:parse(line)
    end

    file:close()

    print("-- Finished loading file " .. filepath .. " --")
end
