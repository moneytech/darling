/*
 This file is part of Darling.

 Copyright (C) 2019 Lubos Dolezel

 Darling is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Darling is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Darling.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _CMTIME_H_
#define _CMTIME_H_

#include <stdint.h>

typedef int64_t CMTimeValue;
typedef int32_t CMTimeScale;
typedef int64_t CMTimeEpoch;
typedef uint32_t CMTimeFlags;

typedef struct
{
	CMTimeValue     value;
	CMTimeScale     timescale;
	CMTimeFlags     flags;
	CMTimeEpoch     epoch;
}
CMTime;

extern const CMTime kCMTimeInvalid;
extern const CMTime kCMTimeIndefinite;
extern const CMTime kCMTimePositiveInfinity;
extern const CMTime kCMTimeNegativeInfinity;
extern const CMTime kCMTimeZero;

#endif
