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

/// \file       RenderingEngine.cpp
/// \date       11/02/2018
/// \project    Cardinal Engine
/// \package    Rendering
/// \author     Vincent STEHLY--CALISTO

#include <chrono>
#include <iostream>
#include <Header/Runtime/Core/Assertion/Assert.hh>

#include "Glew/include/GL/glew.h"

#include "Runtime/Core/Debug/Logger.hpp"
#include "Runtime/Rendering/Shader/IShader.hpp"
#include "Runtime/Rendering/RenderingEngine.hpp"
#include "Runtime/Rendering/Debug/DebugManager.hpp"
#include "Runtime/Rendering/Shader/ShaderManager.hpp"
#include "Runtime/Rendering/Shader/ShaderCompiler.hpp"
#include "Runtime/Rendering/Renderer/MeshRenderer.hpp"
#include "Runtime/Rendering/Renderer/TextRenderer.hpp"
#include "Runtime/Rendering/Texture/TextureLoader.hpp"
#include "Runtime/Rendering/Texture/TextureManager.hpp"

/// \namespace cardinal
namespace cardinal
{

/* static */ RenderingEngine * RenderingEngine::s_pInstance = nullptr;

/// Initializes the rendering engine from parameters
/// \param width The width of the window
/// \param height The height of the window
/// \param szTitle The title of the window
/// \param fps The fps limit
/// \param bInterpolate Should the engine interpolate frame ?
bool RenderingEngine::Initialize(int width, int height, const char * szTitle, float fps, bool bInterpolate)
{
    Logger::LogInfo("Initializing the Rendering Engine ...");
    Logger::LogInfo("Initializing OpenGL context ...");
    m_window.Initialize(width, height, szTitle);

    if(m_window.GetContext() == nullptr)
    {
        Logger::LogError("Failed to initialize the rendering engine. Aborting.");
        return false;
    }

    // Texture initializes
    TextureManager::Initialize();
    TextureLoader::Initialize();

    // IShader initializes
    ShaderManager::Initialize();

    // Loads Textures
    TextureLoader::LoadTexture("SAORegular", "Resources/Textures/SAORegular.bmp");
    TextureLoader::LoadTexture("Block",      "Resources/Textures/BlockAtlas.bmp");

    // Loads shaders
    ShaderManager::Register("UnlitTexture", ShaderCompiler::LoadShaders(
            "Resources/Shaders/Unlit/UnlitTransparentVertexShader.glsl",
            "Resources/Shaders/Unlit/UnlitTransparentFragmentShader.glsl"));

    ShaderManager::Register("UnlitColor", ShaderCompiler::LoadShaders(
            "Resources/Shaders/Unlit/UnlitColorVertexShader.glsl",
            "Resources/Shaders/Unlit/UnlitColorFragmentShader.glsl"));

    ShaderManager::Register("UnlitTransparent", ShaderCompiler::LoadShaders(
            "Resources/Shaders/Unlit/UnlitTransparentVertexShader.glsl",
            "Resources/Shaders/Unlit/UnlitTransparentFragmentShader.glsl"));

    ShaderManager::Register("Text", ShaderCompiler::LoadShaders(
            "Resources/Shaders/Text/TextVertexShader.glsl",
            "Resources/Shaders/Text/TextFragmentShader.glsl"));

    // Debug
    DebugManager::Initialize();

    // Configures OpenGL pipeline
    glEnable   (GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable   (GL_CULL_FACE);
    glCullFace (GL_BACK);
    glFrontFace(GL_CW);

    // TODO : Makes clear color configurable
    glClearColor(0.0f, 0.709f, 0.866f, 1.0f);

    // TODO : Removes magic values
    m_projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 10000.0f);

    m_frameDelta   = 1.0 / fps;
    m_frameTime    = 0.0;
    m_frameLag     = 0.0,
    m_currentTime  = 0.0;
    m_previousTime = 0.0;
    m_elapsedTime  = 0.0;
    m_frameCount   = 0;
    m_currentFps   = 0;
    m_fpsCounter   = 0;
    s_pInstance    = this;

    m_pEngineName = AllocateTextRenderer();
    m_pCurrentFPS = AllocateTextRenderer();
    m_pTotalFPS   = AllocateTextRenderer();
    m_pFrameTime  = AllocateTextRenderer();

    m_pEngineName->Initialize();
    m_pCurrentFPS->Initialize();
    m_pTotalFPS->Initialize();
    m_pFrameTime->Initialize();

    m_pEngineName->SetText("Cardinal Engine v1.0", 5, 580, 14, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_pCurrentFPS->SetText("FPS   : 0", 5, 560, 12, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    m_pTotalFPS->SetText  ("Frame : 0", 5, 545, 12, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_pFrameTime->SetText ("Time  : 0", 5, 530, 12, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    Logger::LogInfo("Rendering engine successfully initialized in %3.4lf s.", glfwGetTime());
}

/// \brief Renders the frame
/// \param step The progression into the current frame
void RenderingEngine::Render(float step)
{
    // Getting the current elapsed time
    m_currentTime = glfwGetTime();

    double elapsed = m_currentTime - m_previousTime;

    m_frameLag    += elapsed;
    m_elapsedTime += elapsed;
    m_previousTime = m_currentTime;

    if(m_elapsedTime >= 1.0f)
    {
        m_currentFps  = m_fpsCounter / m_elapsedTime;
        m_elapsedTime = 0.0f;
        m_fpsCounter  = 0;

        // Logger::LogInfo("%3.3lf FPS", m_currentFps);

        std::string _fps = "FPS   : " + std::to_string(m_currentFps);
        m_pCurrentFPS->SetText(_fps.c_str(), 5, 560, 12, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    }

    if(m_frameLag < m_frameDelta)
    {
        return;
    }

    // Keeping the frame time overflow
    // for the next frame
    m_frameLag    -= m_frameDelta;

    // Starts instrumentation on frame
    double beginFrame = glfwGetTime();

    RenderFrame(step);

    // Computes the total frame time
    m_frameTime = glfwGetTime() - beginFrame;

    // We rendered a new frame, inc. the counter
    m_frameCount++;
    m_fpsCounter++;

    std::string _frame = "Frame : " + std::to_string(m_frameCount);
    std::string _time  = "Time  : " + std::to_string(m_frameTime) + " s";
    m_pTotalFPS->SetText  (_frame.c_str(), 5, 545, 12, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_pFrameTime->SetText (_time.c_str(),  5, 530, 12, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

/// \brief Frame rendering implementation
/// \param step The normalized progression in the frame
void RenderingEngine::RenderFrame(float step)
{
    // Clears buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Gets the projection matrix
    glm::mat4 Projection  = m_projectionMatrix;
    glm::mat4 View        = m_pCamera->GetViewMatrix();
    glm::mat4 ProjectView = Projection * View;

    // Draw meshes
    size_t rendererCount = m_meshRenderer.size();
    for(int nRenderer = 0; nRenderer < rendererCount; ++nRenderer)
    {
        // Buffers the renderer
        MeshRenderer * pRenderer = m_meshRenderer[nRenderer];
        IShader      * pShader   = pRenderer->m_pShader;

        pShader->Begin(ProjectView * pRenderer->m_model);

        // Render it
        glBindVertexArray(pRenderer->m_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, pRenderer->m_elementsCount, GL_UNSIGNED_SHORT, nullptr);

        pShader->End();
    }

    // Draw Texts
    rendererCount = m_textRenderer.size();
    for(int nRenderer = 0; nRenderer < rendererCount; ++nRenderer)
    {
        // Buffers the renderer
        TextRenderer * pRenderer = m_textRenderer[nRenderer];
        IShader      * pShader   = pRenderer->m_pShader;

        pShader->Begin(ProjectView * glm::mat4(1.0f));

        // Render it
        glBindVertexArray(pRenderer->m_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_TRIANGLES, 0, pRenderer->m_vertexCount);

        pShader->End();
    }

    // Cleanup
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);

#ifdef CARDINAL_DEBUG
   DebugManager::Draw(ProjectView);
#endif

#ifdef CARDINAL_DEBUG
    DebugManager::Clear();
#endif

    //  Display
    glfwSwapBuffers(m_window.GetContext());
}

/// \brief Sets the current camera
void RenderingEngine::SetCamera(Camera * pCamera)
{
    m_pCamera = pCamera;
}

/// \brief Shutdow the engine
void RenderingEngine::Shutdow()
{
    // TODO
    TextureManager::Shutdown();
}

/// \brief Returns a pointer on the window
Window * RenderingEngine::GetWindow()
{
    return &m_window;
}

/// \brief TMP
glm::mat4 const &RenderingEngine::GetProjectionMatrix()
{
    return m_projectionMatrix;
}

/// \brief Allocates and return a pointer on a renderer
///        Registers the renderer into the engine
/* static */ MeshRenderer * RenderingEngine::AllocateMeshRenderer()
{
    ASSERT_NOT_NULL(RenderingEngine::s_pInstance);

    MeshRenderer * pRenderer = new MeshRenderer(); // NOLINT
    RenderingEngine::s_pInstance->m_meshRenderer.push_back(pRenderer);

    return pRenderer;
}

/// \brief Deallocates a renderer
///        Unregisters the renderer
/* static */ void RenderingEngine::ReleaseMeshRenderer(MeshRenderer *& pRenderer)
{
    ASSERT_NOT_NULL(RenderingEngine::s_pInstance);

    int index = -1;
    size_t count = RenderingEngine::s_pInstance->m_meshRenderer.size();
    for(size_t nRenderer = 0; nRenderer < count; ++nRenderer)
    {
        if(RenderingEngine::s_pInstance->m_meshRenderer[nRenderer] == pRenderer)
        {
            index = static_cast<int>(nRenderer);
            break;
        }
    }

    if(index != -1)
    {
        RenderingEngine::s_pInstance->m_meshRenderer.erase(
                RenderingEngine::s_pInstance->m_meshRenderer.begin() + index);
    }

    delete pRenderer;
    pRenderer = nullptr;
}

/// \brief Allocates and return a pointer on a renderer
///        Registers the renderer into the engine
TextRenderer *RenderingEngine::AllocateTextRenderer()
{
    ASSERT_NOT_NULL(RenderingEngine::s_pInstance);

    TextRenderer * pRenderer = new TextRenderer(); // NOLINT
    RenderingEngine::s_pInstance->m_textRenderer.push_back(pRenderer);

    return pRenderer;
}

/// \brief Deallocates a renderer
///        Unregisters the renderer
void RenderingEngine::ReleaseTextRenderer(TextRenderer *&pRenderer)
{
    ASSERT_NOT_NULL(RenderingEngine::s_pInstance);

    int index = -1;
    size_t count = RenderingEngine::s_pInstance->m_meshRenderer.size();
    for(size_t nRenderer = 0; nRenderer < count; ++nRenderer)
    {
        if(RenderingEngine::s_pInstance->m_textRenderer[nRenderer] == pRenderer)
        {
            index = static_cast<int>(nRenderer);
            break;
        }
    }

    if(index != -1)
    {
        RenderingEngine::s_pInstance->m_textRenderer.erase(
                RenderingEngine::s_pInstance->m_textRenderer.begin() + index);
    }

    delete pRenderer;
    pRenderer = nullptr;
}

} // !namespace