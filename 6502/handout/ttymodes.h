#ifndef __TTYMODES_H__
#define __TTYMODES_H__

/* FROM : Advanced Programming in the UNIX Environment, Addison-Wesley,
   1992, ISBN 0-201-56317-7
   http://www.kohala.com/start/apue.linux.tar.Z
*/

int		 tty_cbreak(int);			/* {Prog raw} */
int		 tty_raw(int);				/* {Prog raw} */
int		 tty_reset(int);			/* {Prog raw} */
void	         tty_atexit(void);			/* {Prog raw} */
#ifdef	ECHO	/* only if <termios.h> has been included */
struct termios	*tty_termios(void);	/* {Prog raw} */
#endif


#endif
