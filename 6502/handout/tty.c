/* JA obtained from http://www.hackchina.com/en/r/140807/tty_raw.c__html */
/* @file tty_raw.c
 *
 * R. Stevens Adv. Prog. in UNIX Env. p.355
 *
 */
#include <stdlib.h>  // exit
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "tty.h"
#include "ttymodes.h"

static void
sig_catch( int signo )
{
  //  tty_atexit();  Not necessary as called as a side effect
  //  of exit
  exit(0);
}

/* modified by Jonathan Appavoo */
void
tty_init(int fd) {
  
  if ( signal( SIGINT, sig_catch ) == SIG_ERR ) {
    printf("signal(SIGINT) error\n");
    exit(1);
  }
  if ( signal( SIGTERM, sig_catch ) == SIG_ERR ) {
    printf("signal(SIGTERM) error\n");
    exit(1);
  }
  if ( signal( SIGQUIT, sig_catch ) == SIG_ERR ) {
    printf("signal(SIGQUIT) error\n");
    exit(1);
  }

  atexit(tty_atexit);

  tty_cbreak(fd);
}
