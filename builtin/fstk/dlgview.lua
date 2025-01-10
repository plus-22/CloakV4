--Minetest
--Copyright (C) 2014 sapier
--
--This program is free software; you can redistribute it and/or modify
--it under the terms of the GNU Lesser General Public License as published by
--the Free Software Foundation; either version 2.1 of the License, or
--(at your option) any later version.
--
--This program is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--GNU Lesser General Public License for more details.
--
--You should have received a copy of the GNU Lesser General Public License along
--with this program; if not, write to the Free Software Foundation, Inc.,
--51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


--------------------------------------------------------------------------------
-- A dlgview implementation                                                   --
-- Usage:                                                                     --
-- dlgview.create: returns initialized dlgview raw element                    --
-- element.add(tab): add a tab declaration                                    --
-- element.handle_buttons()                                                   --
-- element.handle_events()                                                    --
-- element.getFormspec() returns formspec of dlgview                          --
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
local function add_tab(self,tab)
	assert(tab.size == nil or (type(tab.size) == table and
			tab.size.x ~= nil and tab.size.y ~= nil))
	assert(tab.cbf_formspec ~= nil and type(tab.cbf_formspec) == "function")
	assert(tab.cbf_button_handler == nil or
			type(tab.cbf_button_handler) == "function")
	assert(tab.cbf_events == nil or type(tab.cbf_events) == "function")

	local newtab = {
		name = tab.name,
		caption = tab.caption,
		button_handler = tab.cbf_button_handler,
		event_handler = tab.cbf_events,
		get_formspec = tab.cbf_formspec,
		tabsize = tab.tabsize,
		formspec_version = tab.formspec_version or 6,
		on_change = tab.on_change,
		tabdata = {},
	}

	self.tablist[#self.tablist + 1] = newtab

	if self.last_tab_index == #self.tablist then
		self.current_tab = tab.name
		if tab.on_activate ~= nil then
			tab.on_activate(nil,tab.name)
		end
	end
end

--------------------------------------------------------------------------------
local function get_formspec(self)
	if self.hidden or (self.parent ~= nil and self.parent.hidden) then
		return ""
	else
		mm_game_theme.clear_single("header")
	end
	local tsize = { width = self.width, height = self.height }
	if self.parent == nil and not prepend then
		prepend = string.format("size[%f,%f,%s]", tsize.width, tsize.height, dump(self.fixed_size)) ..
		"position[0.01,0.01]" .. "anchor[0,0]" ..
		"bgcolor[#0000]" ..
		"style_type[image_button;border=false;textcolor=white;font_size=*2;padding=0;font=bold;bgimg=" .. core.formspec_escape(defaulttexturedir .. "menu_local.png") .. ";bgimg_hovered=" .. core.formspec_escape(defaulttexturedir .. "menu_local_hovered.png") .. "]" ..
		"image_button[0,1;4,0.95;;local_btn;" .. fgettext("Local") .. "]"..
		"style_type[image_button;border=false;textcolor=white;font_size=*2;padding=0;font=bold;bgimg=" .. core.formspec_escape(defaulttexturedir .. "menu_online.png") .. ";bgimg_hovered=" .. core.formspec_escape(defaulttexturedir .. "menu_online_hovered.png") .. "]" ..
		"image_button[0,2;4,0.95;;online;" .. fgettext("Online") .. "]" ..
		"style_type[image_button;border=false;textcolor=white;font_size=*2;padding=0;font=bold;bgimg=" .. core.formspec_escape(defaulttexturedir .. "menu_csm.png") .. ";bgimg_hovered=" .. core.formspec_escape(defaulttexturedir .. "menu_csm_hovered.png") .. "]" ..
		"image_button[0,3;4,0.95;;csm;" .. fgettext("CSMs") .. "]" ..
		"image_button[0,4;4,0.95;;content;" .. fgettext("Content") .. "]" ..
		"style_type[image_button;border=false;textcolor=white;font_size=*2;padding=0;font=bold;bgimg=" .. core.formspec_escape(defaulttexturedir .. "menu_settings.png") .. ";bgimg_hovered=" .. core.formspec_escape(defaulttexturedir .. "menu_settings_hovered.png") .. "]" ..
		"image_button[0,5;4,0.95;;settings;" .. fgettext("Settings") .. "]" ..
		"style_type[image_button;border=false;textcolor=white;font_size=*2;padding=0;font=bold;bgimg=" .. core.formspec_escape(defaulttexturedir .. "menu_about.png") .. ";bgimg_hovered=" .. core.formspec_escape(defaulttexturedir .. "menu_about_hovered.png") .. "]" ..
		"image_button[0,6;4,0.95;;about;" .. fgettext("About") .. "]" ..
		"image[-1,-0.7;7,1.8;" .. core.formspec_escape(defaulttexturedir .. "menu_header.png") .. "]"
	end

	local formspec = (prepend or "")

	return formspec
end

--------------------------------------------------------------------------------
local function handle_buttons(self,fields)

	if self.hidden then
		return false
	end

	if fields.local_btn then
		local dlg = create_local_dlg()
		dlg:set_parent(self)
		self:hide()
		dlg:show()
		return true
	end

	if fields.online then
		local dlg = create_online_dlg()
		dlg:set_parent(self)
		self:hide()
		dlg:show()
		return true
	end

	if fields.csm then
		local dlg = create_csm_dlg()
		dlg:set_parent(self)
		self:hide()
		dlg:show()
		return true
	end

	if fields.content then
		local dlg = create_content_dlg()
		dlg:set_parent(self)
		self:hide()
		dlg:show()
		return true
	end

	if fields.settings then
		local dlg = create_settings_dlg()
		dlg:set_parent(self)
		self:hide()
		dlg:show()
		return true
	end

	if fields.about then
		local dlg = create_about_dlg()
		dlg:set_parent(self)
		self:hide()
		dlg:show()
		return true
	end

	return false
end

--------------------------------------------------------------------------------
local function handle_events(self,event)

	if self.hidden then
		return false
	end

	if self.glb_evt_handler ~= nil and
		self.glb_evt_handler(self,event) then
		return true
	end

	local tab = self.tablist[self.last_tab_index]
	if tab.evt_handler ~= nil then
		return tab.evt_handler(self, event, tab.name, tab.tabdata)
	end

	return false
end


--------------------------------------------------------------------------------
local function tab_header(self, size)
	local toadd = ""

	for i = 1, #self.tablist do
		if toadd ~= "" then
			toadd = toadd .. ","
		end

		local caption = self.tablist[i].caption
		if type(caption) == "function" then
			caption = caption(self)
		end

		toadd = toadd .. caption
	end
	return ""
	--return string.format("tabheader[%f,%f;%f,%f;%s;%s;%i;true;false]",
	--		self.header_x, self.header_y, size.width, size.height, self.name, toadd, self.last_tab_index)
end

--------------------------------------------------------------------------------
local function switch_to_tab(self, index)
	--first call on_change for tab to leave
	if self.tablist[self.last_tab_index].on_change ~= nil then
		self.tablist[self.last_tab_index].on_change("LEAVE",
				self.current_tab, self.tablist[index].name)
	end

	--update dlgview data
	self.last_tab_index = index
	local old_tab = self.current_tab
	self.current_tab = self.tablist[index].name

	if (self.autosave_tab) then
		core.settings:set(self.name .. "_LAST",self.current_tab)
	end

	-- call for tab to enter
	if self.tablist[index].on_change ~= nil then
		self.tablist[index].on_change("ENTER",
				old_tab,self.current_tab)
	end
end

--------------------------------------------------------------------------------
local function handle_tab_buttons(self,fields)
	--save tab selection to config file
	if fields[self.name] then
		local index = tonumber(fields[self.name])
		switch_to_tab(self, index)
		return true
	end

	return false
end

--------------------------------------------------------------------------------
local function set_tab_by_name(self, name)
	for i=1,#self.tablist,1 do
		if self.tablist[i].name == name then
			switch_to_tab(self, i)
			return true
		end
	end

	return false
end

--------------------------------------------------------------------------------
local function hide_dlgview(self)
	self.hidden=true

end

--------------------------------------------------------------------------------
local function show_dlgview(self)
	self.hidden=false
end

local dlgview_metatable = {
	add                       = add_tab,
	handle_buttons            = handle_buttons,
	handle_events             = handle_events,
	get_formspec              = get_formspec,
	show                      = show_dlgview,
	hide                      = hide_dlgview,
	delete                    = function(self) ui.delete(self) end,
	set_parent                = function(self,parent) self.parent = parent end,
	set_autosave_tab          =
			function(self,value) self.autosave_tab = value end,
	set_tab                   = set_tab_by_name,
	set_global_button_handler =
			function(self,handler) self.glb_btn_handler = handler end,
	set_global_event_handler =
			function(self,handler) self.glb_evt_handler = handler end,
	set_fixed_size =
			function(self,state) self.fixed_size = state end,
	set_end_button =
			function(self, v) self.end_button = v end,
	tab_header = tab_header,
	handle_tab_buttons = handle_tab_buttons
}

dlgview_metatable.__index = dlgview_metatable

--------------------------------------------------------------------------------
function dlgview_create(name, size, tabheaderpos)
	local self = {}

	self.name     = name
	self.type     = "toplevel"
	self.width    = size.x
	self.height   = size.y
	self.header_x = tabheaderpos.x
	self.header_y = tabheaderpos.y

	setmetatable(self, dlgview_metatable)

	self.fixed_size     = true
	self.hidden         = true
	self.current_tab    = nil
	self.last_tab_index = 1
	self.tablist        = {}

	self.autosave_tab   = false

	ui.add(self)
	return self
end
