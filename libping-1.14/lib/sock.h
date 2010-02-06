/**
 * SOCKET header
 *
 * Copyright (C) 2001 Jeffrey Fulmer <jdfulmer@armstrong.com>
 * This file is part of LIBPING
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifndef SOCK_H
#define SOCK_H

#define  TRUE  1
#define  FALSE 0

#define JOEsocket(a,b,c)     mysocket((a),(b),(c),(0))
#define JOEtsocket(a,b,c,d)  mysocket((a),(b),(c),(d))

int mysocket( const char *, int, int, int );

int JOEsend( char * );

int JOEwrite( char *buf, unsigned int len );

int JOErecv( char *, int );

int JOEread( char *buf, unsigned int len );

void JOEclose( );

#endif /* SOCK_H */

