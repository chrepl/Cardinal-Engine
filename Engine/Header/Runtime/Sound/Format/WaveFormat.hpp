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

/// \file       WaveFormat.hpp
/// \date       27/02/2018
/// \project    Cardinal Engine
/// \package    Runtime/Sound/Format
/// \author     Vincent STEHLY--CALISTO

#ifndef CARDINAL_ENGINE_WAVE_FORMAT_HPP__
#define CARDINAL_ENGINE_WAVE_FORMAT_HPP__

/// \namespace cardinal
namespace cardinal
{

/// \namespace wave
namespace wave
{
    /// \struct Header
    struct Header
    {
        char chunkID[4];
        long chunkSize;
        char format[4];
    };

    /// \struct Format
    struct Format
    {
        char  subChunkID[4];
        long  subChunkSize;
        short audioFormat;
        short numChannels;
        long  sampleRate;
        long  byteRate;
        short blockAlign;
        short bitsPerSample;
    };

    /// \struct Data
    struct Data
    {
        char subChunkID[4];
        long subChunk2Size;
    };

} // !namespace

} // !namespace

#endif // !CARDINAL_ENGINE_WAVE_FORMAT_HPP__