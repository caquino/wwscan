/**
 * HTTP module
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
#ifdef  HAVE_CONFIG_H
# include <config.h>
#endif/*HAVE_CONFIG_H*/

#include <sock.h>
#include <http.h>
#include <util.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>

#ifdef STDC_HEADERS
# include <string.h>
#else
# ifndef HAVE_STRCHR
#  define strchr index
#  define strrchr rindex
# endif
char *strchr (), *strrchr ();
# ifndef HAVE_MEMCPY
#  define memcpy(d, s, n) bcopy ((s), (d), (n))
#  define memmove(d, s, n) bcopy ((s), (d), (n))
# endif
#endif

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

#ifndef HAVE_STRCASECMP
int strcasecmp();
#endif
#ifndef HAVE_STRNCASECMP
int strncasecmp();
#endif
#ifndef HAVE_STRNCMP
int strncmp();
#endif
#ifndef HAVE_STRLEN
int strlen();
#endif

#define  MSGBUF  2048 
#define  PLENGTH 84

struct URL U;    /* struct for URLs */
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
http_alarm( int sig )
{
  longjmp( env, 1 );
}

int
myhttp( char *url, int t )
{
  int SSL  = FALSE;
  int len  = 0;
  int sock = 0;
  int port = 80;
  int b    = 0;
  struct headers h;
  struct timeval mytime; 

  bzero( &U, sizeof( struct URL ));
  parse_url( url );

  if( t == 0 ) timo = 10;
  else         timo = t;
  signal( SIGALRM, http_alarm ); 

  if(( strncasecmp( U.protocol, "https", 5 )) == 0 ) SSL = TRUE;

  U.pathname = (strlen( U.pathname)==0)?strcpy(U.pathname,"/"):U.pathname;

  if(( sock = JOEtsocket( U.hostname, U.port, SSL, timo )) <= 0 ){
    return sock;
  } 

  if( setjmp( env ) == 0 ){
    alarm( timo );
 
    /* set the rrt timer */
    (void) gettimeofday( &mytime, (struct timezone *)NULL);
    /* send a HTTP request to the server */
    if(( http_send( U.hostname, U.pathname )) == -1 ){
      return 0;
    }
    /* read the headers from the socket */
    h = http_read_headers( ); 

    /* read the content and determine bytes read */
    b = http_read();
   
    alarm( 0 );
  }
  else{
    JOEclose();
    return 0;   /* time out code */
  }

  JOEclose( sock );
  rrt = elapsed_time( &mytime ); 

  if( h.code != 200 ){ return -1; }
  else               { return  1; }
}

int
http_send( char *host, char *path )
{
  /* assign socket_write to t */
  int  t = 0;

  /* HTTP request char array  */
  char request[1024];

  char *user_agent = "Mozilla/4.06 [en] (X11; I; Pinger 1.00)";

  /* build a request string to
     pass to the server
     BUG FIX: changed this to
     snprintf to avoid buffer
     overload.               */
  snprintf(
    request, sizeof( request ),
    "GET %s HTTP/1.1\015\012"
    "Host: %s\015\012"
    "Accept: */*\015\012"
    "Accept-Encoding: * \015\012"
    "User-Agent: %s\015\012"
    "Connection: close\015\012\015\012",
    path, host, user_agent
  );

  return t = JOEwrite( request, sizeof( request ));
}

struct headers
http_read_headers( )
{
  int  x;           /* while loop index      */
  int  n;           /* assign socket_read    */
  char c;           /* assign char read      */
  char line[255];   /* assign chars read     */
  struct headers h; /* struct to hold it all */

  while( TRUE ){
    x = 0;
    memset( &line, 0, sizeof( line ));
    while(( n = JOEread( &c, 1 )) == 1 ){
      line[x] = c;
      if(( line[0] == '\n' ) || ( line[1] == '\n' )){
        return h;
      }
      if( line[x] == '\n' ) break;
      x ++;
    }
    if( strncasecmp( line, "http", 4 ) == 0 ){
      /* reset the char array to hold new info */
      memset( &h.head, 0, sizeof( h.head ));
      strncpy( h.head, line, 8 );
      h.code = atoi( line + 9 );
    }
    if( strncasecmp( line, "content-length: ", 16 ) == 0 ){
      h.length = atol( line + 16 );
    }

  } /* end of while TRUE */
  return h;
}

/**
 * returns int
 * reads a http/https socket
 * ( you know what I mean :)
 */
int
http_read( void )
{
  char c;         /* assign char read    */
  int  n;         /* assign socket_read  */
  int  bytes;     /* assign bytes rec'd  */
  char line[512]; /* assign chars read   */

  bytes = 0;
  /* no need to actually DO anything with the output */
  while(( n = JOEread( &c, 1 )) == 1 ){
    bytes++;
  } /* end of while n    */

  /* bytes read */
  return bytes;
}

/**
 * ALERT: using hardcoded array lengths below,
 * if you change this array, then redefine PLENGTH
 *
 * Currently http(prot[25]) and https(prot[26]) are
 * the only supported protocols.  But all w3c supported
 * protocols are listed for URL evaluation.
 */
static char *prot[] = {
  "about:",      "addrbook:",  "acap:",      "afp:",
  "afs:",        "callto:",    "chttp:",     "cid:",
  "clsid:",      "data:",      "date:",      "DAV:",
  "dns:",        "eid:",       "fax:",       "file:",
  "finger:",     "freenet:",   "ftp:",       "gopher:",
  "gsm:",        "h323:",      "h324:",      "hdl:",
  "hnews:",      "http:",      "https:",     "iioploc:",
  "ilu:",        "imap:",      "IOR:",       "irc:",
  "isbn:",       "java:",      "JavaRMI:",   "javascript:",
  "jdbc:",       "ldap:",      "lid:",       "lifn:",
  "livescript:", "lrq:",       "mailto:",    "mailserver:",
  "md5:",        "mid:",       "mocha:",     "modem:",
  "news:",       "nfs:",       "nntp:",      "opaquelocktoken:"
  "path:",       "phone:",     "pop:",       "pop3:",
  "printer:",    "prospero:",  "res:",       "rtsp:",
  "rvp:",        "rwhois:",    "rx:",        "sdp:",
  "sip:",        "shttp:",     "snews:",     "STANF:",
  "t120:",       "tel:",       "telephone:", "telnet:",
  "tip:",        "tn3270:",    "tv:",        "uuid:",
  "urn:",        "vemmi:",     "videotex:",  "view:",
  "wais:",       "whois++:",   "whodp:",     "z39.50r:",
  "z39.50s:"

};

/**
 * int value of the length of the protocol
 * string passed to the function.
 */
int
protocol_length( char *url )
{
  int x;
  /**
   * hardcoded protocol length!!
   * see explanation above...
   */
  for( x = 0; x < PLENGTH; x ++ ){
    if( strncasecmp( url, prot[x], strlen( prot[x] )) == 0 )
      return strlen( prot[x] );
  }
  return 0;
}

/**
 * boolean, returns true if the protocol is
 * supported by siege, false if it is not.
 */
int
is_supported( char *url )
{
  if( strncasecmp( url, prot[25], strlen( prot[25] )) == 0 )
    return TRUE;
  if( strncasecmp( url, prot[26], strlen( prot[26] )) == 0 )
    #ifdef HAVE_SSL
      return TRUE;
    #else
      return FALSE;
    #endif /* HAVE_SSL */
  else
    return FALSE;
}

/**
 * Returns a char pointer of the default
 * port.  Why not just return an int? Legacy.
 */
char *
get_default_port( char *p )
{
  /* match the prot array, could have
     chosen strlen -1 as well.       */
  strcat( p, ":" );

  if( strncasecmp( p, prot[25], strlen( prot[25] )) == 0 )
    return "80";
  if( strncasecmp( p, prot[26], strlen( prot[26] )) == 0 )
    #ifdef HAVE_SSL
      return "443";
    #else
      return "80";     /* this might get me in trouble! */
    #endif /* HAVE_SSL */
  else
    return "80";
}


/**
 * parse_url  populates struct with URL information
 * @return    int, 1 on success, 0 on failure
 */
int
parse_url( char *url )
{
  int i, j, k, l;  /* placement counters. */
  char *portname;  /* temp holding space  */

  if(( i = protocol_length( url )) > 0 && is_supported( url ) == TRUE )
    i += 2;
  else if(( i = protocol_length( url )) > 0 && is_supported( url ) == FALSE )
    return -1;
  else
    i = 0;  /* no specified protocol, assuming http: */

  j = i;
  while( url[j] && url[j] != ':' && url[j] != '/' ) {
    j++;
  }
  if( url[j] == ':' ){
    l = j;
    while( url[j] && url[j] != '/' ){
      j++;
    }
  }
  else{
    l = j;
  }
  if( url[j] == '/' )
    k = j;
  else
    k = strlen( url );

  if( i == 0 ) U.protocol = (char*)strdup( "http://" );
  else         U.protocol = (char*)strdup( substring( url, 0, i-3 ));
  
  U.hostname = (char *)strdup(substring( url, i, ( l - i )));
  
  if( l == j )
    portname = (char *)strdup( get_default_port( U.protocol ));
  else
    portname = (char *)strdup(substring( url, l+1, (k-(l+1))));
  U.port = atoi( portname );

  if(( U.pathname = (char *)strdup(substring( url, k, strlen( url )))) == NULL )
       U.pathname = (char *)strdup( "/" );
  
  return 0;
}

int
pinghttp( char *hostname )
{
  return myhttp( hostname, 0 );
}

int
pingthttp( char *hostname, int t )
{
  return myhttp( hostname, t );
}

int
tpinghttp( char *hostname )
{
  int ret = 0;
 
  ret = myhttp( hostname, 0 );
 
  if( ret > 0 ){ return rrt; }
  else         { return ret; }
}
 
int
tpingthttp( char *hostname, int t )
{
  int ret = 0;
 
  ret = myhttp( hostname, t );
 
  if( ret > 0 ){ return rrt; }
  else         { return ret; }
 
} 
