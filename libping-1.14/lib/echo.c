/**
 * ECHO module
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
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/ 

#include <sock.h>
#include <echo.h>
#include <util.h>
#include <stdlib.h>
#include <stdio.h>  
#include <errno.h>

#ifdef HAVE_SYS_TIMES_H
# include <sys/times.h>
#endif /*HAVE_SYS_TIMES_H*/
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif /*TIME_WITH_SYS_TIME*/ 

#include <signal.h>
#include <setjmp.h> 

#define MAXLINE  81920 

int bytes;       /* bytes to send   */
int rrt;         /* round trip time */
int timo;        /* time out value  */
jmp_buf env;

/**
 * returns void
 * signal handler implemented for connection
 * time outs, yech! non-blocking connections
 * were choking on some platforms.
 */
void
alarm_handler( int sig )
{
  longjmp( env, 1 );
} 

int
myecho( const char *hostname, int t )
{
  int sock;      /* tcp socket handle  */
  int ret;       /* return conditional */
  int port = 7;  /* tcp echo port      */
  
  if( t == 0 ) timo = 5;
  else         timo = t;
  signal( SIGALRM, alarm_handler ); 

  if(( sock = JOEtsocket( hostname, port, FALSE, timo )) <= 0 ){
    return sock;
  }
  if( setjmp( env ) == 0 ){
    alarm( timo );
    ret = send_data(); 
    alarm( 0 );
  } 
  else{
    JOEclose();
    return 0;
  }
  
  JOEclose();  
 
  if( ret > 0 ){ return  1; }
  else         { return -1; }

}

int 
send_data()
{
  char buf[MAXLINE+1];
  char rec[MAXLINE+1];
  struct timeval mytime;
  char c;
  int  len;
  int  ret;
  int  x, n;

  bytes = (bytes>MAXLINE)?MAXLINE:bytes;

  memset( buf, 0, bytes+1 );
  memset( rec, 0, bytes+1 );

  for( x = 0; x < bytes; x++ )
    buf[x] = 'X';

  (void) gettimeofday( &mytime, (struct timezone *)NULL);  
  if( JOEsend( buf ) < 0 )
    return 0; 

  for( x = 0; x < bytes; x ++ ){
    if(( n = JOEread( &c, 1 )) == 1 ){
      rec[x] = c;
      if( c == '\n' )
        break;
    }
    else if( n == 0 ){
      if( x == 1 )
        return 0;
      else
        break;
    }
    else{
      if( errno == EINTR )
        continue;
      return -1;
    }
  }
  rrt = elapsed_time( &mytime );

  if(( ret = strlen( buf )) > 0 ){ 
    return 1;
  }
  else
    return 0;
}

int
echohost( const char *hostname, int b )
{
  bytes   = b;
  
  return ( myecho( hostname, 0 ));
}
 
int
echothost( const char *hostname, int b, int t )
{
  bytes   = b;
 
  return ( myecho( hostname, t ));
}

int
techohost( const char *hostname, int b )
{
  int ret = 0;
  bytes   = b;
 
  ret = myecho( hostname, 0 );
 
  if( ret > 0 ){ return rrt; }
  else         { return ret; }
}
 
int
techothost( const char *hostname, int b, int t )
{
  int ret = 0;
  bytes   = b;
 
  ret = myecho( hostname, t );
 
  if( ret > 0 ){ return rrt; }
  else         { return ret; }
 
} 

