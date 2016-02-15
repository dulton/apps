/*
 * MobiAqua Media Player
 *
 * Copyright (C) 2013-2014 Pawel Kolodziejski <aquadran at users.sourceforge.net>
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

#include <assert.h>

#include "basetypes.h"
#include "demuxer_base.h"
#include "demuxer_libav.h"

namespace MediaPLayer {

Demuxer::Demuxer() :
		_initialized(false) {
	assert(false);
}

Demuxer::~Demuxer() {
	assert(false);
}

Demuxer *CreateDemuxer(DEMUXER_TYPE demuxerType) {
	switch (demuxerType) {
	case DEMUXER_LIBAV:
		return new DemuxerLibAV();
	default:
		return NULL;
	}
}

} // namespace