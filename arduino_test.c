#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
char *portname = "/dev/ttyACM0"

int main()
{
    int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
    if(fd < 0){
        printf("error");
        exit(-1);
    }
    
    int i;
    
    int n;
    
    while(1){
        n = read(fd,i,sizeof(i));
        sleep(1);
    }
    return(0);
}
