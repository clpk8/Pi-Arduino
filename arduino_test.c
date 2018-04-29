

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <pthread.h>

void reading(void* ptr){

    int *i;
    i = (int*) ptr;
    
    while (serialDataAvail (i))
    {
        printf (" -> %3d", serialGetchar (i)) ;
        fflush (stdout) ;
    }
    
}
int main ()
{
    int fd ;
    int count ;
    unsigned int nextTime ;

    pthread_t t1;
    if ((fd = serialOpen ("/dev/ttyACM0", 9600)) < 0)
    {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
        return 1 ;
    }

    if (wiringPiSetup () == -1)
    {
        fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
        return 1 ;
    }
 //   pthread_create(&t1, NULL, (void*)&reading,(void*)&fd);

    char data;
    const char a = 'A';
    const char b = 'B';
    const char c = 'C';
    const char d = 'D';
    const char e = 'E';
    const char f = 'F';
    char z;

    int i;

       // data = serialGetchar (fd);

    
    while(1){
        printf("Enter what you want to send\n");
        scanf("%c",&z);
        switch (z) {
            case 'a':
                serialPutchar (fd, a);
            case 'b':
                serialPutchar (fd, b);
            case 'c':
                serialPutchar (fd, c);
            case 'd':
                serialPutchar (fd, d);
            case 'e':
                serialPutchar (fd, e);
            case 'f':
                serialPutchar (fd, f);
                break;
                
            default:
                break;
        }
    }


serialClose(fd);






    printf ("\n") ;
    return 0 ;
}

