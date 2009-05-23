#ifndef LUABASE_H
#define LUABASE_H

#include <lua.hpp>

#include <string>

enum DebugLevel {
	None, Traceback, Interactive
};

#ifdef _DEBUG_		
#define __M_DebugLevel 	Traceback
#else			
#define __M_DebugLevel	None
#endif			

class LUABase {
public:
	LUABase();
	virtual ~LUABase();

	void Init();

	static lua_State* GetLuaState();
	static bool CallScript(std::string, DebugLevel = __M_DebugLevel);
	static bool CallScriptS(std::string, std::string, DebugLevel = __M_DebugLevel);
private:
};

#endif
