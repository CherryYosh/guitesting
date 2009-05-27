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

#define __M_IsNumber(arg) \
	(arg.type() == typeid(unsigned int)	|| arg.type() == typeid(int)	|| \
	 arg.type() == typeid(unsigned short)	|| arg.type() == typeid(short)	|| \
	 arg.type() == typeid(unsigned long)	|| arg.type() == typeid(long)	|| \
	 arg.type() == typeid(float)		|| arg.type() == typeid(double) )

#define __M_IsString(arg) \
	(arg.type() == typeid(std::string))

class LUABase {
public:
	LUABase();
	virtual ~LUABase();

	void Init();

	static lua_State* GetLuaState();
	static bool CallScript(std::string, DebugLevel = __M_DebugLevel);
	static bool CallScript(std::string, std::string, DebugLevel = __M_DebugLevel);
	static bool CallScript(std::string, LuaArgList, DebugLevel = __M_DebugLevel);
private:
};

#endif
