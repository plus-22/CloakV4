/*
Dragonfire
Copyright (C) 2020 Elias Fleckenstein <eliasfleckenstein@web.de>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#pragma once

#include "cpp_api/s_base.h"
#include <vector>
#include <string>

class ScriptApiCheatsCheat
{
public:
	ScriptApiCheatsCheat(const std::string &name, const std::string &setting, const std::string &info_text);
	ScriptApiCheatsCheat(const std::string &name, const int &function, const std::string &info_text);
	std::string m_name;
	std::string m_info_text;
	void set_info_text(std::string infoText);
	std::string get_info_text();
	bool is_enabled();
	void toggle(lua_State *L, int error_handler);

private:
	std::string m_setting;
	int m_function_ref;
};

class ScriptApiCheatsCheatSetting
{
public:
	ScriptApiCheatsCheatSetting(const std::string &name, const std::string &setting);
	~ScriptApiCheatsCheatSetting();
	void set_value(const bool &value);
	void set_value(const double &value);
	void set_value(const std::string &value);
	std::string m_name;
	std::string m_type;
	std::string m_parent;
	double m_min;
	double m_max;
	double m_steps;
	double m_size;
	std::vector<std::string *> m_options;
private:
	std::string m_setting;
};

class ScriptApiCheatsCategory
{
public:
	ScriptApiCheatsCategory(const std::string &name);
	~ScriptApiCheatsCategory();
	std::string m_name;
	void read_cheats(lua_State *L);
	std::vector<ScriptApiCheatsCheat *> m_cheats;
	std::vector<ScriptApiCheatsCheatSetting *> m_cheat_settings;
	//ScriptApiCheatsCheat* get_cheat(const std::string &name);
};

class ScriptApiCheats : virtual public ScriptApiBase
{
public:
	virtual ~ScriptApiCheats();
	void init_cheats();
	void init_cheat_settings();
	void update_infotexts();
	void toggle_cheat(ScriptApiCheatsCheat *cheat);
	void print_all_cheat_settings(); // New function to print cheat settings

	bool m_cheats_loaded = false;
	std::vector<ScriptApiCheatsCategory *> m_cheat_categories;
	ScriptApiCheatsCategory* get_category(const std::string &name);
};
