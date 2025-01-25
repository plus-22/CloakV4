core.register_cheat_setting("Distance", "Combat", "autoaim", "autoaim.distance", {type="slider_int", min=1, max=10, steps=10})
core.register_cheat_setting("Only Enemies", "Combat", "autoaim", "autoaim.aim_all", {type="bool"})
core.register_on_active_object_step(function(gcao)
    if minetest.settings:get_bool("autoaim.aim_all") == true then
        core.update_infotext("Autoaim", "Combat", "autoaim", "Enemies, Range: " .. core.settings:get("autoaim.distance") .. "m")
    else
        core.update_infotext("Autoaim", "Combat", "autoaim", "All, Range: " .. core.settings:get("autoaim.distance") .. "m")
    end
    local player = core.localplayer
    if (not core.settings:get_bool("autoaim") or not player or not gcao or gcao.is_local_player or not gcao.is_player or not core.can_attack(gcao.id)) then return end
    if (player:get_hp() <= 0 or gcao.hp <= 0) then return end
	local is_enemy = player:get_entity_relationship(gcao.id) == core.EntityRelationship.ENEMY
    if (not is_enemy and minetest.settings:get_bool("autoaim.aim_all") == true) then
        return
    end
    local myPos = player:get_pos()
    local enemyPos = gcao.position;
	local range_setting = core.settings:get("autoaim.distance")
    local range = tonumber(range_setting) or 10 
	local dist = (myPos.x - enemyPos.x)^2 + (myPos.y - enemyPos.y)^2 + (myPos.z - enemyPos.z)^2
	local is_close = dist <= (range^2)
    if not is_close then return end
    if (core.settings:get_bool("autoaim")) then
        ws.aim(enemyPos)
    end
    return true
end)
core.register_cheat_with_infotext("Autoaim", "Combat", "autoaim", "")