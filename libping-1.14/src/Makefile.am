##
## src/Makefile.am
##
## Copyright (C) 2001 Jeffrey Fulmer <jdfulmer@armstrong.com>
## This file is part of libping
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##

bin_PROGRAMS         =   ring

ring_LDADD           =   $(top_srcdir)/lib/libping.la 

ring_DEPENDENCIES    =   $(top_srcdir)/lib/libping.la 

ring_SOURCES         =   main.c version.c getopt.c getopt.h getopt1.c

AUTOMAKE_OPTIONS     =   foreign no-dependencies                   

install-exec-hook:
	chown root $(bindir)/ring
	chgrp 0    $(bindir)/ring
	chmod 4755 $(bindir)/ring

