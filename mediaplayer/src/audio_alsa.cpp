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

#include "audio_base.h"
#include "audio_alsa.h"

namespace MediaPLayer {

AudioAlsa::AudioAlsa() {
};

AudioAlsa::~AudioAlsa() {
	deinit();
};

STATUS AudioAlsa::init() {
	if (_initialized == true)
		return S_FAIL;

	return S_OK;
}

STATUS AudioAlsa::deinit() {
	if (_initialized == false)
		return S_FAIL;

	return S_OK;
}

} // namespace
