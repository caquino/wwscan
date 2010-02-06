/**
 * RING
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
#include <stdio.h>
#include <stdlib.h>
#include <ping.h>
#include <getopt.h>

#ifndef  EXIT_SUCCESS
# define EXIT_SUCCESS 0
#endif /*EXIT_SUCCESS*/

#ifndef  EXIT_FAILURE
# define EXIT_FAILURE 1
#endif /*EXIT_FAILURE*/

#define  TRUE         1
#define  FALSE        0

typedef enum
{
  ECHO,
  SMTP,
  POP3,
  HTTP,
  PING
} pingtype;

extern char *version_string; 

char *hostname;
int nhosts;
int bytes    = 2560;
int script   = 0;
int timeout  = 0;
int RRT      = 0;
pingtype pt  = PING;

/**
 * long options, std options struct
 */
static struct option long_options[] =
{
  { "version",    no_argument,       NULL, 'V' },
  { "version",    no_argument,       NULL, 'v' },
  { "help",       no_argument,       NULL, 'h' },
  { "verbose",    no_argument,       NULL, 'v' },
  { "echo",       required_argument, NULL, 'e' },
  { "rtt",        no_argument,       NULL, 'r' },
  { "timeout",    required_argument, NULL, 't' },
  { "script",     no_argument,       NULL, 's' }, 
  { "mail",       no_argument,       NULL, 'm' }, 
  { "pop",        no_argument,       NULL, 'p' }, 
  { "www",        no_argument,       NULL, 'w' } 
};

void
display_version( int i )
{
  /**
   * version_string is defined in version.c
   * adding it to a separate file allows us
   * to parse it in configure.
   */
  printf( "ring %s\n", version_string );
  /**
   * if TRUE, exit
   */
  if( i == 1 ){ exit( EXIT_SUCCESS ); }
}  /* end of display version */

void
display_help()
{
  /**
   * call display_version, but do not exit
   */
  display_version( FALSE );
  printf("Usage: ring [options]\n");
  printf("Options:\n"                    );
  puts("  -V, --version      VERSION, prints version number to screen.");
  puts("  -h, --help         HELP, prints this section.");
  puts("  -t, --timeout=NUM  TIMEOUT, seconds of inactivity before the connection" );
  puts("                     times out. Default timeout value is 5." );
  puts("  -r, --rtt          ROUND TRIP TIME, returns the rtt in milliseconds." );
  puts("  -e, --echo=NUM     ECHO, tcp echo NUM bytes to a network device;");
  puts("                     returns the round trip time in milliseconds." );
  puts("  -m, --mail         MAIL, ping a smtp mail server");
  puts("  -p, --pop          POP, ping a pop3 mail server");
  puts("  -w, --www          WWW, ping a http/https web server");
  puts("  -s, --script       BOOLEAN, prints the  output in \'script friendly\' format,");
  puts("                     i.e., 1 = host is alive, 0 = host is unreachable.  This" );
  puts("                     option does NOT print a newline \'\\n\' character so there" );
  puts("                     is no need to chomp it." );
  /**
   * our work is done, exit nicely
   */
  exit( EXIT_SUCCESS );
}

/**
 * parses command line arguments and assigns
 * values to run time variables. relies on GNU
 * getopts included with this distribution.
 */
void
parse_cmdline( int argc, char *argv[] )
{
  int c = 0;
  while ((c = getopt_long (argc, argv, "Vvhe:srt:mpw", long_options, (int *)0)) != EOF){
  switch( c ){
      case 'V':
        display_version( TRUE );
        break;
      case 'v':
        display_version( TRUE );
        break;
      case 'h':
        display_help();
        exit( EXIT_SUCCESS );
      case 's':
        script = 1;
        break;
      case 't':
        timeout  = atoi( optarg );
        break;
      case 'r':
        RRT = 1;
        break;       
      case 'e':
        pt = ECHO;
        bytes = atoi( optarg );
        break;
      case 'm':
        pt = SMTP;
        break;
      case 'p':
        pt = POP3;
        break;
      case 'w':
        pt = HTTP;
        break;
    } /** end of switch( c )           **/
  }   /** end of while c = getopt_long **/
  nhosts   = argc - optind;
  hostname = argv[argc-1];
  if( !nhosts ){
    display_help();
  }
}     /** end of parse_cmdline         **/

int
main( int argc, char *argv[] )
{
  int test = 0;
  parse_cmdline( argc, argv );

  switch( pt ){
  case PING:
    if( timeout ){
      if( RRT )
        test = tpingthost( hostname, timeout ); 
      else
        test = pingthost( hostname, timeout );
    }
    else{
      if( RRT )
        test = tpinghost( hostname );
      else
        test = pinghost( hostname ); 
    }
    break;
  case ECHO:
    if( timeout ){
      if( RRT )
        test = techothost( hostname, bytes, timeout );
      else
        test = echothost ( hostname, bytes, timeout );
    }
    else{
      if( RRT )
        test = techohost( hostname, bytes );
      else 
        test = echohost ( hostname, bytes );
    }
    break;
  case SMTP:
    if( timeout ){
      if( RRT )
        test = tpingtsmtp( hostname, timeout );
      else
        test = pingtsmtp ( hostname, timeout );
    }
    else{
      if( RRT )
        test = tpingsmtp( hostname );
      else
        test = pingsmtp ( hostname );
    }
    break;
  case POP3:
    if( timeout ){
      if( RRT )
        test = tpingtpop3( hostname, timeout );
      else
        test = pingtpop3 ( hostname, timeout );
    }
    else{
      if( RRT )
        test = tpingpop3( hostname );
      else
        test = pingpop3 ( hostname );
    }
    break; 
  case HTTP:
    if( timeout ){
      if( RRT )
        test = tpingthttp( hostname, timeout );
      else
        test = pingthttp ( hostname, timeout );
    }
    else{
      if( RRT )
        test = tpinghttp( hostname );
      else
        test = pinghttp ( hostname );
    }
    break; 
  default:
    break;
  }

  if( test  > 1 ){
    if( script ) 
      printf( "%d", test );
    else
      printf( "round trip time: %d milliseconds\n", test );
  }
  else if( test == 1 ){ 
    if( script )
      printf( "1" );
    else
      printf( "%s is alive.\n", hostname );
  }
  else if ( test == 0 ){
    if( script ) 
      printf( "0" );
    else
      printf( "0 connection timed out.\n" );
  }
  else if( test == -1 ){
    if( script ) 
      printf( "-1" );
    else
      printf( "-1 host or service is unreachable.\n" );
  }
  else if( test == -2 ){
    if( script ) 
      printf( "-2" );
    else
      printf( "-2 socket error, do you have permission?\n" );
  }
  else if( test == -3 ){
    if( script )
      printf( "-3" );
    else
      printf( "-3 connection refused\n" );
  } 
  
  exit( 0 );
}
