#ifndef LUABASE_H
#define LUABASE_H

#include <lua.hpp>

#include <string>

enum DebugLevel{None, Traceback, Interactive};

class LUABase{
public:
	LUABase();
	virtual ~LUABase();

	void Init();

	static lua_State* GetLuaState();
	static bool CallScript(std::string, DebugLevel =
#ifdef _DEBUG_
	Traceback
#else
	None
#endif
	);
private:
};

#endif
