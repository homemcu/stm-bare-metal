/*
* Copyright (c) 2018 Vladimir Alemasov
* All rights reserved
*
* This program and the accompanying materials are distributed under 
* the terms of GNU General Public License version 2 
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#include <sys/stat.h>

void putcharuart(char ch);
char getcharuart(void);

//--------------------------------------------
void _exit(int status)
{
	while (1);
}

//--------------------------------------------
caddr_t _sbrk(int incr)
{
	extern char _ebss;
	static char *heap;
	char *prev_heap;

	if (heap == NULL)
	{
		heap = &_ebss;
	}
	prev_heap = heap;
	heap += incr;
	return (caddr_t)prev_heap;
}

//--------------------------------------------
int _write(int file, char *ptr, int len)
{
	int cnt;
	for (cnt = 0; cnt < len; cnt++)
	{
		putcharuart(*ptr++);
	}
	return len;
}

//--------------------------------------------
int _close(int file)
{
	return -1;
}
 
//--------------------------------------------
int _lseek(int file, int ptr, int dir)
{
	return 0;
}

//--------------------------------------------
int _read(int file, char *ptr, int len)
{
	int cnt;
	char ch;
	for (cnt = 0; cnt < len; cnt++)
    {
		ch = getcharuart();
		putcharuart(ch);
		ptr[cnt] = ch;
		if (ch == '\r' || ch == '\n')
		{
			ptr[cnt] = '\n';
			cnt++;
			break;
		}
	}
	return cnt;
}

//--------------------------------------------
int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

//--------------------------------------------
int _isatty(int file)
{
	return 1;
}
