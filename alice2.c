#include <stdio.h>//printf
#include <fcntl.h> //flags for open
#include <unistd.h> //r/w/open/close
#include <stdlib.h>//atoi
#include <errno.h>//errno
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/shm.h>

int *balance;//=1000000;
void spend (char *name)
{
    int total_spent=0;
    //int total_spentB=0;
    while(*balance>=100)
    {
        printf("%s spends $100\n", name);
        total_spent+=100;
        *balance -= 100;
    }
    printf("\t\tTOTAL spent by %s $%d\n", name, total_spent);
}

int main(int argc, char* argv[])
{
    int sid=shmget(IPC_PRIVATE, sizeof(int), 0666);//creates segment
    printf("SIZ=%d\n", sid);
    balance = shmat(sid, NULL, 0);//attach segment
    *balance = 1000000; 
    int pid=fork();
    if (pid==0)//bob
    {
        spend("Bob");
        shmdt(balance);//detach
        return 0;
    }
    else//ALICE
    {
        spend("Alice");
        wait(NULL);
    }
    printf("Remaining balance: $%d\n", *balance);
    shmdt(balance);//detach
    shmctl(sid, IPC_RMID, NULL);
    return 0;
}