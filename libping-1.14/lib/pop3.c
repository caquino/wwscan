/**
 * POP3 module
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
#include <pop3.h>
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/

#include <sock.h>
#include <util.h>
#include <stdlib.h>
#include <stdio.h>
 
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

#define MSGBUF  1024

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
pop_alarm( int sig )
{
  longjmp( env, 1 );
} 

int
mypop3( const char *hostname, int t )
{
  char buf[MSGBUF];
  int x;
  int sock = 0;
  int len  = 0;
  int ret  = 0;
  int port = 110;
  struct timeval mytime;
  
  if( t == 0 ) timo = 10;
  else         timo = t;
  signal( SIGALRM, pop_alarm );
 
  if(( sock = JOEtsocket( hostname, port, FALSE, timo )) <= 0 ){
    return sock;
  }

  if( setjmp( env ) == 0 ){
    alarm( timo );

    /* set the rrt timer */
    (void) gettimeofday( &mytime, (struct timezone *)NULL); 

    do{
      if (!( len = JOErecv( buf, MSGBUF ))) break;
    } while(( buf[len - 1] != '\n') && ( buf[len - 1] != EOF ));
    if( !strncmp( buf, "+OK", 3 )) ret = 1; 
    
    alarm( 0 );
  }
  else{
    JOEclose();
    return 0;   /* time out code */
  } 

  send_cmd( "QUIT", NULL );
  JOEclose();
  rrt = elapsed_time( &mytime ); 

  return ret;
}

int
send_cmd( char *cmd, char *val )
{
  char buf[256];
  char rec[MSGBUF];
  int  len;

  if( val )
    snprintf( buf, sizeof( buf ), "%s %s\r\n", cmd, val );
  else
    snprintf( buf, sizeof( buf ), "%s\r\n", cmd );

  if( JOEsend( buf ) < 0 )
    return -1;

  do{
    if( !( len = JOErecv( rec, MSGBUF ))) break;
  } while(( rec[len - 1] != '\n') && ( rec[len -1] != EOF ));

  if( !strncmp( rec, "-ERR", 4 )){
    return -1;
  }

  return 1; 
}

int 
pingpop3( const char *hostname )
{
  return mypop3( hostname, 0 );
}

int
pingtpop3( const char *hostname, int t )
{
  return mypop3( hostname, t );
}

int
tpingpop3( const char *hostname )
{
  int ret = 0;
 
  ret = mypop3( hostname, 0 );
 
  if( ret > 0 ){ return rrt; }
  else         { return ret; }
}
 
int
tpingtpop3( const char *hostname, int t )
{
  int ret = 0;
 
  ret = mypop3( hostname, t );
 
  if( ret > 0 ){ return rrt; }
  else         { return ret; }
 
} 
