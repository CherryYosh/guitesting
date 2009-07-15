function GetEventByName(name)
	if name == "colorchange" then
		return events.ColorChangeEvent()
	elseif name == "move" then
		return events.MoveEvent()
	elseif name == "close" then
		return events.CloseEvent()
	elseif name == "rotation" then
		return events.RotationEvent()
	else
		return events.Event()
	end
end

function ToLayer(str)
	if str == "top" then
		return widgets.TOP_LAYER
	elseif str == "default" then
		return widgets.DEFAULT_LAYER
	elseif str == "bottom" then
		return widgets.BOTTOM_LAYER
	end
end

function ToOrientation(str)
    if str == "all" then
	return widgets.All
    elseif str == "vertical" then
	return widgets.Vertical
    elseif str == "horizontal" then
	return widgets.Horizontal
    elseif str == "none" then
	return widgets.DontResize
    end
end