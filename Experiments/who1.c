#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>

void show_info( struct utmp *utbufp );

/*
 *  who version 1   - read utmp file and show info therein
 */

#define INFOFILE  "/var/run/utmp"

int main(int ac, char **av)
{
  struct utmp utbuf;               /* read info into here */
  int   utmpfd;                    /* read from this descriptor */

  if ( (utmpfd = open( INFOFILE, O_RDONLY )) == -1 ){
    fprintf(stderr,"%s: cannot open %s\n", *av, INFOFILE);
    exit(1);
  }

  while (read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
    show_info( &utbuf );
  close( utmpfd );
  return 0;                        /* went ok */
}


/*
 *  show info()
 *      displays the contents of the utmp struct
 *      in human readable form
 *      *note* these sizes should not be hardwired
 */
void show_info( struct utmp *utbufp )
{
  printf("%-8.8s", utbufp->ut_name);    /* the logname  */
  printf(" ");                          /* a space  */
  printf("%-8.8s", utbufp->ut_line);    /* the tty  */
  printf(" ");                          /* a space  */
  printf("%10d", utbufp->ut_time);      /* login time */
  printf(" ");                          /* a space  */
  printf("(%s)", utbufp->ut_host);      /* the host */
  printf("\n");                         /* newline  */
}
