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

#ifndef DISPLAY_FBDEV_H
#define DISPLAY_FBDEV_H

#include <linux/fb.h>

#include "basetypes.h"

namespace MediaPLayer {

class DisplayFBDev : public Display {
private:

	int                         _fd;
	struct fb_var_screeninfo    _vinfo;
	struct fb_fix_screeninfo    _finfo;
	U8                         *_fbPtr;
	U32                         _fbSize;
	U32                         _fbStride;
	U32                         _fbWidth, _fbHeight;
	U32                         _dstX, _dstY;
	U32                         _dstWidth, _dstHeight;

public:

	DisplayFBDev();
	virtual ~DisplayFBDev();

	virtual STATUS init();
	virtual STATUS deinit();
	virtual STATUS configure(U32 width, U32 height);
	virtual STATUS putImage(VideoFrame *frame);
	virtual STATUS flip();

private:

	virtual STATUS internalInit();
	virtual void internalDeinit();
};

} // namespace

#endif
