attack_enemies_only = true
core.register_on_active_object_step(function(gcao)
    local player = core.localplayer
    if (not core.settings:get_bool("autoaim") or not player or not gcao or gcao.is_local_player or not gcao.is_player or not core.can_attack(gcao.id)) then return end
    if (player:get_hp() <= 0 or gcao.hp <= 0) then return end
	local is_enemy = not player:is_player_friendly(gcao.id)
    if (not is_enemy and attack_enemies_only == true) then
        return
    end
    local myPos = player:get_pos()
    local enemyPos = gcao.position;
	local range = player:get_wielded_item_range()^2
	local dist = (myPos.x - enemyPos.x)^2 + (myPos.y - enemyPos.y)^2 + (myPos.z - enemyPos.z)^2
	local is_close = dist <= range
	if not is_close then 
        return
    else
        ws.aim(enemyPos)
    end
    return true
end)
core.register_cheat_with_infotext("Autoaim", "Combat", "autoaim", "")
if attack_enemies_only == true then
    core.update_infotext("Autoaim", "Combat", "autoaim", "Enemies")
else
    core.update_infotext("Autoaim", "Combat", "autoaim", "All")
end
