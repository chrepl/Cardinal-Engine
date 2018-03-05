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

/// \file       GodRay.hpp
/// \date       04/03/2018
/// \project    Cardinal Engine
/// \package    Runtime/Rendering/PostProcessing/PostEffects
/// \author     Vincent STEHLY--CALISTO

#ifndef CARDINAL_ENGINE_GOD_RAY_HPP__
#define CARDINAL_ENGINE_GOD_RAY_HPP__

#include "Runtime/Rendering/PostProcessing/PostEffects/PostEffect.hpp"

/// \namespace cardinal
namespace cardinal
{

/// \class GodRay
/// \brief Volumetric light beam
class GodRay : public PostEffect
{
public:

    // None

private:

    friend class PostProcessingStack;

    /// \brief Constructor
    GodRay();

    /// \brief Destructor
    ~GodRay();

    /// \brief Applies the effect from the given textures
    /// \param colorTexture The color texture
    /// \param depthTexture The depth buffer texture
    /// \param lightScatteringTexture The result of the light scattering pass
    void ApplyEffect(uint colorTexture, uint depthTexture, uint lightScatteringTexture) final;

    /// \brief Called to draw the GUI
    void OnGUI() final;

private:

    float m_decay;
    float m_weight;
    float m_density;
    float m_exposure;
    int   m_sampleCount;

    int m_decayID;
    int m_weightID;
    int m_densityID;
    int m_exposureID;
    int m_sampleCountID;
    int m_lightPosition2DID;
    int m_lightScatteringTextureID;
};

} // !namespace

#endif // !CARDINAL_ENGINE_GOD_RAY_HPP__