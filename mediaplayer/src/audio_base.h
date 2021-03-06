/*
 * MobiAqua Media Player
 *
 * Copyright (C) 2014-2016 Pawel Kolodziejski <aquadran at users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef AUDIO_BASE_H
#define AUDIO_BASE_H

#include "basetypes.h"
#include "avtypes.h"

namespace MediaPLayer {

class Audio {
protected:

	bool _initialized;

public:

	Audio();
	virtual ~Audio() {}
	virtual STATUS init() = 0;
	virtual STATUS deinit() = 0;
};

Audio *CreateAudio(AUDIO_TYPE audioType);

} // namespace

#endif
