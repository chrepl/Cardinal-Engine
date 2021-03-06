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

/// \file       TextureLoader.cpp
/// \date       13/02/2018
/// \project    Cardinal Engine
/// \package    Runtime/Rendering/Texture
/// \author     Vincent STEHLY--CALISTO

#include "Glew/include/GL/glew.h"

#include "Runtime/Core/Debug/Logger.hpp"
#include "Runtime/Core/Assertion/Assert.hh"
#include "Runtime/Rendering/Texture/TextureLoader.hpp"
#include "Runtime/Rendering/Texture/TextureManager.hpp"

/// \namespace cardinal
namespace cardinal
{

/* static */ TextureLoader * TextureLoader::s_pInstance = nullptr;

/// \brief Default constructor
TextureLoader::TextureLoader()
: m_pDataBuffer(nullptr)
{
    // None
}

/// \brief Initializes the loader manager
///        Initializes the loader instance
/* static */ void TextureLoader::Initialize()
{
    if(TextureLoader::s_pInstance == nullptr)
    {
        TextureLoader::s_pInstance = new TextureLoader();
        TextureLoader::s_pInstance->m_pDataBuffer = new uchar[8192 * 8192];

        ASSERT_NOT_NULL(TextureLoader::s_pInstance->m_pDataBuffer);
        Logger::LogInfo("Texture loader successfully initialized");
    }
    else
    {
        Logger::LogWaring("The texture loader is already initialized");
    }
}

/// \brief Destroys the loader manager
///        Destroys the loader instance
/* static */ void TextureLoader::Shutdown()
{
    if(TextureLoader::s_pInstance != nullptr)
    {
        delete[] TextureLoader::s_pInstance->m_pDataBuffer;
        delete   TextureLoader::s_pInstance;

        TextureLoader::s_pInstance = nullptr;
        Logger::LogInfo("Texture loader successfully destroyed");
    }
    else
    {
        Logger::LogWaring("The texture loader is already destroyed");
    }
}

// TODO : Use IDs
// TODO : Class texture
/// \brief Loads a texture from a path and register it into the
/// \param key The key of the texture
/// \param path The path of the texture
/* static */ void TextureLoader::LoadTexture(std::string const& key, std::string const& path, bool nearest)
{
    ASSERT_NOT_NULL_MSG(TextureLoader::s_pInstance, "Is the manager initialized ?");
    std::string ext = std::string(path.begin() + path.find_first_of('.'), path.end());

    TextureImporter::TextureProperty properties {nullptr, 0, 0, 0, 0, 0, false};
    properties.pBuffer = TextureLoader::s_pInstance->m_pDataBuffer;

    if(ext == ".bmp")
    {
        if(!TextureImporter::ImportTexture_BMP(path.c_str(), properties))
        {
            Logger::LogError("Cannot load the texture referenced by the key %s.", key.c_str());
            return;
        }
    }
    else if(ext == ".dds")
    {
        if(!TextureImporter::ImportTexture_DDS(path.c_str(), properties))
        {
            Logger::LogError("Cannot load the texture referenced by the key %s.", key.c_str());
            return;
        }
    }
    else
    {
        Logger::LogError("Not a valid image format");
        return;
    }

    uint textureID = BindTexture(properties, nearest);
    TextureManager::Register(key, textureID);
}

// TODO : Use IDs
/// \brief Loads a bunch of textures from a list
/// \param keys Texture keys
/// \param paths Texture paths
/* static */ void TextureLoader::LoadTextures(
        std::vector<std::string> const& keys,
        std::vector<std::string> const& paths)
{
    ASSERT_EQ(keys.size(), paths.size());
    ASSERT_NOT_NULL_MSG(TextureLoader::s_pInstance, "Is the manager initialized ?");

    TextureImporter::TextureProperty properties {nullptr, 0, 0, 0, 0, 0, false};
    properties.pBuffer = TextureLoader::s_pInstance->m_pDataBuffer;

    size_t pathSize = paths.size();
    for(size_t  nPath = 0; nPath < pathSize; ++nPath)
    {
        if(!TextureImporter::ImportTexture_BMP(paths[nPath].c_str(), properties))
        {
            Logger::LogError("Cannot load the texture referenced by the key %s.", keys[nPath].c_str());
            return;
        }

        properties.width  = 0;
        properties.height = 0;

        uint textureID = BindTexture(properties);
        TextureManager::Register(keys[nPath], textureID);
    }
}

/// \brief Generates mip mapping from textures
/// \param key The key of the texture
/// \param path The path of the textures
void TextureLoader::LoadMipMapTextures(std::string const& key, std::vector<std::string> paths)
{
    ASSERT_NE(key, std::string(""));
    ASSERT_GT(paths.size(), 1);
    ASSERT_NOT_NULL_MSG(TextureLoader::s_pInstance, "Is the manager initialized ?");

    size_t pathSize = paths.size();
    std::vector<TextureImporter::TextureProperty> properties;
    for(size_t nPath = 0; nPath < pathSize; ++nPath)
    {
        properties.push_back({new uchar[8192 * 8192], 0, 0, 0, 0, 0, false});

        if(!TextureImporter::ImportTexture_BMP(paths[nPath].c_str(), properties.back()))
        {
            Logger::LogError("Cannot load the texture referenced by the key %s.", key.c_str());
            return;
        }
    }

    // Generates mip mapping
    uint textureID = BindTexture(properties);
    TextureManager::Register(key, textureID);

    // Free memory
    for(TextureImporter::TextureProperty & current : properties)
    {
        delete[] current.pBuffer;
    }
}

/// \brief Binds the texture and load into OpenGL
/// \param property Properties of the texture
/// \return The ID of the texture
/* static */ uint TextureLoader::BindTexture(TextureImporter::TextureProperty const& property, bool nearest)
{
    ASSERT_NOT_NULL(property.pBuffer);

    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if(property.alpha)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        unsigned int blockSize = property.blockSize;
        unsigned int offset    = 0;

        uint width  = property.width;
        uint height = property.height;

        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, property.format, width, height, 0, width * height, property.pBuffer);
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

        if(nearest)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.6f);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 4);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, property.width, property.height, 0, GL_BGR, GL_UNSIGNED_BYTE, property.pBuffer);
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

        if(nearest)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.6f);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    ASSERT_NE(textureID, 0);
    return textureID;
}

/// \brief Binds the texture and load into OpenGL
/// \param prop The vector of properties
/// \return The ID of the texture
/* static */ uint TextureLoader::BindTexture(std::vector<TextureImporter::TextureProperty> const& prop)
{
    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,  prop.size() - 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.6f);

    size_t propCount = prop.size();
    for(size_t i = 0; i < propCount; ++i)
    {
        glTexImage2D(GL_TEXTURE_2D, (int)i, GL_RGB, prop[i].width, prop[i].height, 0, GL_BGR, GL_UNSIGNED_BYTE, prop[i].pBuffer);
    }

    return textureID;
}

} // !namespace
