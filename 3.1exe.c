#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<../apue.h"

#define BUFFSIZE 1024;
static void sig_tstp(int signo);
void sig_handler(int signo)
{ 
    if (signo == SIGINT)
        printf("\nThis is a special signal handler for SIGINT\n");
   
    else if (signo == SIGQUIT)
        printf("\nThis is a special signal handler for SIGQUIT\n");

    else (signal(SIGTSTP, SIG_IGN) == SIG_DFL)
	signal(SIGTSTP, sig_tstp);
}

int main(void)
{
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    if (signal(SIGQUIT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGQUIT\n");
    while(1)
        sleep(1);
    return 0;
}

static void sig_tstp(int signo)
{
    sigset_t mask;

    printf("see you later...\n");

    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    signal(SIGTSTP, SIG_DFL);
    kill(getpid(), SIGTSTP);

    signal(SIGTSTP, sig_tstp);
    
   printf("..and we're back!\n");
}
