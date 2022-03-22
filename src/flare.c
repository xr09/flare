#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

// default time to sleep is 15s
int sleep_time = 15;

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGINT)
    {
        printf("\nreceived SIGINT (Keyboard Interrupt)\n");
        exit(0);
    }
    else if (signo == SIGTERM)
    {
        printf("received SIGTERM\n");
        printf("the program will take %is to finish correctly\n", sleep_time);
        printf("...\n");
        sleep(sleep_time);
        printf("SUCCESS: the program had enough time to finish\n");
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    // if passed as CLI arg then use that
    if( argc == 2 )
        sleep_time = atoi(argv[1]);

    // disable stdout buffer to get real time logs from the container
    setbuf(stdout, NULL);

    printf("Flare running with PID %ld\n\n", (long)getpid());

    // warn about uncapturable signals
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("Can't catch SIGINT\n");
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        printf("Can't catch SIGUSR1\n");
    if (signal(SIGTERM, sig_handler) == SIG_ERR)
        printf("Can't catch SIGTERM\n");

    // Wait for signals
    while(1)
        sleep(1);
    return 0;
}
