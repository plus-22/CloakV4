core.register_cheat_setting("min HP", "Combat", "be_a_bitch", "be_a_bitch.hp", {type="slider_int", min=1, max=10, steps=10})

minetest.register_on_damage_taken(function(hp)
	if minetest.settings:get_bool("be_a_bitch") then
		local hhp=minetest.localplayer:get_hp()
		if (hhp <= tonumber(core.settings:get("be_a_bitch.hp")) ) then
			sh=true
                minetest.disconnect()
		end
    end
end)
minetest.register_cheat("CombatLog", "Combat", "be_a_bitch")