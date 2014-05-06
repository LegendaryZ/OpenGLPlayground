#include "LuaScript.h"
#include "../../Platform/ThreadManager.h"
#include <functional>


/**
 * Create a lua script with the file containing the actual script
 *
 * @param filename		The name of the file to load in
 *
 * @param luaScript		The loaded script or NULL if the script fails to init
 **/
LuaScript* LuaScript::create(const std::string& filename) {
   LuaScript* luaScript = new LuaScript();
   if(luaScript->init(filename))
	   return luaScript;

   //Failure case
   return NULL;
}

LuaScript::LuaScript()
{
	status = Uninitialized;
}

LuaScript::~LuaScript() {
	if(L) lua_close(L);
}

/**
 * Load in a lua script into the object
 *
 * @param filename		the name of the file to load in
 *
 * @param success or failure
 **/
bool LuaScript::init(const std::string& filename)
{
	L = luaL_newstate();
    if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
        std::cout<<"Error: failed to load ("<<filename<<")"<<std::endl;
		L = 0;
		return false;
    }

    if(L) luaL_openlibs(L);

	this->filename = filename;
	status = Running;
	return true;
}

/**
 * Print an error message to the console
 *
 * @param variableName	the variable attempted to be accessed
 * @param reason		the reason for the failure
 **/
void LuaScript::printError(const std::string& variableName, const std::string& reason) {
	std::cout<<"Error: can't get ["<<variableName<<"]. "<<reason<<std::endl;
}

/**
 * Register a c style function for use within the Lua Script
 *
 * @param f		the function to be registered
 * @param luaFuncName	The name of the function lua will know it as
 **/ 
void LuaScript::registercfunc(lua_CFunction f, std::string luaFuncName)
{
	lua_pushcfunction(L, f);
	lua_setglobal(L, luaFuncName.c_str());
}

/**
 * Call a function from Lua with no return. First arg
 * must be the function to be called
 *
 * Format for argdesc:
 * i = int
 * d = double
 * f = float
 * b = bool
 * s = char*
 **/
void LuaScript::lua_voidfunc(char* argdesc, ...)
{
	va_list arg_list;
	int numarg_list = strlen(argdesc);
	if(numarg_list < 1 || argdesc[0] != 's')
		return;

	va_start(arg_list, argdesc);

	char* funcname = va_arg(arg_list, char*);
	lua_getglobal(L, funcname);
 
	for(int i = 1; i < numarg_list; i++)
	{
		switch(argdesc[i])
		{
		case 'i':
			lua_pushnumber(L, va_arg(arg_list, int));
			break;
		case 'd':
			lua_pushnumber(L, va_arg(arg_list, double));
			break;
		case 'f':
			lua_pushnumber(L, va_arg(arg_list, float));
			break;
		case 'b':
			lua_pushboolean(L, va_arg(arg_list, bool));
			break;
		case 's':
			lua_pushstring(L, va_arg(arg_list, char*));
			break;
		}
	}
	va_end(arg_list);
 
	if(lua_pcall(L, numarg_list-1, 0, 0) != 0)
	   printf( "error running function `%s': %s", funcname, lua_tostring(L, -1));

 // lua_pop(L,1);
  clean();
}

/**
 * Call a function from Lua that returns a double. First arg
 * must be the function to be called
 *
 * Format for argdesc:
 * i = int
 * d = double
 * f = float
 * b = bool
 * s = char*
 **/
double LuaScript::lua_doublefunc(char* argdesc, ...)
{
	va_list arg_list;
	int numarg_list = strlen(argdesc);
	if(numarg_list < 1 || argdesc[0] != 's')
		return 0;

	va_start(arg_list, argdesc);

	char* funcname = va_arg(arg_list, char*);
	lua_getglobal(L, funcname);
 
	for(int i = 1; i < numarg_list; i++)
	{
		switch(argdesc[i])
		{
		case 'i':
			lua_pushnumber(L, va_arg(arg_list, int));
			break;
		case 'd':
			lua_pushnumber(L, va_arg(arg_list, double));
			break;
		case 'f':
			lua_pushnumber(L, va_arg(arg_list, float));
			break;
		case 'b':
			lua_pushboolean(L, va_arg(arg_list, bool));
			break;
		case 's':
			lua_pushstring(L, va_arg(arg_list, char*));
			break;
		}
	}
	va_end(arg_list);
 
	if(lua_pcall(L, numarg_list-1, 1, 0) != 0)
	{
	   printf( "error running function `%s': %s", funcname, lua_tostring(L, -1));
	   lua_pop(L, 1);
	   return 0;
	}

	if(!lua_isnumber(L, -1))
	{
		printf("function '%s' does not return a double", funcname);
		lua_pop(L, 1);
		return 0;
	}

	double result = lua_tonumber(L, -1);
	lua_pop(L,1);

	return result;
}

/**
 * Call a function from Lua that returns an int. First arg
 * must be the function to be called
 *
 * Format for argdesc:
 * i = int
 * d = double
 * f = float
 * b = bool
 * s = char*
 **/
int LuaScript::lua_intfunc(char* argdesc, ...)
{
	va_list arg_list;
	int numarg_list = strlen(argdesc);
	if(numarg_list < 1 || argdesc[0] != 's')
		return 0;

	va_start(arg_list, argdesc);

	char* funcname = va_arg(arg_list, char*);
	lua_getglobal(L, funcname);
 
	for(int i = 1; i < numarg_list; i++)
	{
		switch(argdesc[i])
		{
		case 'i':
			lua_pushnumber(L, va_arg(arg_list, int));
			break;
		case 'd':
			lua_pushnumber(L, va_arg(arg_list, double));
			break;
		case 'f':
			lua_pushnumber(L, va_arg(arg_list, float));
			break;
		case 'b':
			lua_pushboolean(L, va_arg(arg_list, bool));
			break;
		case 's':
			lua_pushstring(L, va_arg(arg_list, char*));
			break;
		}
	}
	va_end(arg_list);
 
	if(lua_pcall(L, numarg_list-1, 1, 0) != 0)
	{
	   printf( "error running function `%s': %s", funcname, lua_tostring(L, -1));
	   lua_pop(L, 1);
	   return 0;
	}

	if(!lua_isnumber(L, -1))
	{
		printf("function '%s' does not return a int", funcname);
		lua_pop(L, 1);
		return 0;
	}

	int result = (int)lua_tonumber(L, -1);
	lua_pop(L,1);

	return result;
}

/**
 * Call a function from Lua that returns a string. First arg
 * must be the function to be called
 *
 * Format for argdesc:
 * i = int
 * d = double
 * f = float
 * b = bool
 * s = char*
 **/
std::string LuaScript::lua_stringfunc(char* argdesc, ...)
{
	va_list arg_list;
	int numarg_list = strlen(argdesc);
	if(numarg_list < 1 || argdesc[0] != 's')
		return 0;

	va_start(arg_list, argdesc);

	char* funcname = va_arg(arg_list, char*);
	lua_getglobal(L, funcname);
 
	for(int i = 1; i < numarg_list; i++)
	{
		switch(argdesc[i])
		{
		case 'i':
			lua_pushnumber(L, va_arg(arg_list, int));
			break;
		case 'd':
			lua_pushnumber(L, va_arg(arg_list, double));
			break;
		case 'f':
			lua_pushnumber(L, va_arg(arg_list, float));
			break;
		case 'b':
			lua_pushboolean(L, va_arg(arg_list, bool));
			break;
		case 's':
			lua_pushstring(L, va_arg(arg_list, char*));
			break;
		}
	}
	va_end(arg_list);
 
	if(lua_pcall(L, numarg_list-1, 1, 0) != 0)
	{
	   printf( "error running function `%s': %s", funcname, lua_tostring(L, -1));
	   lua_pop(L, 1);
	   return 0;
	}

	if(!lua_isstring(L, -1))
	{
		printf("function '%s' does not return a double", funcname);
		lua_pop(L, 1);
		return 0;
	}

	std::string result = lua_tostring(L, -1);
	lua_pop(L,1);

	return result;
}

/**
 * Call a function from Lua that returns a bool. First arg
 * must be the function to be called
 *
 * Format for argdesc:
 * i = int
 * d = double
 * f = float
 * b = bool
 * s = char*
 **/
bool LuaScript::lua_boolfunc(char* argdesc, ...)
{
	va_list arg_list;
	int numarg_list = strlen(argdesc);
	if(numarg_list < 1 || argdesc[0] != 's')
		return 0;

	va_start(arg_list, argdesc);

	char* funcname = va_arg(arg_list, char*);
	lua_getglobal(L, funcname);
 
	for(int i = 1; i < numarg_list; i++)
	{
		switch(argdesc[i])
		{
		case 'i':
			lua_pushnumber(L, va_arg(arg_list, int));
			break;
		case 'd':
			lua_pushnumber(L, va_arg(arg_list, double));
			break;
		case 'f':
			lua_pushnumber(L, va_arg(arg_list, float));
			break;
		case 'b':
			lua_pushboolean(L, va_arg(arg_list, bool));
			break;
		case 's':
			lua_pushstring(L, va_arg(arg_list, char*));
			break;
		}
	}
	va_end(arg_list);
 
	if(lua_pcall(L, numarg_list-1, 1, 0) != 0)
	{
	   printf( "error running function `%s': %s", funcname, lua_tostring(L, -1));
	   lua_pop(L, 1);
	   return 0;
	}

	if(!lua_isboolean(L, -1))
	{
		printf("function '%s' does not return a double", funcname);
		lua_pop(L, 1);
		return 0;
	}

	bool result = lua_toboolean(L, -1);
	lua_pop(L,1);

	return result;
}

/**
 * Get all the keys of a table in the lua script
 *
 * @param name		the name of the table
 *
 * @return strings	the keys in the table
 **/
std::vector<std::string> LuaScript::getTableKeys(const std::string& name) {
    std::string code = 
        "function getKeys(name) "
        "s = \"\""
        "for k, v in pairs(_G[name]) do "
        "    s = s..k..\",\" "
        "    end "
        "return s "
        "end"; // function for getting table keys
    luaL_loadstring(L, 
        code.c_str()); // execute code
    lua_pcall(L,0,0,0);
    lua_getglobal(L, "getKeys"); // get function
    lua_pushstring(L, name.c_str());
    lua_pcall(L, 1 , 1, 0); // execute function
    std::string test = lua_tostring(L, -1);
    std::vector<std::string> strings;
    std::string temp = "";
    std::cout<<"TEMP:"<<test<<std::endl;
    for(unsigned int i = 0; i < test.size(); i++) {     
        if(test.at(i) != ',') {
            temp += test.at(i);
        } else {
            strings.push_back(temp);
            temp= "";
        }
    }
    clean();
    return strings;
}

void LuaScript::infiniteRun()
{
	auto f = [this](){
	lua_voidfunc("s", "init");
	while(status != Dead)
		lua_voidfunc("s", "update");
	};
	ThreadManager::getInstance()->asyncTask(f);
}