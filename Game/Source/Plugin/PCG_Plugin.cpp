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

/// \file       PCG_Plugin.cpp
/// \date       26/02/2018
/// \project    Cardinal Engine
/// \package    Plugin
/// \author     Vincent STEHLY--CALISTO

// Engine
#include "Runtime/Rendering/Lighting/Lighting.hh"
#include "Runtime/Rendering/PostProcessing/PostEffects/Mirror.hpp"

#include "ImGUI/imgui.h"

// Game
#include "Plugin/PCG_Plugin.hpp"
#include "World/Generator/BasicWorldGenerator.hpp"

/* static */ PCG_Plugin * PCG_Plugin::s_pPlugin = nullptr;

/// \brief Hook to register user plugin from the static libraries
void OnPluginRegistration()
{
    ASSERT_NULL(PCG_Plugin::s_pPlugin);

    // Creating a new pcg plugin instance
    PCG_Plugin::s_pPlugin = new PCG_Plugin();

    // Finally registering the plugin into the engine
    cardinal::PluginManager::RegisterPlugin(PCG_Plugin::s_pPlugin);
}

/// \brief Constructor
PCG_Plugin::PCG_Plugin() : m_generatorGui(), m_worldGenerator()
{
    m_pWorld = nullptr;
}

/// \brief Called when the game begins
void PCG_Plugin::OnPlayStart()
{
    // Configure engine here
    // Lighting
    cardinal::LightManager::CreateDirectionalLight();
    cardinal::LightManager::GetDirectionalLight()->SetPosition(glm::vec3(520.0f, 20.0f, 307.0f));
    cardinal::LightManager::GetDirectionalLight()->SetDirection(glm::vec3(-0.5f, -0.5f, -0.5f));

    // Gizmos
    cardinal::DebugManager::EnableGizmo(cardinal::DebugManager::EGizmo::DirectionalLight);

    // Post-processing
    cardinal::RenderingEngine::SetPostProcessingActive(true);
    cardinal::RenderingEngine::IsPostProcessingActive();

    cardinal::PostProcessingStack * pPostProcessingStack = cardinal::RenderingEngine::GetPostProcessingStack();
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::Identity,      false);
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::Mirror,        false);
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::Negative,      false);
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::Sepia,         false);
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::BoxBlur,       false);
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::GaussianBlur,  false);
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::Sharpen,       false);
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::EdgeDetection, false);
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::DepthBuffer ,  false);
    pPostProcessingStack->SetEffectActive(cardinal::PostEffect::EType::GodRay ,       false);

    // auto * pMirror   = (cardinal::Mirror   *)pPostProcessingStack->GetPostEffect(cardinal::PostEffect::EType::Mirror);
    // auto * pNegative = (cardinal::Negative *)pPostProcessingStack->GetPostEffect(cardinal::PostEffect::EType::Negative);

    // Setting up the game
    m_pWorld = m_worldGenerator.generateWorld();

    // Camera and Character
    m_cameraManager.SetCamera(cardinal::RenderingEngine::GetMainCamera());
    m_cameraManager.SetCharacter(&m_character);
}

/// \brief Called when the game stops
void PCG_Plugin::OnPlayStop()
{
}

/// \brief Called just before the engine update
void PCG_Plugin::OnPreUpdate()
{
    // TODO
}

/// \brief Called after the engine update
/// \param dt The elapsed time in seconds
void PCG_Plugin::OnPostUpdate(float dt)
{
    m_character.Update(cardinal::RenderingEngine::GetWindow(), dt);
    m_cameraManager.Update(cardinal::RenderingEngine::GetWindow(), dt);
}

/// \brief Called when it's time to render the GUI
void PCG_Plugin::OnGUI()
{
    static bool a = true;
    //ImGui::ShowDemoWindow(&a);
    m_generatorGui.setGenerator(&m_worldGenerator);
    m_generatorGui.drawGUI(&a);
    // TODO
}
