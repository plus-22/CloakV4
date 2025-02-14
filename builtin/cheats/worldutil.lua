-- Credits to Lizzy Fleckenstein
-- lgpl license

core.register_on_dignode(function(pos)
	if core.settings:get_bool("replace") then
		core.after(0, core.place_node, pos)
	end
end)

local etime = 0
local uptime = 0
local hud_id = nil

core.register_globalstep(function(dtime)
	etime = etime + dtime
	if etime < 1 then return end
	local player = core.localplayer
	if not player then return end
	local pos = player:get_pos()
	local item = player:get_wielded_item()
	local def = core.get_item_def(item:get_name())
	local nodes_per_tick = tonumber(core.settings:get("nodes_per_tick")) or 8

	if item and item:get_count() > 0 and def and def.node_placement_prediction ~= "" and (core.settings:get_bool("scaffold") or core.settings:get_bool("scaffold_plus")) then
        local total_items = core.get_total_items(item:get_name())
        local player = core.localplayer
        if player then
            if hud_id then
                player:hud_change(hud_id, "text", total_items .. " " .. item:get_description() .. " left")
            else
                hud_id = player:hud_add({
                    type = "text",
                    position = {x = 0.5, y = 0.5},
                    offset = {x = 0, y = 25},
                    text = total_items .. " " .. item:get_description() .. " left",
                    alignment = {x = 0, y = 0},
                    scale = {x = 150, y = 150},
                    number = 0xF0F0F0
                })
            end
        end
    else
        if hud_id and core.localplayer then
            core.localplayer:hud_remove(hud_id)
            hud_id = nil
        end
    end

	if item and item:get_count() > 0 and def and def.node_placement_prediction ~= "" then
		if core.settings:get_bool("scaffold") then
			core.settings:set_bool("scaffold.active", true)
			local p = vector.round(vector.add(pos, {x = 0, y = -0.6, z = 0}))
			local node = core.get_node_or_nil(p)
			if not node or core.get_node_def(node.name).buildable_to then
				core.place_node(p)
			end
		end
		if core.settings:get_bool("scaffold_plus") then
			core.settings:set_bool("scaffold.active", true)
			local z = pos.z
			local positions = {
				{x = 0, y = -0.6, z = 0},
                {x = 1, y = -0.6, z = 0},
                {x = -1, y = -0.6, z = 0},
                {x = -1, y = -0.6, z = -1},
                {x = 0, y = -0.6, z = -1},
                {x = 1, y = -0.6, z = -1},
                {x = -1, y = -0.6, z = 1},
                {x = 0, y = -0.6, z = 1},
                {x = 1, y = -0.6, z = 1}
			}
			for i, p in pairs(positions) do
				core.place_node(vector.add(pos, p))
			end
		end
		if core.settings:get_bool("block_water") then
			local positions = core.find_nodes_near(pos, 5, {"mcl_core:water_source", "mcl_core:water_floating"}, true)
			for i, p in pairs(positions) do
				if i > nodes_per_tick then return end
				core.place_node(p)
			end
		end
		if core.settings:get_bool("block_lava") then
			local positions = core.find_nodes_near(pos, 5, {"mcl_core:lava_source", "mcl_core:lava_floating"}, true)
			for i, p in pairs(positions) do
				if i > nodes_per_tick then return end
				core.place_node(p)
			end
		end
		if core.settings:get_bool("autotnt") then -- little modification here, autoTNT now will only work if the player is holding tnt
            core.switch_to_item("mcl_tnt:tnt") 
			local positions = core.find_nodes_near_under_air_except(pos, 2, item:get_name(), true)
			for i, p in pairs(positions) do
                core.switch_to_item("mcl_tnt:tnt") 
				if i > nodes_per_tick then return end
                    if core.switch_to_item("mcl_tnt:tnt") then
				        core.place_node(vector.add(p, {x = 0, y = 1, z = 0}))
                    else end
			end
		end
	else
		core.settings:set_bool("scaffold.active", false)
	end
	if core.settings:get_bool("nuke") then
		local i = 0
		for x = pos.x - 4, pos.x + 4 do
			for y = pos.y - 4, pos.y + 4 do
				for z = pos.z - 4, pos.z + 4 do
					local p = vector.new(x, y, z)
					local node = core.get_node_or_nil(p)
					local def = node and core.get_node_def(node.name)
					if def or def.diggable then
						if i > nodes_per_tick then return end
						core.dig_node(p)
						i = i + 1
					end
				end
			end
		end
	end
end)

core.register_cheat("Scaffold", "World", "scaffold")
core.register_cheat("ScaffoldPlus", "World", "scaffold_plus")
core.register_cheat("BlockWater", "World", "block_water")
core.register_cheat("BlockLava", "World", "block_lava")
core.register_cheat("AutoTNT", "World", "autotnt")
core.register_cheat("Nuke", "World", "nuke")
core.register_cheat("Replace", "World", "replace")
