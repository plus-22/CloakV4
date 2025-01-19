AllPlayers = {}
enemies = {}
local teams = {}

local teams = {}
local valid_teams = {"red", "blue", "green", "purple"}

minetest.register_on_receiving_chat_message(function(message)
    print(message)  -- Выводим полное сообщение для отладки
    if string.sub(message, 1, 4) == "Team" then
        -- Извлекаем имя игрока
        local player_name = string.match(message, "has%s+1%s+players:%s+(%w+)")

        if player_name then
            print("Игрок: " .. player_name)  -- Выводим имя игрока
        else
            print("Имя игрока не найдено в сообщении.")
        end
    end
end)

-- Пример вывода таблицы команд и игроков
-- minetest.register_on_receiving_chat_message(function(message)
--     if string.sub(message, 1, 5) == "New m" then
--         minetest.run_server_chatcommand("team", "")
--     end
--     if string.sub(message, 1, 5) == "Team " then
--         local team_strings = string.split(message, "\n")
--         for _, value in ipairs(team_strings) do
--             print(value)
--             local team = string.match(value, "Team  %(c@#%x+%)%s*(%w+)")
--             if team ~= nil then
--                 print(team)

--                 local playerNames = {}
--                 message = string.split(value, ":")[2]
    
--                 for playerName in string.gmatch(message, "[%a%d_-]+") do
--                     table.insert(playerNames, playerName)
--                 end
--                 local is_team = false
--                 for _, name in ipairs(playerNames) do
--                     if name == minetest.localplayer:get_name() then
--                         is_team = true
--                         local_team = team
--                     end
--                 end
--                 if is_team == false then
--                     enemies[team] = playerNames
--                 end
                
--                 AllPlayers[team] = playerNames
--             end
--         end
--         if awaiting_team then
--             awaiting_team = false
--             return true
--         end
--     end
-- end)

-- minetest.register_on_receiving_chat_message(function(message)
--     if string.sub(message, 1, 4) == "Team" then
--         local player_names = {}

--         for name in string.gmatch(message, "has%s+1%s+players:%s+(%w+)") do
--             table.insert(player_names, name)
--         end

--         -- Выводим имена игроков
--         for _, player in ipairs(player_names) do
--             print("Игрок: " .. player)
--         end
--     end
-- end)

local is_enemy = function(name)
    for _, team in pairs(enemies) do
        for _, player in pairs(team) do
            if player == name then
                return true
            end
        end
    end
    return false
end

local get_team = function(name)
    for teamname, team in pairs(AllPlayers) do
        for _, player in pairs(team) do
            if player == name then
                return teamname
            end
        end
    end
    return "none"
end

