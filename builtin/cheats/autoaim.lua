attack_enemies_only = true

core.register_on_active_object_step(function(gcao)
	if (not core.settings:get_bool("autoaim")) then return end
	if (gcao.is_local_player) then return end
    if (not gcao.is_player) then return end
	if (not core.can_attack(gcao.id)) then return end
    local player = core.localplayer
    if (player:get_hp() <= 0) then return end
    local interval = 0.1
	if (gcao.hp <= 0) then return end
	local is_enemy = not player:is_player_friendly(gcao.id)
	if (not is_enemy and attack_enemies_only == true) then
        return
    elseif (not is_enemy and attack_enemies_only == false) then
    end
    local myPos = player:get_pos()
    local enemyPos = gcao.position;
	local range = player:get_wielded_item_range()^2
	local dist = (myPos.x - enemyPos.x)^2 + (myPos.y - enemyPos.y)^2 + (myPos.z - enemyPos.z)^2
	local is_close = dist <= range
	if not is_close then return end
        if (core.settings:get_bool("autoaim")) then
            ws.aim(enemyPos)
        end
    return true
end)

core.register_cheat_with_infotext("Autoaim", "Combat", "autoaim", "")

if attack_enemies_only == true then
    minetest.update_infotext("Autoaim", "Combat", "autoaim", "Enemies")
else
    minetest.update_infotext("Autoaim", "Combat", "autoaim", "All")
end