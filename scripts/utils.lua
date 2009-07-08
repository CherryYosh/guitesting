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