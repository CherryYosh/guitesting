function GetEventByName(name)
	if name == "colorchange" then
		return events.ColorChangeEvent()
	elseif name == "move" then
		return events.MoveEvent()
	else
		return events.Event()
	end
end