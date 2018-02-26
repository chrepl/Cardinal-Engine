/// Copyright (C) 2018-2019, Cardinal Engine
/// Vincent STEHLY--CALISTO, vincentstehly@hotmail.fr
///
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License along
/// with this program; if not, write to the Free Software Foundation, Inc.,
/// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

/// \file       Main_Plugin.cpp
/// \date       26/02/2018
/// \project    Cardinal Engine
/// \package    Plugin
/// \author     Vincent STEHLY--CALISTO

#include "Plugin/Main_Plugin.hpp"
#include "World/Generator/BasicWorldGenerator.hpp"

/* static */ Main_Plugin * Main_Plugin::s_pPlugin = nullptr;

/// \brief Hook to register user plugin from the static libraries
void OnPluginRegistration()
{
    ASSERT_NULL(Main_Plugin::s_pPlugin);

    // Creating a new main plugin instance
    Main_Plugin::s_pPlugin = new Main_Plugin();

    // Finally registering the plugin into the engine
    cardinal::PluginManager::RegisterPlugin(Main_Plugin::s_pPlugin);
}

/// \brief Constructor
Main_Plugin::Main_Plugin()
{
    m_pWorld = nullptr;
}

/// \brief Called when the game begins
void Main_Plugin::OnPlayStart()
{
    // Configure engine here
    cardinal::DebugManager::EnableGizmo(cardinal::DebugManager::EGizmo::Grid);
    cardinal::DebugManager::EnableGizmo(cardinal::DebugManager::EGizmo::Axis);

    // Setting up the game
    BasicWorldGenerator bwg;
    m_pWorld = bwg.generateWorld();
    m_character.AttachCamera(cardinal::RenderingEngine::GetMainCamera());
}

/// \brief Called when the game stops
void Main_Plugin::OnPlayStop()
{
    // TODO
}

/// \brief Called just before the engine update
void Main_Plugin::OnPreUpdate()
{
    // TODO
}

/// \brief Called after the engine update
/// \param dt The elapsed time in seconds
void Main_Plugin::OnPostUpdate(float dt)
{
    m_character.Update(cardinal::RenderingEngine::GetWindow(), dt);
}


