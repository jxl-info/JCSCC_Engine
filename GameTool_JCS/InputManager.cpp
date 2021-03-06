﻿/**
 * $File: InputManager.cpp $
 * $Date: $
 * $Revision: $
 * $Creator: Jen-Chieh Shen $
 * $Notice: See LICENSE.txt for modification and distribution information
 *                   Copyright (c) 2015 by Shen, Jen-Chieh $
 */

#include "InputManager.h"


namespace JCS_GameTool
{
    InputManager::InputManager()
        : m_pKeyboardClient(nullptr)
        , m_pMouseClient(nullptr)
        , m_pInput(nullptr)
    {
        
    }

    InputManager::~InputManager()
    {
        SafeDeleteObject(m_pKeyboardClient);
        SafeDeleteObject(m_pMouseClient);
    }

    /**
    * 相關解釋
    * http://stackoverflow.com/questions/22146094/why-should-i-use-a-pointer-rather-than-the-object-itself
    */
    void InputManager::setKeyboard(KeyboardServer* kServer)
    {
        SafeDeleteObject(m_pKeyboardClient);
        if (m_pKeyboardClient == nullptr && kServer != nullptr)
            m_pKeyboardClient = new JCS_GameInput::KeyboardClient(kServer);
    }
    void InputManager::setMouse(const MouseServer* mServer)
    {
        SafeDeleteObject(m_pMouseClient);
        if (m_pMouseClient == nullptr && mServer != nullptr)
            m_pMouseClient = new JCS_GameInput::MouseClient(mServer);
    }

}// end namespace "JCS_GameTool"

