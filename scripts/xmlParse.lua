require "theme"
require "events"
require "lxp"

local Theme = theme.Theme()
local curWidget = nil
local e = events.Event()

local function import(name, args)
	main(args["file"])
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

local function child(name, args)
	local child = Theme : PushWidget( curWidget, args["type"] )

	for i=1,#args do
		if args[i] ~= "type" then
			if args[i] == "layer" then
				child : SetLayer(args["layer"])
			else
				child[args[i]] = args[args[i]]
			end
		end
	end
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
