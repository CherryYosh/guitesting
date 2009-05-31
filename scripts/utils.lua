function GetEventByName(name)
	if name == "colorchange" then
		return events.ColorChangeEvent()
	elseif name == "move" then
		return events.MoveEvent()
	elseif name == "close" then
		return events.CloseEvent()
	else
		return events.Event()
	end
end