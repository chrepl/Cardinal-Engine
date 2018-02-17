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

/// \file       MeshRenderer.hpp
/// \date       11/02/2018
/// \project    Cardinal Engine
/// \package    Rendering/Renderer
/// \author     Vincent STEHLY--CALISTO

#ifndef CARDINAL_ENGINE_TEXT_RENDERER_HPP__
#define CARDINAL_ENGINE_TEXT_RENDERER_HPP__

#include <vector>
#include "Glm/glm/glm.hpp"
#include "Glm/glm/ext.hpp"

#include "Runtime/Rendering/Shader/IShader.hpp"
#include "Runtime/Platform/Configuration/Configuration.hh"

/// \namespace cardinal
namespace cardinal
{

/// \class  TextRenderer
/// \brief  Renderer for 2D text
class TextRenderer
{
public :

    /// \brief Default constructor
    TextRenderer();

    /// \brief Destructor
    ~TextRenderer();

    /// \brief Initializes the text renderer
    void Initialize();

    /// \brief Updates the text
    /// \param szText The text to display
    /// \param x The x coordinate (screen space)
    /// \param y The y coordinate (screen space)
    /// \param size The size of the text
    void SetText(const char * szText, int x, int y, int size);

    /// \brief Sets the renderer shader
    /// \param pShader The pointer on the shader
    void SetShader(IShader * pShader);

private:

    friend class RenderingEngine;

    uint      m_vao;
    uint      m_texture;
    uint      m_vertexbject;
    uint      m_uvsObject;
     int      m_textureSampler;
    IShader * m_pShader;
    uint      m_vertexCount;
};

} // !namespace

#endif // !CARDINAL_ENGINE_TEXT_RENDERER_HPP__