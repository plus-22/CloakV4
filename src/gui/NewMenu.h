/*
Copyright (C) 2020 Maintainer_(Ivan Shkatov) <ivanskatov672@gmail.com>
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#ifndef NEWMENU_H
#define NEWMENU_H

#include <irrlicht.h>
#include "gui/modalMenu.h"
#include <vector>
#include "script/scripting_client.h"
#include "client/client.h"
#include "client/fontengine.h"
#include <codecvt> 
#include <locale> 
#include "log.h"

class CustomEditBox;
using namespace irr;
using namespace gui;

#define GET_SCRIPT_POINTER                                                     \
    ClientScripting *script = m_client->getScript();                         \
    if (!script || !script->m_cheats_loaded)                                 \
        return;

#define GET_SCRIPT_POINTER_BOOL                                                     \
    ClientScripting *script = m_client->getScript();                         \
    if (!script || !script->m_cheats_loaded)                                 \
        return true;     

class NewMenu: public IGUIElement
{
public:
    NewMenu(gui::IGUIEnvironment* env, gui::IGUIElement* parent, s32 id, IMenuManager* menumgr, Client *client);

    void create();
    void close();

    virtual bool OnEvent(const irr::SEvent& event);
    virtual void draw() override;

    bool isOpen() { return m_is_open; }
    
    ~NewMenu();

private:
    core::rect<s32> createRect(s32 x, s32 y);
    std::vector<core::rect<s32>> categoryRects;
    core::vector2d<s32> offset; 
    IMenuManager* m_menumgr; 
    bool isDragging;
    core::vector2d<s32> rectPosition; 
    const int rectWidth = 175; 
    const int rectHeight = 35;
    bool m_is_open = false; 
    int draggedRectIndex;
    std::vector<bool> selectedCategory;
    std::vector<std::vector<core::rect<s32>>> subCategoryRects;
    std::vector<std::vector<video::SColor>> subCategoryColors;
    Client* m_client;
    bool m_rectsCreated = false;
    core::vector2d<s32> lastMousePos;
    core::rect<s32> form;
    gui::IGUIEnvironment* env;
    CustomEditBox *customEditBox = nullptr;
    video::SColor outlineColor = video::SColor(255, 255, 255, 255);
};

#endif 


using namespace irr;
using namespace core;
using namespace video;
using namespace gui;
using namespace io;
using namespace scene;

class CustomEditBox {
public:
    CustomEditBox(IGUIEnvironment* guienv, const wchar_t* name, std::vector<std::wstring>& texts, rect<s32> position)
        : texts(texts) {
        editBox = guienv->addEditBox(name, rect<s32>(0, 0, 200, 30));
        editBox->setRelativePosition(position);
    }

    void handleInput(const SEvent& event) {
        if (event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown) {
            if (event.KeyInput.Key == KEY_RETURN) {
                std::wstring text = editBox->getText();
            }
        }
    }

    void updatePosition(rect<s32> newPosition) {
        editBox->setRelativePosition(newPosition);
    }

    void moveEditBox(s32 deltaX, s32 deltaY) {
        rect<s32> currentPosition = editBox->getRelativePosition();
        currentPosition.UpperLeftCorner.X += deltaX;
        currentPosition.UpperLeftCorner.Y += deltaY;
        currentPosition.LowerRightCorner.X += deltaX;
        currentPosition.LowerRightCorner.Y += deltaY;
        updatePosition(currentPosition);
    }

    void Event(const SEvent& event, bool& dragging, vector2d<s32>& lastMousePos, rect<s32>& rectangle) {
        if (!rectangle.isPointInside(vector2d<s32>(event.MouseInput.X, event.MouseInput.Y))) {
            return; 
        }

        if (event.EventType == EET_MOUSE_INPUT_EVENT) {
            switch (event.MouseInput.Event) {
                case EMIE_LMOUSE_PRESSED_DOWN:
                    dragging = true;
                    lastMousePos = vector2d<s32>(event.MouseInput.X, event.MouseInput.Y);
                    break;

                case EMIE_LMOUSE_LEFT_UP:
                    dragging = false;
                    break;

                case EMIE_MOUSE_MOVED:
                    if (dragging) {
                        s32 deltaX = event.MouseInput.X - lastMousePos.X;
                        s32 deltaY = event.MouseInput.Y - lastMousePos.Y;
                        rectangle.UpperLeftCorner.X += deltaX;
                        rectangle.UpperLeftCorner.Y += deltaY;
                        rectangle.LowerRightCorner.X += deltaX;
                        rectangle.LowerRightCorner.Y += deltaY;

                        moveEditBox(deltaX, deltaY);

                        lastMousePos = vector2d<s32>(event.MouseInput.X, event.MouseInput.Y);
                    }
                    break;

                default:
                    break;
            }
        }
    }


    void open() {
        editBox->setVisible(true);
    }
    void close() {
        editBox->setVisible(false);
    }


private:
    IGUIEditBox* editBox;
    std::vector<std::wstring>& texts; 
};