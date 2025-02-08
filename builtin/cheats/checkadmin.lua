local Time = 0
local isAdmin = {}
local color = {a=255, r=255, g=0, b=0}
core.register_on_receiving_chat_message(function(message)
    message = string.gsub(string.gsub(string.gsub(message, "(T@__builtin)", ""), "F", ""), "E", "")
    
    if string.find(message, "Privileges") then
        local player_name = string.match(message, "of%s+(%S+)")
        local privileges_part = string.match(message, ":%s*(.*)")
        
        if privileges_part and player_name then
            local privileges = {}
            local isPlayerAdmin = false
            
            for privilege in string.gmatch(privileges_part, "[^,]+") do
                local trimmed_privilege = privilege:match("^%s*(.-)%s*$")
                table.insert(privileges, trimmed_privilege)
                
                if trimmed_privilege == "ban" or trimmed_privilege == "kick" then
                    isPlayerAdmin = true
                end
            end

            isAdmin[player_name] = isPlayerAdmin
        end
    end
end)

core.register_globalstep(function(dtime)
    Time = Time + dtime

    if Time >= 8 and minetest.settings:get_bool("checkadmin") then
        local player_table = core.get_player_names()

        for _, v in pairs(player_table) do
            core.run_server_chatcommand("privs", v)
            if isAdmin[v .. ":"] then
                local message = minetest.colorize("#696969", v) .. 
                        minetest.colorize("#FFFFFF", " Is Admin")
                ws.dcm(message)
            end
        end

        Time = 0
    end
end)
--Temporally disabled cuz im still working on it
--minetest.register_cheat("CheckAdmin", "Misc", "checkadmin")