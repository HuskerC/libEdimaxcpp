#include <iostream>
#include "../libEdimax.h"
#include <exception>
#ifdef __linux__
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
typedef void (*sighandler_t)(int);
static sighandler_t
handle_signal (int sig_nr, sighandler_t signalhandler) {
   struct sigaction neu_sig, alt_sig;
   neu_sig.sa_handler = signalhandler;
   sigemptyset (&neu_sig.sa_mask);
   neu_sig.sa_flags = SA_RESTART;
   if (sigaction (sig_nr, &neu_sig, &alt_sig) < 0)
      return SIG_ERR;
   return alt_sig.sa_handler;
}
static void start_daemon (const char *log_name, int facility) {
   int i;
   pid_t pid;
   /* Kill Parent Process */
   if ((pid = fork ()) != 0)
      exit (EXIT_FAILURE);
   /* Getting session leadership               */
   if (setsid() < 0) {
      printf("%s cannot get session leadership!\n",
         log_name);
      exit (EXIT_FAILURE);
   }
   /* Signal SIGHUP ignored */
   handle_signal (SIGHUP, SIG_IGN);
   /* Or: signal(SIGHUP, SIG_IGN) ... */
   /* Kill child process */
   if ((pid = fork ()) != 0)
      exit (EXIT_FAILURE);
   /* Gründe für das Arbeitsverzeichnis:
    * + core-Datei wird im aktuellen Arbeitsverzeichnis
    *   hinterlegt.
    * + Damit bei Beendigung mit umount das Dateisystem
    *   sicher abgehängt werden kann
    */
   chdir ("/");
   /* clearing parent bitmask  */
   umask (0);
   /* Wir schließen alle geöffneten Filedeskriptoren ... */
   for (i = sysconf (_SC_OPEN_MAX); i > 0; i--)
      close (i);
   /* prepare syslog                            */
   openlog ( log_name,
             LOG_PID | LOG_CONS| LOG_NDELAY, facility );
}
#endif

int main(int argc, char* argv[])
{
	try{
#ifdef __linux__
		start_daemon("meinDaemon", LOG_LOCAL0);
#endif
		EdimaxDevice device("127.0.0.1");
		/* The Big Loop */
		int i=0;
		while (true) {
			EdimaxDevice::PowerInformation info = device.getCurrentPowerUsage();
			syslog(LOG_DAEMON,"PowerInfo%i:%f",i++,info.amps);
			sleep(30);
		}
	}
	catch(std::exception &e)
	{
		syslog(LOG_DAEMON,"Exception caught:%s ",e.what());
	}
}

