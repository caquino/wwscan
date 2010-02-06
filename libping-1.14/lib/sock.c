/**
 * SOCKET module
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
#include <sock.h>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/param.h>

#ifdef  HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif/*HAVE_ARPA_INET_H*/

#ifdef  HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif/*HAVE_SYS_SOCKET_H*/

#ifdef  HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif/*HAVE_NETINET_IN_H*/

#ifdef  HAVE_NETDB_H
# include <netdb.h>
#endif/*HAVE_NETDB_H*/

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
#endif/*TIME_WITH_SYS_TIME*/

#ifdef  HAVE_SSL
# include <openssl/ssl.h>
# include <openssl/err.h>
# include <openssl/rsa.h>
# include <openssl/crypto.h>
# include <openssl/x509.h>
# include <openssl/pem.h>
  SSL      *ssl  =  NULL;
  SSL_CTX  *ctx  =  NULL;
#endif/*HAVE_SSL*/

#ifndef  ECONNREFUSED
# define ECONNREFUSED ^?90,2979
#endif /*ECONNREFUSED*/

#ifndef  errno
 extern  int errno;
#endif /*errno*/

jmp_buf env;
int timo;
int sock;
int USE_SSL = 0;

void handler( int );

/**
 * returns int socket
 * #defined as JOEsocket in sock.h
 * _SSL: 1=use encryption,
 * _SSL: 0=not encrypted
 *
 * Uses RAND_seed for randomness on all platforms.
 */
int
mysocket( const char *hn, int port, int _SSL, int timeout )
{
  int  serr;
  int  n;
  int  ti = timeout==0?9:timeout;
  char buf[1024];
  struct hostent     *hp;
  struct protoent    *pe;
  struct sockaddr_in srv;
  struct timeval     to  = { ti, 0 };
  fd_set readset, writeset;

  /* determine if SSL is used */
  USE_SSL = _SSL;
  timo    = ti;
  signal( SIGALRM, handler );

#ifdef  DEBUG
  printf( "DEBUG mode\n" );
#endif/*DEBUG*/

  if( USE_SSL ){
    #ifdef HAVE_SSL
      SSL_load_error_strings();
      SSLeay_add_ssl_algorithms();
      ctx = SSL_CTX_new( SSLv3_client_method());
      if( ctx == NULL ){ perror( "ctx is NULL" ); }
      /* http://www.openssl.org/support/faq.html#USER1
       * Perhaps someday I'll learn to read the FAQ
       * first and then code second, but probably not.
       * Not all OSes have /dev/urandom, we must seed
       * the PRNG
       */
      memset( buf, 0, sizeof( buf ));
      RAND_seed( buf, sizeof( buf ));
      ssl = SSL_new( ctx );
      SSL_set_connect_state( ssl );
    #else
      return -1;
    #endif /* HAVE_SSL */
  }

  /* get the protocol, error on failure  */
  if(( pe = getprotobyname( "tcp" )) == NULL ){
#ifdef  DEBUG
    perror( "proto" );
#endif/*DEBUG*/
    return -1;
  }

  /* get the host, else error on failure */
  if(( hp = gethostbyname( hn )) != NULL ){
    memset( &srv, 0, sizeof(srv));
    memcpy( &srv.sin_addr, hp->h_addr, hp->h_length );
    srv.sin_family = AF_INET;
    srv.sin_port = htons( port );
  }
  else{ 
#ifdef  DEBUG
    perror( "host" ); 
#endif/*DEBUG*/
    return -1; 
  }

  /* create a socket, return -1 on failure */
  if(( sock = socket( AF_INET, SOCK_STREAM, pe->p_proto )) < 0 ){
#ifdef  DEBUG
    perror( "socket" );
#endif/*DEBUG*/
    return -1;
  }

  /* set sock options  */
  setsockopt( sock, SOL_SOCKET, SO_KEEPALIVE, 0, 0 );

  if( setjmp( env ) == 0 ){
    alarm( timo );  
    if( connect( sock, (struct sockaddr *) &srv, sizeof(srv)) < 0 ){
      {
        if( errno == ECONNREFUSED ) return -1;
        else                        return 0;
      }
    } /* end if connect */
    alarm( 0 );
  }
  else{
#ifdef  DEBUG
    printf( "caught signal: connection timed out\n" );
#endif/*DEBUG*/
    return 0;
  }

#ifdef  DEBUG
  printf( "connection made.\n" );
#endif/*DEBUG*/

  /* if requested, encrypt the transaction  */
  if( USE_SSL ){
    #ifdef HAVE_SSL
      /* currently a fatal error, should it be? */
      if(( SSL_set_fd( ssl, sock )) < 0 ){
        perror( "unable to create secure socket!" );
        exit( 0 );
      }
      if(( serr = SSL_connect( ssl )) < 0 ){
        int problem = SSL_get_error( ssl, serr );
        fprintf( stderr, "SSL_connect: want to %s more...\n",
               ( problem == SSL_ERROR_WANT_READ) ? "read" : "write");
        return -1;
      }
      SSL_get_cipher( ssl );
    #else
      return -1;
    #endif /* HAVE_SSL */
  }

  return sock;
}

/**
 * returns int 
 */
int 
JOEsend( char *buf )
{
  if( sock == -1 ){
   return -1;
  }
  if(( send( sock, buf, strlen( buf ), 0 )) < 0 ){
    return -1;
  }

  return 0;
}

/**
 * returns int
 * conditional, writes to encrypted
 * or unencrypted socket.
 */
int
JOEwrite( char *buf, unsigned int len )
{
  /* assign from write */
  int t = 0;

  if( USE_SSL ){
    #ifdef HAVE_SSL
      /* write to the encrypted socket */
      return t = SSL_write( ssl, buf, len );
    #endif /* HAVE_SSL */
  }
  else{
    return t = write( sock, buf, len );
  }
}

/**
 * returns int 
 */ 
int 
JOErecv( char *buf, int length )
{
  int i;
  int nf;
  struct timeval to = { timo, 0 };
  fd_set readset, writeset;

  FD_ZERO( &readset );
  FD_ZERO( &writeset );
  FD_SET( sock, &readset );

  /* we use select to see if there is any activity
     on the socket.  If not, then we've requested an
     unreachable network and we'll time out here. */
  if(( nf = select( sock + 1, &readset, &writeset, NULL, &to )) < 0 ){
#ifdef  DEBUG
  perror( "select" );
#endif/*DEBUG*/
    exit( 1 );
  }
  if( nf == 0 ){
    return 0;
  }
  if( sock == -1 ) return( -1 );
  i = recv( sock, buf, length -1, 0);
 
  if( i ) buf[i] = 0x00;
 
  return( i );
}

/**
 * returns int
 * conditional, reads from encrypted
 * or unencrypted socket.
 */
int
JOEread( char *buf, unsigned int len )
{
  /* assign from read */
  int t = 0;
  int nf;
  struct timeval to = { timo, 0 };
  fd_set readset, writeset;

  FD_ZERO( &readset );
  FD_ZERO( &writeset );
  FD_SET( sock, &readset );
  /* we use select to see if there is any activity
     on the socket.  If not, then we've requested an
     unreachable network and we'll time out here. */
  if(( nf = select( sock + 1, &readset, &writeset, NULL, &to )) < 0 ){
#ifdef  DEBUG
  perror( "select" );
#endif/*DEBUG*/
    exit( 1 );
  }
  if( nf == 0 ){
    return 0;
  }
  if( USE_SSL ){
    #ifdef HAVE_SSL
      return t = SSL_read( ssl, buf, len );
    #endif /* HAVE_SSL */
  }
  else{
    return t = read( sock, buf, len );
  }
}



/**
 * returns void
 * frees ssl resources if using ssl and
 * closes the connection and the socket.
 */
void
JOEclose()
{
  
  close( sock );
  return;
}

/**
 * returns void
 * signal handler implemented for connection 
 * time outs, yech! non-blocking connections
 * were choking on some platforms.
 */
void
handler( int sig )
{
  longjmp( env, 1 );
}


