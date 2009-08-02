#ifndef LUABASE_H
#define LUABASE_H

#include <lua.hpp>

#include <vector>
#include <string>
#include <boost/any.hpp>

enum DebugLevel {
	None, Traceback, Interactive
};

#ifdef _DEBUG_		
#define __M_DebugLevel 	Traceback
#else			
#define __M_DebugLevel	None
#endif			

typedef std::vector<boost::any> LuaArgList;

class LUABase {
public:
	LUABase();
	virtual ~LUABase();

	static void Init();

	static lua_State* GetLuaState();
	static bool CallScript(std::string, DebugLevel = __M_DebugLevel);
	static bool CallScript(std::string, std::string, DebugLevel = __M_DebugLevel);
	static bool CallScript(std::string, LuaArgList, DebugLevel = __M_DebugLevel);
private:
    static lua_State* L;
    static bool initialized;
};

#endif
