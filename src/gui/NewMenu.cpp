#include "NewMenu.h"
#include "settings.h"
#include <iostream>

NewMenu::NewMenu(gui::IGUIEnvironment* env, 
    gui::IGUIElement* parent, 
    s32 id, IMenuManager* menumgr, 
    Client* client)
    : IGUIElement(gui::EGUIET_ELEMENT, env, parent, id, 
    core::rect<s32>(0, 0, 0, 0)), 
    m_menumgr(menumgr), isDragging(false), draggedRectIndex(-1),
    m_client(client), m_rectsCreated(false) 
{    
    infostream << "[NEWMENU] Successfully created" << std::endl;
    this->env = env;
}

NewMenu::~NewMenu()
{
}

void NewMenu::create()
{
    GET_SCRIPT_POINTER

    if (script->m_cheat_categories.empty()) {
        std::cout << "No categories available." << std::endl;
        return;
    }
    if (!m_initialized) {
        category_positions.resize(script->m_cheat_categories.size(), core::position2d<s32>(0, 0));
        categoryRects.resize(script->m_cheat_categories.size(), core::rect<s32>(0,0,0,0));
        dropdownRects.resize(script->m_cheat_categories.size(), core::rect<s32>(0,0,0,0));
        textRects.resize(script->m_cheat_categories.size(), core::rect<s32>(0,0,0,0));
        selectedCategory.resize(script->m_cheat_categories.size(), false);
        selectedCheat.resize(script->m_cheat_categories.size());
        dropdownHovered.resize(script->m_cheat_categories.size(), false);
        textHovered.resize(script->m_cheat_categories.size(), false);
        cheatRects.resize(script->m_cheat_categories.size());
        cheatTextRects.resize(script->m_cheat_categories.size());
        cheatDropdownRects.resize(script->m_cheat_categories.size());
        cheatDropdownHovered.resize(script->m_cheat_categories.size());
        cheat_positions.resize(script->m_cheat_categories.size());
        cheatTextHovered.resize(script->m_cheat_categories.size());
        cheatSettingRects.resize(script->m_cheat_categories.size());
        cheatSettingTextRects.resize(script->m_cheat_categories.size());
        cheatSettingTextHovered.resize(script->m_cheat_categories.size());
        cheat_setting_positions.resize(script->m_cheat_categories.size());

        for (size_t i = 0; i < script->m_cheat_categories.size(); ++i) {
            category_positions[i] = core::position2d<s32>(category_height / 2, category_height / 2 + ((category_height + category_height / 2) * i));
            categoryRects[i] = core::rect<s32>(category_positions[i].X, category_positions[i].Y, 
                                            category_positions[i].X + category_width, category_positions[i].Y + category_height);

            textRects[i] = core::rect<s32>(category_positions[i].X, category_positions[i].Y, 
                                        category_positions[i].X + (category_width - category_height), category_positions[i].Y + category_height);

            dropdownRects[i] = core::rect<s32>(category_positions[i].X + (category_width - category_height), category_positions[i].Y, 
                                            category_positions[i].X + category_width, category_positions[i].Y + category_height);
                                    
            cheatRects[i].resize(script->m_cheat_categories[i]->m_cheats.size(), core::rect<s32>(0,0,0,0));
            cheatTextRects[i].resize(script->m_cheat_categories[i]->m_cheats.size(), core::rect<s32>(0,0,0,0));
            cheatDropdownRects[i].resize(script->m_cheat_categories[i]->m_cheats.size(), core::rect<s32>(0,0,0,0));
            cheatDropdownHovered[i].resize(script->m_cheat_categories[i]->m_cheats.size(), false);
            cheatTextHovered[i].resize(script->m_cheat_categories[i]->m_cheats.size(), false);
            selectedCheat[i].resize(script->m_cheat_categories[i]->m_cheats.size(), false);
            cheat_positions[i].resize(script->m_cheat_categories[i]->m_cheats.size(), core::position2d<s32>(0, 0));
            cheatSettingRects[i].resize(script->m_cheat_categories[i]->m_cheats.size());
            cheatSettingTextRects[i].resize(script->m_cheat_categories[i]->m_cheats.size());
            cheatSettingTextHovered[i].resize(script->m_cheat_categories[i]->m_cheats.size());
            cheat_setting_positions[i].resize(script->m_cheat_categories[i]->m_cheats.size());
            for (size_t c = 0; c < script->m_cheat_categories[i]->m_cheats.size(); ++c) {
                cheatSettingRects[i][c].resize(script->m_cheat_categories[i]->m_cheats[c]->m_cheat_settings.size());
                cheatSettingTextRects[i][c].resize(script->m_cheat_categories[i]->m_cheats[c]->m_cheat_settings.size());
                cheatSettingTextHovered[i][c].resize(script->m_cheat_categories[i]->m_cheats[c]->m_cheat_settings.size(), false);
                cheat_setting_positions[i][c].resize(script->m_cheat_categories[i]->m_cheats[c]->m_cheat_settings.size());
            }
        }
        m_initialized = true;
    }

    core::rect<s32> screenRect(0, 0, 
        Environment->getVideoDriver()->getScreenSize().Width, 
        Environment->getVideoDriver()->getScreenSize().Height);
    setRelativePosition(screenRect);

    IGUIElement::setVisible(true);
    Environment->setFocus(this);
    m_menumgr->createdMenu(this);
    m_is_open = true;
}

void NewMenu::close()
{
    Environment->removeFocus(this);
    m_menumgr->deletingMenu(this);
    IGUIElement::setVisible(false);
    m_is_open = false;
}

core::rect<s32> NewMenu::createRect(s32 x, s32 y) {
    return core::rect<s32>(x, y, x + category_width, y + category_height);
}

s32 NewMenu::respaceMenu(size_t i)
{
    GET_SCRIPT_POINTER_S32
    s32 last_height = category_positions[i].Y + category_height + 1;
    categoryRects[i] = core::rect<s32>(category_positions[i].X, category_positions[i].Y, 
                                category_positions[i].X + category_width, category_positions[i].Y + category_height);

    textRects[i] = core::rect<s32>(category_positions[i].X, category_positions[i].Y, 
                                category_positions[i].X + (category_width - category_height), category_positions[i].Y + category_height);

    dropdownRects[i] = core::rect<s32>(category_positions[i].X + (category_width - category_height), category_positions[i].Y, 
                                    category_positions[i].X + category_width, category_positions[i].Y + category_height);
    

    for (size_t c = 0; c < script->m_cheat_categories[i]->m_cheats.size(); ++c) {
        cheat_positions[i][c] = core::position2d<s32>(category_positions[i].X, last_height);
        last_height += category_height;
        cheatRects[i][c] = core::rect<s32>(cheat_positions[i][c].X, cheat_positions[i][c].Y, 
                                            cheat_positions[i][c].X + category_width, cheat_positions[i][c].Y + category_height);
        if (script->m_cheat_categories[i]->m_cheats[c]->has_settings()) {
            
            cheatTextRects[i][c] = core::rect<s32>(cheat_positions[i][c].X, cheat_positions[i][c].Y, 
                                                cheat_positions[i][c].X + (category_width - category_height), cheat_positions[i][c].Y + category_height);

            cheatDropdownRects[i][c] = core::rect<s32>(cheat_positions[i][c].X + (category_width - category_height), cheat_positions[i][c].Y, 
                                                    cheat_positions[i][c].X + category_width, cheat_positions[i][c].Y + category_height);
            if (selectedCheat[i][c]) {
                for (size_t s = 0; s < script->m_cheat_categories[i]->m_cheats[c]->m_cheat_settings.size(); ++s) {
                    cheat_setting_positions[i][c][s] = core::position2d<s32>(category_positions[i].X, last_height);
                    if (script->m_cheat_categories[i]->m_cheats[c]->m_cheat_settings[s]->m_type == "selectionbox") {
                        cheatSettingRects[i][c][s] = core::rect<s32>(cheat_setting_positions[i][c][s].X,                  cheat_setting_positions[i][c][s].Y, 
                                                                     cheat_setting_positions[i][c][s].X + category_width, cheat_setting_positions[i][c][s].Y + category_height * 2);
                    } else {
                        cheatSettingRects[i][c][s] = core::rect<s32>(cheat_setting_positions[i][c][s].X,                  cheat_setting_positions[i][c][s].Y, 
                                                                     cheat_setting_positions[i][c][s].X + category_width, cheat_setting_positions[i][c][s].Y + category_height);
                        cheatSettingTextRects[i][c][s] = core::rect<s32>(cheat_setting_positions[i][c][s].X + (setting_bar_padding * 2) + setting_bar_width, cheat_setting_positions[i][c][s].Y, 
                                                                         cheat_setting_positions[i][c][s].X + category_width,                                cheat_setting_positions[i][c][s].Y + category_height);
                    }
                    
                    last_height += cheatSettingRects[i][c][s].getHeight();
                }
            }
        } else {
            cheatTextRects[i][c] = core::rect<s32>(cheat_positions[i][c].X, cheat_positions[i][c].Y, 
                                                cheat_positions[i][c].X + category_width, cheat_positions[i][c].Y + category_height);

            cheatDropdownRects[i][c] = core::rect<s32>(cheat_positions[i][c].X + category_width, cheat_positions[i][c].Y, 
                                                    cheat_positions[i][c].X + category_width, cheat_positions[i][c].Y);
        }
    }
    return last_height;
}

bool NewMenu::OnEvent(const irr::SEvent& event) 
{
    if (!m_is_open) {
        return false;
    }
    GET_SCRIPT_POINTER_BOOL  
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        if (event.KeyInput.Key == KEY_ESCAPE)
        {
            close();
            return true; 
        }
    }
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
            for (size_t i = 0; i < script->m_cheat_categories.size(); ++i) {
                if (dropdownRects[i].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y))) {
                    selectedCategory[i] = !selectedCategory[i];
                    s32 screenWidth = Environment->getVideoDriver()->getScreenSize().Width;
                    s32 screenHeight = Environment->getVideoDriver()->getScreenSize().Height;
                    s32 newX = category_positions[i].X;
                    s32 newY = category_positions[i].Y;

                    if (newX < 0) {
                        newX = 0;
                    } else if (newX + categoryRects[i].getWidth() > screenWidth) {
                        newX = screenWidth - categoryRects[i].getWidth();
                    }
                    s32 cheats_height = categoryRects[i].getHeight();
                    if (selectedCategory[i]){
                        cheats_height = cheatRects[i][cheatRects[i].size() - 1].LowerRightCorner.Y - category_positions[i].Y;
                    } 
                    if (newY < 0) {
                        newY = 0;
                    } else if (newY + cheats_height > screenHeight) {
                        newY = screenHeight - cheats_height;
                    }
                    if (g_settings->exists("use_menu_grid") && g_settings->getBool("use_menu_grid") == true) {
                        newX = std::round(newX / (category_height / 2)) * (category_height / 2);
                        newY = std::round(newY / (category_height / 2)) * (category_height / 2);
                    }

                    category_positions[i] = core::position2d<s32>(newX, newY);
                    
                    respaceMenu(i);
                    return true;
                }

                if (textRects[i].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y))) {
                    isDragging = true;
                    draggedRectIndex = i;
                    offset = core::vector2d<s32>(event.MouseInput.X - textRects[i].UpperLeftCorner.X, event.MouseInput.Y - textRects[i].UpperLeftCorner.Y);
                    return true; 
                }

                for (size_t c = 0; c < script->m_cheat_categories[i]->m_cheats.size(); ++c) {
                    if (cheatTextRects[i][c].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y))) {
                        script->toggle_cheat(script->m_cheat_categories[i]->m_cheats[c]);
                        return true; 
                    }

                    if (cheatDropdownRects[i][c].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y))) {
                        selectedCheat[i][c] = !selectedCheat[i][c];
                        
                        s32 last_height = respaceMenu(i);
                        s32 screenWidth = Environment->getVideoDriver()->getScreenSize().Width;
                        s32 screenHeight = Environment->getVideoDriver()->getScreenSize().Height;
                        s32 newX = category_positions[i].X;
                        s32 newY = category_positions[i].Y;

                        if (newX < 0) {
                            newX = 0;
                        } else if (newX + categoryRects[i].getWidth() > screenWidth) {
                            newX = screenWidth - categoryRects[i].getWidth();
                        }
                        s32 cheats_height = categoryRects[i].getHeight();
                        if (selectedCategory[i]){
                            cheats_height = last_height - categoryRects[i].UpperLeftCorner.Y;
                        } 
                        if (newY < 0) {
                            newY = 0;
                        } else if (newY + cheats_height > screenHeight) {
                            newY = screenHeight - cheats_height;
                        }
                        if (g_settings->exists("use_menu_grid") && g_settings->getBool("use_menu_grid") == true) {
                            newX = std::round(newX / (category_height / 2)) * (category_height / 2);
                            newY = std::round(newY / (category_height / 2)) * (category_height / 2);
                        }

                        category_positions[i] = core::position2d<s32>(newX, newY);
                        
                        respaceMenu(i);
                        return true; 
                    }

                    for (size_t s = 0; s < script->m_cheat_categories[i]->m_cheats[c]->m_cheat_settings.size(); ++s) {
                        if (cheatSettingTextRects[i][c][s].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y))) {
                            script->m_cheat_categories[i]->m_cheats[c]->m_cheat_settings[s]->toggle();
                        }
                    }
                }
                
            }
            return false; 
        } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
            isDragging = false;
            draggedRectIndex = -1;
            return true;
        } else if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED && isDragging && draggedRectIndex != -1) {
            s32 last_height = respaceMenu(draggedRectIndex);
            s32 screenWidth = Environment->getVideoDriver()->getScreenSize().Width;
            s32 screenHeight = Environment->getVideoDriver()->getScreenSize().Height;

            s32 newX = event.MouseInput.X - offset.X;
            s32 newY = event.MouseInput.Y - offset.Y;

            if (newX < 0) {
                newX = 0;
            } else if (newX + categoryRects[draggedRectIndex].getWidth() > screenWidth) {
                newX = screenWidth - categoryRects[draggedRectIndex].getWidth();
            }
            s32 cheats_height = categoryRects[draggedRectIndex].getHeight();
            if (selectedCategory[draggedRectIndex]){
                cheats_height = last_height - categoryRects[draggedRectIndex].UpperLeftCorner.Y;
            } 
            if (newY < 0) {
                newY = 0;
            } else if (newY + cheats_height > screenHeight) {
                newY = screenHeight - cheats_height;
            }
            if (g_settings->exists("use_menu_grid") && g_settings->getBool("use_menu_grid") == true) {
                newX = std::round(newX / (category_height / 2)) * (category_height / 2);
                newY = std::round(newY / (category_height / 2)) * (category_height / 2);
            }

            category_positions[draggedRectIndex] = core::position2d<s32>(newX, newY);
            respaceMenu(draggedRectIndex);

            return true;
        } else if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {
            for (size_t i = 0; i < script->m_cheat_categories.size(); ++i) {

                dropdownHovered[i] = dropdownRects[i].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y));

                textHovered[i] = textRects[i].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y));

                for (size_t c = 0; c < script->m_cheat_categories[i]->m_cheats.size(); ++c) {

                    cheatTextHovered[i][c] = cheatTextRects[i][c].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y));

                    cheatDropdownHovered[i][c] = cheatDropdownRects[i][c].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y));

                    for (size_t s = 0; s < script->m_cheat_categories[i]->m_cheats[c]->m_cheat_settings.size(); ++s) {

                        cheatSettingTextHovered[i][c][s] = cheatSettingTextRects[i][c][s].isPointInside(core::vector2d<s32>(event.MouseInput.X, event.MouseInput.Y));

                    }
                }
            }
        }
    }

    return Parent ? Parent->OnEvent(event) : false; 
}

void NewMenu::drawCategory(video::IVideoDriver* driver, gui::IGUIFont* font, const size_t i)
{
    GET_SCRIPT_POINTER
    // TODO REMOVE THESE AND STORE THEM AS SETTINGS
    const s32 unit_size = category_height / 9;
    const video::SColor category_color = video::SColor(230, 30, 30, 40);
    const video::SColor cheat_color = video::SColor(210, 10, 15, 20);
    const video::SColor cheat_color_enabled = video::SColor(210, 50, 80, 175);
    const video::SColor outline_color = video::SColor(230, 75, 125, 250);
    
    ///////////////////////// DRAW CATEGORY HEADER /////////////////////////
    video::SColor arrow_color = video::SColor(255, 255, 255, 255);
    video::SColor text_color = video::SColor(255, 255, 255, 255);
    if (dropdownHovered[i] == true) {
        arrow_color = video::SColor(255, 127, 127, 127);
    }
    if (textHovered[i] == true) {
        text_color = video::SColor(255, 127, 127, 127);
    }

    driver->draw2DRectangle(category_color, categoryRects[i]);
    
    driver->draw2DRectangleOutline(core::rect<s32>(categoryRects[i].UpperLeftCorner.X, categoryRects[i].UpperLeftCorner.Y, categoryRects[i].LowerRightCorner.X - 1, categoryRects[i].LowerRightCorner.Y - 1), outline_color);
    driver->draw2DRectangleOutline(core::rect<s32>(categoryRects[i].UpperLeftCorner.X - 1, categoryRects[i].UpperLeftCorner.Y - 1, categoryRects[i].LowerRightCorner.X, categoryRects[i].LowerRightCorner.Y), outline_color);

    const std::string& categoryName = script->m_cheat_categories[i]->m_name;
    std::wstring wCategoryName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(categoryName);

    core::dimension2d<u32> textSizeU32 = font->getDimension(wCategoryName.c_str());
    core::dimension2d<s32> textSize(textSizeU32.Width, textSizeU32.Height);

    s32 textX = textRects[i].UpperLeftCorner.X + (textRects[i].getWidth() - textSize.Width) / 2;
    s32 textY = textRects[i].UpperLeftCorner.Y + (textRects[i].getHeight() - textSize.Height) / 2;


    font->draw(wCategoryName.c_str(), core::rect<s32>(textX, textY, textX + textSize.Width, textY + textSize.Height), text_color);

    core::position2d<s32> dropdown_center(dropdownRects[i].UpperLeftCorner.X + dropdownRects[i].getWidth() / 2 ,dropdownRects[i].UpperLeftCorner.Y + dropdownRects[i].getHeight() / 2);

    if (selectedCategory[i]) {
        driver->draw2DLine(core::position2d<s32>(dropdown_center.X - (unit_size * 3), dropdown_center.Y + (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X, dropdown_center.Y - (unit_size * 1.5)), arrow_color);
        driver->draw2DLine(core::position2d<s32>(dropdown_center.X - (unit_size * 3), 1 + dropdown_center.Y + (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X, 1 + dropdown_center.Y - (unit_size * 1.5)), arrow_color);
        driver->draw2DLine(core::position2d<s32>(dropdown_center.X, dropdown_center.Y - (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X + (unit_size * 3), dropdown_center.Y + (unit_size * 1.5)), arrow_color);
        driver->draw2DLine(core::position2d<s32>(dropdown_center.X, 1 + dropdown_center.Y - (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X + (unit_size * 3), 1 + dropdown_center.Y + (unit_size * 1.5)), arrow_color);
        ///////////////////////// DRAW CHEATS /////////////////////////
        for (size_t cheat_index = 0; cheat_index < script->m_cheat_categories[i]->m_cheats.size(); ++cheat_index) {
            arrow_color = video::SColor(255, 255, 255, 255);
            text_color = video::SColor(255, 255, 255, 255);
            if (cheatDropdownHovered[i][cheat_index] == true) {
                arrow_color = video::SColor(255, 127, 127, 127);
            }
            if (cheatTextHovered[i][cheat_index] == true) {
                text_color = video::SColor(255, 127, 127, 127);
            }
            if (script->m_cheat_categories[i]->m_cheats[cheat_index]->is_enabled()) {
                driver->draw2DRectangle(cheat_color_enabled, cheatRects[i][cheat_index]);
            } else {
                driver->draw2DRectangle(cheat_color, cheatRects[i][cheat_index]);
            }

            const std::string& cheatName = script->m_cheat_categories[i]->m_cheats[cheat_index]->m_name;
            std::wstring wCheatName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(cheatName);

            textSizeU32 = font->getDimension(wCheatName.c_str());
            core::dimension2d<s32> textSize(textSizeU32.Width, textSizeU32.Height);

            s32 textX = cheatTextRects[i][cheat_index].UpperLeftCorner.X + (cheatTextRects[i][cheat_index].getWidth() - textSize.Width) / 2;
            s32 textY = cheatTextRects[i][cheat_index].UpperLeftCorner.Y + (cheatTextRects[i][cheat_index].getHeight() - textSize.Height) / 2;


            font->draw(wCheatName.c_str(), core::rect<s32>(textX, textY, textX + textSize.Width, textY + textSize.Height), text_color);
            if (script->m_cheat_categories[i]->m_cheats[cheat_index]->has_settings()) {
                core::position2d<s32> dropdown_center(cheatDropdownRects[i][cheat_index].UpperLeftCorner.X + cheatDropdownRects[i][cheat_index].getWidth() / 2, 
                                                      cheatDropdownRects[i][cheat_index].UpperLeftCorner.Y + cheatDropdownRects[i][cheat_index].getHeight() / 2);

                if (selectedCheat[i][cheat_index]) {
                    driver->draw2DLine(core::position2d<s32>(dropdown_center.X - (unit_size * 3), dropdown_center.Y + (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X, dropdown_center.Y - (unit_size * 1.5)), arrow_color);
                    driver->draw2DLine(core::position2d<s32>(dropdown_center.X - (unit_size * 3), 1 + dropdown_center.Y + (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X, 1 + dropdown_center.Y - (unit_size * 1.5)), arrow_color);
                    driver->draw2DLine(core::position2d<s32>(dropdown_center.X, dropdown_center.Y - (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X + (unit_size * 3), dropdown_center.Y + (unit_size * 1.5)), arrow_color);
                    driver->draw2DLine(core::position2d<s32>(dropdown_center.X, 1 + dropdown_center.Y - (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X + (unit_size * 3), 1 + dropdown_center.Y + (unit_size * 1.5)), arrow_color);
                    ///////////////////////// DRAW CHEAT SETTINGS /////////////////////////

                    driver->draw2DRectangle(settingBackgroundColor, core::rect<s32>(cheatSettingRects[i][cheat_index][0].UpperLeftCorner.X,
                                                                                    cheatSettingRects[i][cheat_index][0].UpperLeftCorner.Y, 
                                                                                    cheatSettingRects[i][cheat_index][cheatSettingRects[i][cheat_index].size() - 1].LowerRightCorner.X,
                                                                                    cheatSettingRects[i][cheat_index][cheatSettingRects[i][cheat_index].size() - 1].LowerRightCorner.Y));
                    driver->draw2DRectangle(settingBarColor, core::rect<s32>(cheatSettingRects[i][cheat_index][0].UpperLeftCorner.X + setting_bar_padding,
                                                                             cheatSettingRects[i][cheat_index][0].UpperLeftCorner.Y + setting_bar_padding, 
                                                                             cheatSettingRects[i][cheat_index][cheatSettingRects[i][cheat_index].size() - 1].UpperLeftCorner.X + setting_bar_width,
                                                                             cheatSettingRects[i][cheat_index][cheatSettingRects[i][cheat_index].size() - 1].LowerRightCorner.Y - setting_bar_padding));
                    
                    for (size_t s = 0; s < script->m_cheat_categories[i]->m_cheats[cheat_index]->m_cheat_settings.size(); ++s) {
                        text_color = video::SColor(255, 255, 255, 255);
                        if (script->m_cheat_categories[i]->m_cheats[cheat_index]->m_cheat_settings[s]->m_type == "selectionbox") {
                            const std::string& settingName = script->m_cheat_categories[i]->m_cheats[cheat_index]->m_cheat_settings[s]->m_name;
                            std::wstring wSettingName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(settingName);

                            textSizeU32 = font->getDimension(wSettingName.c_str());
                            core::dimension2d<s32> textSize(textSizeU32.Width, textSizeU32.Height);

                            core::rect<s32> settingTextRect = core::rect<s32>(cheatSettingRects[i][cheat_index][s].UpperLeftCorner.X + (setting_bar_padding * 2) + setting_bar_width,
                                                                              cheatSettingRects[i][cheat_index][s].UpperLeftCorner.Y, 
                                                                              cheatSettingRects[i][cheat_index][s].LowerRightCorner.X,
                                                                              cheatSettingRects[i][cheat_index][s].UpperLeftCorner.Y + category_height);

                            s32 textX = settingTextRect.UpperLeftCorner.X + (settingTextRect.getWidth() - textSize.Width) / 2;
                            s32 textY = settingTextRect.UpperLeftCorner.Y + (settingTextRect.getHeight() - textSize.Height) / 2;


                            font->draw(wSettingName.c_str(), core::rect<s32>(textX, textY, textX + textSize.Width, textY + textSize.Height), text_color);
                        } else if (script->m_cheat_categories[i]->m_cheats[cheat_index]->m_cheat_settings[s]->m_type == "bool") {
                            if(g_settings->exists(script->m_cheat_categories[i]->m_cheats[cheat_index]->m_cheat_settings[s]->m_setting) && g_settings->getBool(script->m_cheat_categories[i]->m_cheats[cheat_index]->m_cheat_settings[s]->m_setting) == true) {
                                text_color = video::SColor(255, 62, 100, 219);
                            }
                            if(cheatSettingTextHovered[i][cheat_index][s] == true) {
                                if (g_settings->exists(script->m_cheat_categories[i]->m_cheats[cheat_index]->m_cheat_settings[s]->m_setting) && g_settings->getBool(script->m_cheat_categories[i]->m_cheats[cheat_index]->m_cheat_settings[s]->m_setting) == true) {
                                    text_color = video::SColor(255, 31, 50, 109);
                                } else {
                                    text_color = video::SColor(255, 127, 127, 127);
                                }
                            }
                            const std::string& settingName = script->m_cheat_categories[i]->m_cheats[cheat_index]->m_cheat_settings[s]->m_name;
                            std::wstring wSettingName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(settingName);

                            textSizeU32 = font->getDimension(wSettingName.c_str());
                            core::dimension2d<s32> textSize(textSizeU32.Width, textSizeU32.Height);

                            core::rect<s32> settingTextRect = core::rect<s32>(cheatSettingRects[i][cheat_index][s].UpperLeftCorner.X + (setting_bar_padding * 2) + setting_bar_width,
                                                                              cheatSettingRects[i][cheat_index][s].UpperLeftCorner.Y, 
                                                                              cheatSettingRects[i][cheat_index][s].LowerRightCorner.X,
                                                                              cheatSettingRects[i][cheat_index][s].LowerRightCorner.Y);

                            s32 textX = settingTextRect.UpperLeftCorner.X + (settingTextRect.getWidth() - textSize.Width) / 2;
                            s32 textY = settingTextRect.UpperLeftCorner.Y + (settingTextRect.getHeight() - textSize.Height) / 2;


                            font->draw(wSettingName.c_str(), core::rect<s32>(textX, textY, textX + textSize.Width, textY + textSize.Height), text_color);
                        }
                    }
                } else {
                    driver->draw2DLine(core::position2d<s32>(dropdown_center.X - (unit_size * 3), dropdown_center.Y - (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X, dropdown_center.Y + (unit_size * 1.5)), arrow_color);
                    driver->draw2DLine(core::position2d<s32>(dropdown_center.X - (unit_size * 3), 1 + dropdown_center.Y - (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X, 1 + dropdown_center.Y + (unit_size * 1.5)), arrow_color);
                    driver->draw2DLine(core::position2d<s32>(dropdown_center.X, dropdown_center.Y + (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X + (unit_size * 3), dropdown_center.Y - (unit_size * 1.5)), arrow_color);
                    driver->draw2DLine(core::position2d<s32>(dropdown_center.X, 1 + dropdown_center.Y + (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X + (unit_size * 3), 1 + dropdown_center.Y - (unit_size * 1.5)), arrow_color);
                }
                
            }
        }
    } else {
        driver->draw2DLine(core::position2d<s32>(dropdown_center.X - (unit_size * 3), dropdown_center.Y - (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X, dropdown_center.Y + (unit_size * 1.5)), arrow_color);
        driver->draw2DLine(core::position2d<s32>(dropdown_center.X - (unit_size * 3), 1 + dropdown_center.Y - (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X, 1 + dropdown_center.Y + (unit_size * 1.5)), arrow_color);
        driver->draw2DLine(core::position2d<s32>(dropdown_center.X, dropdown_center.Y + (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X + (unit_size * 3), dropdown_center.Y - (unit_size * 1.5)), arrow_color);
        driver->draw2DLine(core::position2d<s32>(dropdown_center.X, 1 + dropdown_center.Y + (unit_size * 1.5)), core::position2d<s32>(dropdown_center.X + (unit_size * 3), 1 + dropdown_center.Y - (unit_size * 1.5)), arrow_color);
    }
}

void NewMenu::draw() 
{
    if (m_client->isShutdown()) {
        return;
    }

    GET_SCRIPT_POINTER
    video::IVideoDriver* driver = Environment->getVideoDriver();
    gui::IGUIFont* font = g_fontengine->getFont(FONT_SIZE_UNSPECIFIED, FM_HD);
    if (g_settings->exists("use_menu_grid") && g_settings->getBool("use_menu_grid") == true) {
        if (isDragging) {
            for (int x = 0; x <= Environment->getVideoDriver()->getScreenSize().Width; x += category_height / 2) {
                driver->draw2DLine(core::position2d<s32>(x, 0), core::position2d<s32>(x, Environment->getVideoDriver()->getScreenSize().Height), video::SColor(50, 255, 255, 255));
            }

            for (int y = 0; y <= Environment->getVideoDriver()->getScreenSize().Height; y += category_height / 2) {
                driver->draw2DLine(core::position2d<s32>(0, y), core::position2d<s32>(Environment->getVideoDriver()->getScreenSize().Width, y), video::SColor(50, 255, 255, 255));
            }
        }
    }
    
    if (m_is_open) {
        for (size_t i = 0; i < script->m_cheat_categories.size(); i++) {
            drawCategory(driver, font, i);
        }
    }
} 
