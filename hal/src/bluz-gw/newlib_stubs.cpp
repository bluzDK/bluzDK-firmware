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

extern unsigned long link_constructors_location;
extern unsigned long link_constructors_end;

static void call_constructors(unsigned long *start, unsigned long *end) __attribute__((noinline));

static void call_constructors(unsigned long *start, unsigned long *end)
{
    unsigned long *i;
    void (*funcptr)();
    for (i = start; i < end; i++)
    {
        funcptr=(void (*)())(*i);
        funcptr();
    }
}


extern "C" {
    
void CallConstructors(void)
{
    call_constructors(&link_constructors_location, &link_constructors_end);
}
    
extern char link_heap_location, link_heap_location_end;

int _write(int file, char *ptr, int len) { return 0; }
//int _read(int file, char *ptr, int len) { return 0; }
//int _close(int file) { return 0; }
int _lseek(int file, int ptr, int dir) { return 0; }
//int _fstat(int file, void *sbuf) { return 0; }
int _isatty(int file) { return 0; }

/* Bare metal, no processes, so always process id 1 */
int _getpid(void)
{
    return 1;
}
/* Bare metal, no processes, so error */
int _kill(int pid, int sig)
{
    return -1;
}


} /* extern "C" */

