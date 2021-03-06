# Copyright (C) 2018-2019 Cardinal Engine
# Vincent STEHLY--CALISTO, vincentstehly@hotmail.fr

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# he Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

# Adds all runtime header
INCLUDE_DIRECTORIES(
        ${CARDINAL_GAME_DIR}/Header/
        ${CARDINAL_ENGINE_DIR}/Header/
        ${CARDINAL_THIRD_PARTY_DIR}/ImGUI/Header/
        ${CARDINAL_THIRD_PARTY_DIR}/Bullet3/src/)

# Gets all source files
FILE(GLOB_RECURSE GAME_SOURCES
        World/*.cpp
        City/*.cpp
        Character/*.cpp
        Rendering/*.cpp)

FILE(GLOB_RECURSE GAME_HEADERS
        ${CARDINAL_GAME_DIR}/Header/World/*.hpp
        ${CARDINAL_GAME_DIR}/Header/City/*.hpp
        ${CARDINAL_GAME_DIR}/Header/Character/*.hpp
        ${CARDINAL_GAME_DIR}/Header/Physics/*.hpp
        ${CARDINAL_GAME_DIR}/Header/Rendering/*.hpp)

# Linking with the engine
# Creating empty targets
ADD_EXECUTABLE(Main_Plugin    "Plugin/Main_Plugin.cpp")
ADD_EXECUTABLE(PCG_Plugin     "Plugin/PCG_Plugin.cpp")
ADD_EXECUTABLE(PCGCity_Plugin "Plugin/PCGCity_Plugin.cpp")
ADD_EXECUTABLE(VR_Plugin      "Plugin/VR_Plugin.cpp")
ADD_EXECUTABLE(Demo_Plugin    "Plugin/Demo_Plugin.cpp")

# Creating a librariy for PCG and Main sources
ADD_LIBRARY(PCGLib        STATIC Plugin/PCG_Plugin.cpp     ${GAME_SOURCES} ${GAME_HEADERS})
ADD_LIBRARY(MainLib       STATIC Plugin/Main_Plugin.cpp    ${GAME_SOURCES} ${GAME_HEADERS})
ADD_LIBRARY(PCGCityLib    STATIC Plugin/PCGCity_Plugin.cpp ${GAME_SOURCES} ${GAME_HEADERS})
ADD_LIBRARY(VRPLuginLib   STATIC Plugin/VR_Plugin.cpp      ${GAME_SOURCES} ${GAME_HEADERS})
ADD_LIBRARY(DemoPluginLib STATIC Plugin/Demo_Plugin.cpp    ${GAME_SOURCES} ${GAME_HEADERS})

# Add dependencies
ADD_DEPENDENCIES(PCGLib        CardinalEngine BulletDynamics BulletCollision LinearMath)
ADD_DEPENDENCIES(MainLib       CardinalEngine BulletDynamics BulletCollision LinearMath)
ADD_DEPENDENCIES(PCGCityLib    CardinalEngine BulletDynamics BulletCollision LinearMath)
ADD_DEPENDENCIES(VRPLuginLib   CardinalEngine BulletDynamics BulletCollision LinearMath)
ADD_DEPENDENCIES(DemoPluginLib CardinalEngine BulletDynamics BulletCollision LinearMath)

# Linking dependencies
TARGET_LINK_LIBRARIES(PCGLib        CardinalEngine ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)
TARGET_LINK_LIBRARIES(MainLib       CardinalEngine ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)
TARGET_LINK_LIBRARIES(PCGCityLib    CardinalEngine ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)
TARGET_LINK_LIBRARIES(VRPLuginLib   CardinalEngine ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)
TARGET_LINK_LIBRARIES(DemoPluginLib CardinalEngine ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)

TARGET_LINK_LIBRARIES(Main_Plugin     CardinalEngine MainLib       ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)
TARGET_LINK_LIBRARIES(PCG_Plugin      CardinalEngine PCGLib        ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)
TARGET_LINK_LIBRARIES(PCGCity_Plugin  CardinalEngine PCGCityLib    ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)
TARGET_LINK_LIBRARIES(VR_Plugin       CardinalEngine VRPLuginLib   ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)
TARGET_LINK_LIBRARIES(Demo_Plugin     CardinalEngine DemoPluginLib ${COMPILER_DEPENDENCIES} ImGUI BulletDynamics BulletCollision LinearMath)