/*
 * MobiAqua Media Player
 *
 * Copyright (C) 2013-2016 Pawel Kolodziejski <aquadran at users.sourceforge.net>
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

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h>

#include "display_base.h"
#include "display_fbdev.h"
#include "yuv420.h"
#include "logs.h"

namespace MediaPLayer {

DisplayFBDev::DisplayFBDev() :
		_fd(-1), _fbPtr(nullptr), _fbSize(0), _fbStride(0),
		_fbWidth(0), _fbHeight(0), _dstX(0), _dstY(0),
		_dstWidth(0), _dstHeight(0) {
}

DisplayFBDev::~DisplayFBDev() {
	deinit();
}

STATUS DisplayFBDev::init() {
	if (_initialized)
		return S_FAIL;

	if (internalInit() == S_FAIL)
		return S_FAIL;

	return S_OK;
}

STATUS DisplayFBDev::deinit() {
	if (!_initialized)
		return S_FAIL;

	internalDeinit();

	return S_OK;
}

STATUS DisplayFBDev::internalInit() {
	_fd = open("/dev/fb0", O_RDWR);
	if (_fd == -1) {
		log->printf("DisplayFBDev::internalInit(): Failed open /dev/fb0 %s\n", strerror(errno));
		goto fail;
	}

	if (ioctl(_fd, FBIOGET_VSCREENINFO, &_vinfo) == -1) {
		log->printf("DisplayFBDev::internalInit(): Failed FBIOGET_VSCREENINFO on /dev/fb0. %s\n", strerror(errno));
		goto fail;
	}

	_vinfo.xres_virtual = _vinfo.xres;
	_vinfo.yres_virtual = _vinfo.yres;

	if (ioctl(_fd, FBIOPUT_VSCREENINFO, &_vinfo) == -1) {
		log->printf("DisplayFBDev::internalInit(): Failed FBIOPUT_VSCREENINFO on /dev/fb0. %s\n", strerror(errno));
		goto fail;
	}

	if (_vinfo.bits_per_pixel != 32) {
		log->printf("DisplayFBDev::internalInit(): Display buffer is not 32 bits per pixel!\n");
		goto fail;
	}

	if (ioctl(_fd, FBIOGET_FSCREENINFO, &_finfo) == -1) {
		log->printf("DisplayFBDev::internalInit(): Failed FBIOGET_FSCREENINFO on /dev/fb0. %s\n", strerror(errno));
		goto fail;
	}

	if (_finfo.type != FB_TYPE_PACKED_PIXELS) {
		log->printf("DisplayFBDev::internalInit(): Only type FB_TYPE_PACKED_PIXELS is supported!\n");
		goto fail;
	}

	if (_finfo.visual != FB_VISUAL_TRUECOLOR) {
		log->printf("DisplayFBDev::internalInit(): Only FB_VISUAL_TRUECOLOR is supported!\n");
		goto fail;
	}

	_fbSize = _finfo.smem_len;
	_fbStride = _finfo.line_length;
	_fbWidth = _vinfo.xres;
	_fbHeight = _vinfo.yres;
	_fbPtr = (U8 *)mmap(0, _fbSize, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
	if (_fbPtr == (U8 *)-1) {
		log->printf("DisplayFBDev::internalInit(): Failed get frame buffer! %s\n", strerror(errno));
		goto fail;
	}

	memset(_fbPtr, 0, _fbSize);

	_initialized = true;
	return S_OK;

fail:

	if (_fd != -1)
		close(_fd);

	return S_FAIL;
}

void DisplayFBDev::internalDeinit() {
	if (_initialized == false)
		return;

	if (_fbPtr)
		munmap(_fbPtr, _fbSize);

	if (_fd != -1)
		close(_fd);

	_initialized = false;
}

STATUS DisplayFBDev::configure(U32 width, U32 height) {
	if (width <= 0 || height <= 0) {
		log->printf("DisplayFBDev::configure(): Bad arguments!\n");
		goto fail;
	}

	if (_vinfo.xres < width || _vinfo.yres < height) {
		log->printf("DisplayFBDev::configure(): Given resulution is bigger than frame buffer resolution!\n");
		goto fail;
	}

	_dstX = (_vinfo.xres - width) / 2;
	_dstY = (_vinfo.yres - height) / 2;
	_dstWidth = width;
	_dstHeight = height;

	return S_OK;

fail:
	return S_FAIL;
}

STATUS DisplayFBDev::putImage(VideoFrame *frame) {
	U32 x, y;

	if (frame == nullptr || frame->data[0] == nullptr) {
		log->printf("DisplayFBDev::putImage(): Bad arguments!\n");
		goto fail;
	}

	if (frame->pixelfmt == FMT_ARGB) {
		for (y = 0; y < _dstHeight; y++) {
			memcpy(_fbPtr + (_fbStride * (_dstY + y)) + _dstX * 4, frame->data[0] + y, frame->stride[0]);
		}
	} else if (frame->pixelfmt == FMT_RGB24) {
		for (y = 0; y < _dstHeight; y++) {
			U8 *ptr = _fbPtr + (_fbStride * (_dstY + y)) + _dstX * 4;
			for (x = 0; x < _dstHeight; x++) {
				ptr[(x * 4) + 0] = frame->data[0][(x * 3) + 0];
				ptr[(x * 4) + 1] = frame->data[0][(x * 3) + 1];
				ptr[(x * 4) + 2] = frame->data[0][(x * 3) + 2];
			}
		}
	} else if (frame->pixelfmt == FMT_YUV420P) {
		//yuv420_to_rgba_open();
		//yuv420_to_rgba_convert();
	} else {
		log->printf("DisplayFBDev::putImage(): Can not handle pixel format!\n");
		goto fail;
	}

	return S_OK;

fail:
	return S_FAIL;
}

STATUS DisplayFBDev::flip() {

	// nothing

	return S_OK;
}

} // namespace
