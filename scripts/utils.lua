function GetEventByName(name)
	if name == "colorchange" then
		return ColorChangeEvent()
	elseif name == "move" then
		return MoveEvent()
	elseif name == "close" then
		return CloseEvent()
	elseif name == "rotation" then
		return RotationEvent()
	else
		return Event()
	end
end

function ToLayer(str)
	if str == "top" then
		return TOP_LAYER
	elseif str == "default" then
		return DEFAULT_LAYER
	elseif str == "bottom" then
		return BOTTOM_LAYER
	end
end

function ToOrientation(str)
    if str == "all" then
	return All
    elseif str == "vertical" then
	return Vertical
    elseif str == "horizontal" then
	return Horizontal
    elseif str == "none" then
	return DontResize
    end
end