#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <string>
#include <vector>
#include <iostream>
#include <lua.hpp>
#include <stdarg.h>

/**
 * Manages a single LuaScript
 **/
class LuaScript {
public:

	enum Status{
		Uninitialized,
		Running,
		Paused,
		Dead
	};

	static LuaScript* create(const std::string& filename);
    ~LuaScript();


    void printError(const std::string& variableName, const std::string& reason);
    
    std::vector<std::string> getTableKeys(const std::string& name);
    
	void registercfunc(lua_CFunction f, std::string luaFuncName);

	void lua_voidfunc(char* argdesc, ...);
	double lua_doublefunc(char* argdesc, ...);
	int lua_intfunc(char* argdesc, ...);
	std::string lua_stringfunc(char* argdesc, ...);
	bool lua_boolfunc(char* argdesc, ...);

	inline lua_State* getLuaState()
	{
		return L;
	}

	/**
	 * Clean the stack
	 **/
    inline void clean() {
      int n = lua_gettop(L);
      lua_pop(L, n);
    }

	/**
	 * Get a vector of ints from the lua script
	 *
	 * @param name	the name of the vector
	 *
	 * @return v	the values of the int vector
	 **/
	template<typename T>
	std::vector<T> getVector(const std::string& vectorName) {
		if(!L) {
			printError(vectorName, "Script is not loaded");
			return lua_getdefaultVector<T>();
		}

		lua_gettostack(vectorName.c_str());
		if(lua_isnil(L, -1)) { // array is not found
			printError(vectorName, "Vector is not found");
			return lua_getdefaultVector<T>();
		}
		lua_pushnil(L);
		vector<T> result = lua_getVector<T>(vectorName);
		clean();

		return result;
	}

	/**
	 * Get a variable out of the Lua Script
	 *
	 * @param variableName		the name of the variable to be accessed
	 *
	 * @return T	The value of the variable
	 **/
    template<typename T>
    T get(const std::string& variableName) {
      if(!L) {
        printError(variableName, "Script is not loaded");
        return lua_getdefault<T>();
      }
      
      T result;
      if(lua_gettostack(variableName)) { // variable succesfully on top of stack
        result = lua_get<T>(variableName);  
      } else {
        result = lua_getdefault<T>();
      }

     
      clean();
      return result;
    }
	
	template<typename T>
	void set(const std::string& variableName, T value)
	{
		if(!L) {
			printError(variableName, "Script is not loaded");
		}
		if(lua_gettostack(variableName)) {//variable succesfully on top of stack
			lua_set<T>(variableName, value);
		}
	}

	/**
	 * put the variable on top of the stack
	 **/
    bool lua_gettostack(const std::string& variableName) {
      level = 0;
      std::string var = "";
        for(unsigned int i = 0; i < variableName.size(); i++) {
          if(variableName.at(i) == '.') {
            if(level == 0) {
              lua_getglobal(L, var.c_str());
            } else {
              lua_getfield(L, -1, var.c_str());
            }
            
            if(lua_isnil(L, -1)) {
              printError(variableName, var + " is not defined");
              return false;
            } else {
              var = "";
              level++;
            }
          } else {
            var += variableName.at(i);
          }
        }
        if(level == 0) {
          lua_getglobal(L, var.c_str());
        } else {
          lua_getfield(L, -1, var.c_str());
        }
        if(lua_isnil(L, -1)) {
            printError(variableName, var + " is not defined");
            return false;
        }

        return true;       
    }
		
    /**
	 * Generic get for variables
	 **/
    template<typename T>
    T lua_get(const std::string& variableName) {
      return 0;
    }

	/**
	 * Failure case get for variables
	 **/
    template<typename T>
    T lua_getdefault() {
      return 0;
    }

	template<typename T>
	void lua_set(const std::string& variableName, T value){
	}

	/**
	 * Generic get for vectors
	 **/
    template<typename T>
    std::vector<T> lua_getVector(const std::string& vectorName) {
      return vector<T>();
    }

	/**
	 * Failure case get for vectors
	 **/
    template<typename T>
    std::vector<T> lua_getdefaultVector() {
      return std::vector<T>();
    }

	Status getStatus()
	{
		return status;
	}

	void setStatus(Status status)
	{
		this->status = status;
	}

	std::string getFilename()
	{
		return filename.substr(12, filename.length() - 16);
	}
   
	void infiniteRun();

private:
	lua_State* L;
    std::string filename;
    int level;
	Status status;

	LuaScript();
	bool init(const std::string& filename);
};

 // Specializations

template <> 
inline bool LuaScript::lua_get<bool>(const std::string& variableName) {
	return (bool)lua_toboolean(L, -1); 
}

template <> 
inline float LuaScript::lua_get<float>(const std::string& variableName) {
    if(!lua_isnumber(L, -1)) {
      printError(variableName, "Not a number");
    }
    return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaScript::lua_get<int>(const std::string& variableName) {
    if(!lua_isnumber(L, -1)) {
      printError(variableName, "Not a number");
    }
    return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaScript::lua_get<std::string>(const std::string& variableName) {
    std::string s = "null";
    if(lua_isstring(L, -1)) {
      s = std::string(lua_tostring(L, -1));
    } else {
      printError(variableName, "Not a string");
    }
    return s;
}

template<>
inline std::string LuaScript::lua_getdefault<std::string>() {
  return "null";
}

// Specializations for lua set

template <> 
inline void LuaScript::lua_set<bool>(const std::string& variableName, bool value) {
	lua_pushboolean(L, value);
	lua_setglobal(L, variableName.c_str());
}

template <> 
inline void LuaScript::lua_set<float>(const std::string& variableName, float value) {
	lua_pushnumber(L, value);
	lua_setglobal(L, variableName.c_str());
}

template <>
inline void LuaScript::lua_set<int>(const std::string& variableName, int value) {
    lua_pushnumber(L, value);
	lua_setglobal(L, variableName.c_str());
}

template <>
inline void LuaScript::lua_set<std::string>(const std::string& variableName, std::string value) {
	lua_pushstring(L, value.c_str());
	lua_setglobal(L, variableName.c_str());
}

 // Specializations for vectors

template <> 
inline std::vector<bool> LuaScript::lua_getVector<bool>(const std::string& vectorName) {
	std::vector<bool> v;
    while(lua_next(L, -2)) { 
		if(!lua_isboolean(L, -1))
			printError(vectorName, "Not a boolean");
		else
		   v.push_back((bool)lua_toboolean(L, -1));
        lua_pop(L, 1);
    }

    return v;
}

template <> 
inline std::vector<float> LuaScript::lua_getVector<float>(const std::string& vectorName) {
	std::vector<float> v;
    while(lua_next(L, -2)) { 
		 if(!lua_isnumber(L, -1)) 
			printError(vectorName, "Not a number");
		 else
			v.push_back((float)lua_tonumber(L, -1));
        lua_pop(L, 1);
    }

    return v;
}

template <>
inline std::vector<int> LuaScript::lua_getVector<int>(const std::string& vectorName) {
   std::vector<int> v;
    while(lua_next(L, -2)) { 
		 if(!lua_isnumber(L, -1)) 
			printError(vectorName, "Not a number");
		 else
			v.push_back((int)lua_tonumber(L, -1));
        lua_pop(L, 1);
    }

    return v;
}

template <>
inline std::vector<std::string> LuaScript::lua_getVector<std::string>(const std::string& vectorName) {
	std::vector<std::string> v;
    while(lua_next(L, -2)) {
		 if(!lua_isstring(L, -1)) 
			printError(vectorName, "Not a string");
		 else
			v.push_back(std::string(lua_tostring(L, -1)));
        lua_pop(L, 1);
    }
    clean();
    return v;
}

#endif