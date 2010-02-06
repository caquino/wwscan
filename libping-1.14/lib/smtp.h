/**
 * SMTP header
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
#ifndef  SMTP_H
#define  SMTP_H

int mysmtp( const char *hostname, int t );

int smtp_cmd( char *cmd, char *val );

int pingsmtp ( const char *hostname );
int pingtsmtp( const char *hostname, int t );
int tpingsmtp( const char *hostname );
int tpingtsmtp( const char *hostname, int t ); 

#endif /*SMTP_H*/
