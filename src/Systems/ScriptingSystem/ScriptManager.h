#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <vector>
#include <string>
#include <map>
#include "LuaScript.h"

using namespace std;

/**
 * Script Manager is responsible for managing
 * all of the lua scripts. Tautology!
 **/
class ScriptManager
{
public:
	static ScriptManager* getInstance();
	~ScriptManager(void);

	bool loadScripts(const std::string& scriptDir);
	LuaScript* getScript(std::string& scriptName);
	bool unloadScript(std::string& scriptName);
	bool hasScript(std::string& scriptName);

	map<string, LuaScript*>& getScripts();
	void static runScript(LuaScript* script);
private:
	map<string, LuaScript*> scripts;

	ScriptManager(void);
	
};

#endif /*SCRIPTMANAGER_H*/

extern "C" {
  static inline int l_killScript(lua_State *L) {

	  const char *filename = NULL;
	  size_t filenameLength = 0; //Lua strings have an explicit length; they can contain null characters.

	  if(lua_gettop(L) != 1)
	  {
		lua_pushstring(L, "Must provide one parameter, the script name, to this Lua function.");
		lua_error(L);
	  }

	  filename = luaL_checklstring(L, 1, &filenameLength);

	  if(L == ScriptManager::getInstance()->getScript(std::string(filename))->getLuaState())
	  {
		  printf("\nKilled %s\n", filename);
		  ScriptManager::getInstance()->getScript(std::string(filename))->setStatus(LuaScript::Status::Dead);
	  }
	  else
		  printf("A script cannot kill other scripts");
	  return 0;
  }
}
