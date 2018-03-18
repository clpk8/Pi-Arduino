#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
char *portname = "/dev/ttyACM0";

int main()
{
    printf("HI");
    int fd = open (portname, O_RDWR);
    if(fd < 0){
        printf("error");
        //exit(-1);
    }

    int i;

    int n;

    while(1){
        n = read(fd,&i,sizeof(i));
        printf("The value received is%d\n",i);
        sleep(1);
    }
    return(0);
}
