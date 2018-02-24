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

/// \file       DebugGizmos.hpp
/// \date       20/02/2018
/// \project    Cardinal Engine
/// \package    Core/Rendering/Debug
/// \author     Vincent STEHLY--CALISTO

#ifndef CARDINAL_ENGINE_DEBUG_GIZMOS_HPP__
#define CARDINAL_ENGINE_DEBUG_GIZMOS_HPP__

#include "Glm/glm/glm.hpp"

/// \namespace cardinal
namespace cardinal
{

namespace debug
{

/// \brief Draw a light bulbe with lines
/// \param position The start point of the line
/// \param color The color of the gizmo
/// \param scale The end point of the line
void DrawLight(glm::vec3 const& position, glm::vec3 const& color, float scale);

}  // !namespace

} // !namespace

#endif // !CARDINAL_ENGINE_DEBUG_GIZMOS_HPP__