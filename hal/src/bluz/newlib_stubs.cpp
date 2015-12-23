/**
  ******************************************************************************
  * @file    newlib_stubs.cpp
  * @author  Zachary Crockett
  * @version V1.0.0
  * @date    24-April-2013
  * @brief
  ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
*/

/* Define caddr_t as char* */
#include <sys/types.h>

extern "C" {

int _write(int file, char *ptr, int len) { return 0; }
//int _read(int file, char *ptr, int len) { return 0; }
//int _close(int file) { return 0; }
int _lseek(int file, int ptr, int dir) { return 0; }
//int _fstat(int file, void *sbuf) { return 0; }
int _isatty(int file) { return 0; }


} /* extern "C" */

