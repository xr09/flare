#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGTERM)
    {
        printf("received SIGTERM\n");
        printf("ending pending work... (it will take 15s)\n");
        sleep(15);
        printf("SUCCESS: program exits correctly\n");
        exit(0);
    }
}

int main(void)
{
    // disable stdout buffer to get real time logs from the container
    setbuf(stdout, NULL);

    printf("Flare running with PID %ld\n\n", (long)getpid());

    // warn about uncapturable signals
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        printf("Can't catch SIGUSR1\n");
    if (signal(SIGTERM, sig_handler) == SIG_ERR)
        printf("Can't catch SIGTERM\n");

    // Wait for signals
    while(1)
        sleep(1);
    return 0;
}
