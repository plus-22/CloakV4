
-------------- Killaura -----------------

core.register_cheat_with_infotext("Killaura", "Combat", "killaura", "")
core.register_cheat_setting("Target Mode", "Combat", "killaura", "targeting.target_mode", {type="selectionbox", options={"Nearest", "Lowest HP", "Highest HP"}})
core.register_cheat_setting("Target Type", "Combat", "killaura", "targeting.target_type", {type="selectionbox", options={"Players", "Entities", "Both"}})
core.register_cheat_setting("Distance", "Combat", "killaura", "targeting.distance", {type="slider_int", min=1, max=10, steps=10})
core.register_cheat_setting("Enemies Only", "Combat", "killaura", "targeting.enemies_only", {type="bool"})

-------------- Auto Aim -----------------

core.register_cheat_with_infotext("Autoaim", "Combat", "autoaim", "")
core.register_cheat_setting("Target Mode", "Combat", "autoaim", "targeting.target_mode", {type="selectionbox", options={"Nearest", "Lowest HP", "Highest HP"}})
core.register_cheat_setting("Target Type", "Combat", "autoaim", "targeting.target_type", {type="selectionbox", options={"Players", "Entities", "Both"}})
core.register_cheat_setting("Distance", "Combat", "autoaim", "targeting.distance", {type="slider_int", min=1, max=10, steps=10})
core.register_cheat_setting("Enemies Only", "Combat", "autoaim", "targeting.enemies_only", {type="bool"})
core.register_cheat_setting("Y offset", "Combat", "autoaim", "autoaim.y_offset", {type="slider_int", min=-5, max=10, steps=16})

--------------- Orbit -------------------

local qtime = 0

core.register_cheat_with_infotext("Orbit", "Combat", "orbit", "Nearest")
core.register_cheat_setting("Target Mode", "Combat", "orbit", "targeting.target_mode", {type="selectionbox", options={"Nearest", "Lowest HP", "Highest HP"}})
core.register_cheat_setting("Target Type", "Combat", "orbit", "targeting.target_type", {type="selectionbox", options={"Players", "Entities", "Both"}})
core.register_cheat_setting("Distance", "Combat", "orbit", "targeting.distance", {type="slider_int", min=1, max=10, steps=10})
core.register_cheat_setting("Radius", "Combat", "orbit", "orbit.radius", {type="slider_float", min=1, max=3, steps=5})
core.register_cheat_setting("Enemies Only", "Combat", "orbit", "targeting.enemies_only", {type="bool"})

--------------- Functions -------------------
function is_valid_target(obj, target_type, max_distance,ppos)
    local target_pos = obj:get_pos()
    local distance = vector.distance(ppos, target_pos)
    if distance > max_distance then
        return false
    end

    return ((target_type == "Entities" and not obj:is_player()) or 
            (target_type == "Players" and obj:is_player()) or 
            target_type == "Both") and
           (core.localplayer:get_entity_relationship(obj:get_id()) == core.EntityRelationship.ENEMY) and
           (obj:get_hp() > 0) and 
           core.can_attack(obj:get_id())
end

function get_best_target(objects, target_mode, target_type, max_distance, player)
    local best_target = nil
    local best_value = target_mode == "Highest HP" and 0 or math.huge -- Initialize based on mode
	local ppos = player:get_pos()
    for _, obj in ipairs(objects) do
        if is_valid_target(obj, target_type, max_distance, ppos) then
            local value = target_mode == "Nearest" and vector.distance(ppos, obj:get_pos()) or obj:get_hp()
            local is_better = (target_mode == "Nearest" and value < best_value) or
                              (target_mode == "Lowest HP" and value < best_value) or
                              (target_mode == "Highest HP" and value > best_value)

            if is_better then
                best_value = value
                best_target = obj
            end
        end
    end

    return best_target
end

function get_punch_interval(player)
    local interval_str = core.settings:get("punch_interval")
    local interval
    if interval_str == "auto" then
        interval = player:get_wielded_item():get_tool_capabilities().full_punch_interval + 0.1
    else
        interval = tonumber(interval_str) or 0.1
    end

    if interval <= 0 then
        interval = 0.1
    end

    if core.settings:get_bool("spamclick") then
        local multiplier = tonumber(core.settings:get("spamclick_multiplier")) or 6
        interval = interval / multiplier
    end

    return interval
end

function core.get_send_speed(critspeed)
    if core.settings:get_bool("critical_hits") then 
        critspeed.y = -5 
    end
    return critspeed
end
function extendPoint(yaw, distance)
	local radYaw = math.rad(yaw)
	local dir = vector.new(math.cos(radYaw), 0, math.sin(radYaw))
	dir = vector.multiply(dir, distance)
	return dir
end
core.register_globalstep(function(dtime)
	if core.settings:get_bool("killaura") then
		local target_mode = core.settings:get("targeting.target_mode")
		local mode_text = target_mode and target_mode:gsub(" HP", "") or "Unknown"
	
		local target_description = core.settings:get("targeting.target_type")
		core.update_infotext("Killaura", "Combat", "killaura", string.format("%s, %s", mode_text, target_description))
	end

	if core.settings:get_bool("autoaim") then
		local target_mode = core.settings:get("targeting.target_mode")
		local mode_text = target_mode and target_mode:gsub(" HP", "") or "Unknown"
	
		local target_description = core.settings:get("targeting.target_type")
		core.update_infotext("Autoaim", "Combat", "autoaim", string.format("%s, %s", mode_text, target_description))
	end

	if core.settings:get_bool("orbit") then
		local target_mode = core.settings:get("targeting.target_mode")
		local mode_text = target_mode and target_mode:gsub(" HP", "") or "Unknown"
	
		local target_description = core.settings:get("targeting.target_type")
		core.update_infotext("Orbit", "Combat", "orbit", string.format("%s, %s", mode_text, target_description))
	end

    local player = core.localplayer
	if not player then return end
    local ppos = core.localplayer:get_pos()
	local objects = core.get_nearby_objects(tonumber(core.settings:get("targeting.distance")))
	local target_enemy = nil

	
	if core.settings:get("targeting.target_mode") then
		local target_mode = core.settings:get("targeting.target_mode")
		local target_type = core.settings:get("targeting.target_type")
		local max_distance = tonumber(core.settings:get("targeting.distance")) + 0.5
	
		target_enemy = get_best_target(objects, target_mode, target_type, max_distance, player)
	end

	if target_enemy and core.settings:get_bool("killaura") then
		
		local interval = get_punch_interval(player)

		if player:get_time_from_last_punch() > interval then
			--[[if (core.settings:get_bool("killaura.assist")) then
				local wield_index = player:get_wield_index() + 1
				local dmg = core.get_inv_item_damage(wield_index, target_enemy:get_id())
				if (dmg and target_enemy:get_hp() - dmg.damage <= 1) then
					return
				end
			end]]--

			player:punch(target_enemy:get_id())
		end
	end

	if target_enemy and core.settings:get_bool("autoaim") then
		local enemyPos = target_enemy:get_pos();
		enemyPos.y = enemyPos.y - 1
		enemyPos.y = enemyPos.y + minetest.settings:get("autoaim.y_offset")/10
		ws.aim(enemyPos)
	end

	if target_enemy and core.settings:get_bool("orbit") then
		local opos = target_enemy:get_pos()

		local distance = tonumber(core.settings:get("orbit.radius")) - 0.5

		local dir=vector.direction(ppos,opos)
		local yyaw=0;
		if dir.x < 0 then
			yyaw = math.atan2(-dir.x, dir.z) + (math.pi * 2)
		else
			yyaw = math.atan2(-dir.x, dir.z)
		end
		yyaw = ws.round2(math.deg(yyaw),2)

		

		local target_pos = vector.add(extendPoint(yyaw-90, distance), opos)
		target_pos.y = ppos.y
		local temp_pos = vector.new(ppos.x, opos.y, ppos.z)
		local current_distance = vector.distance(ppos, opos)

		core.localplayer:set_pos(target_pos)

		local vec = vector.subtract(ppos, opos)
		local yaw = math.atan(vec.z / vec.x) - math.pi/2
		yaw = yaw + (opos.x >= ppos.x and math.pi or 0)
		local y = vec.y
		vec.y = 0
		local x = vector.length(vec)
		qtime = qtime + dtime
		if qtime > 0.1 then
			core.set_keypress("jump", true)
			core.set_keypress("left", true)
			core.after(0.099, function()
				core.set_keypress("jump", false)
				core.set_keypress("left", false)
			end)
			qtime = 0
		end
	end
end)



core.register_cheat("Criticals", "Combat", "critical_hits")

core.register_chatcommand("fasthit", {
	params = "<multiplier>",
	description = "Set fasthit multiplier. Used for killaura ONLY when fasthit/spamclick is on.",
	func = function(param)
		local multiplierNum = tonumber(param)
		if multiplierNum then
			if multiplierNum > 0 then
				core.settings:set("spamclick_multiplier", tostring(multiplierNum))
				return true, "Fasthit multiplier has been set to: " .. tostring(multiplierNum) .. "."
			else
				return false, "Error: Input must be greater than 0."
			end
		else
			return false, "Error: Input must be a number."
		end
	end,
})
