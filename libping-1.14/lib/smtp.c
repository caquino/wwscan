/**
 * SMTP module
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
#include <smtp.h>
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

#define  MSGBUF 2048 

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
smtp_alarm( int sig )
{
  longjmp( env, 1 );
} 

int
mysmtp( const char *smtphost, int t )
{
  char buf[MSGBUF];
  int len  = 0;
  int sock = 0;
  int port = 25;
  struct timeval mytime;
 
  if( t == 0 ) timo = 10;
  else         timo = t;
  signal( SIGALRM, smtp_alarm );
 
  if(( sock = JOEtsocket( smtphost, port, FALSE, timo )) <= 0 ){
    return sock;
  } 

  if( setjmp( env ) == 0 ){
    alarm( timo );
 
    /* set the rrt timer */
    (void) gettimeofday( &mytime, (struct timezone *)NULL); 
  
    do{
      if (!( len = JOErecv( buf, MSGBUF ))) break;
    } while(( buf[len - 1] != '\n') && ( buf[len - 1] != EOF ));
  
    if( smtp_cmd( "HELO", "dude" ) == -1 ) return -1;
    
    alarm( 0 );
  }
  else{
    JOEclose();
    return 0;   /* time out code */
  } 

  smtp_cmd( "QUIT", "" );
  JOEclose( sock );
  rrt = elapsed_time( &mytime );
  
  return 1;
}

int
smtp_cmd( char *cmd, char *val )
{
  char buf[MSGBUF];
  char rec[MSGBUF];
  int  len;
  int  ret;
  
  bzero( buf, sizeof( buf ));
  bzero( rec, sizeof( rec));  

  snprintf( buf, sizeof( buf ), "%s %s\r\n", cmd, val );
  if( JOEsend( buf ) < 0 )
    return -1;

  do{
    if( !( len = JOErecv( rec, MSGBUF ))) break;
  } while(( rec[len - 1] != '\n') && ( rec[len -1] != EOF ));
  
  if(( ret = atoi( rec )) > 400 ) return -1;
  else return 1;
}

int 
pingsmtp( const char *hostname )
{
  return mysmtp( hostname, 0 );
}

int 
pingtsmtp( const char *hostname, int t )
{
  return mysmtp( hostname, t );
}

int
tpingsmtp( const char *hostname )
{
  int ret = 0;
 
  ret = mysmtp( hostname, 0 );
 
  if( ret > 0 ){ return rrt; }
  else         { return ret; }
}
 
int
tpingtsmtp( const char *hostname, int t )
{
  int ret = 0;
 
  ret = mysmtp( hostname, t );
 
  if( ret > 0 ){ return rrt; }
  else         { return ret; }
 
} 
