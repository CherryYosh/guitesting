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
	return RESIZE_ALL
    elseif str == "vertical" then
	return RESIZE_VERTICAL
    elseif str == "horizontal" then
	return RESIZE_HORIZONTAL
    elseif str == "none" then
	return RESIZE_NONE
    end
end