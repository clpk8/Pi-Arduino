

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <pthread.h>
int fd ;

void reading(void* ptr){



    while(1){
        sleep(5);
        printf (" -> %3d", fd) ;
        fflush (stdout) ;
    }

    

}
int main ()
{
    int count ;
    unsigned int nextTime ;

    pthread_t t1;
    if ((fd = serialOpen ("/dev/ttyACM2", 9600)) < 0)
    {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
        return 1 ;
    }

    if (wiringPiSetup () == -1)
    {
        fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
        return 1 ;
    }

    char data;
    const char a = 'A';
    const char b = 'B';
    const char c = 'C';
    const char d = 'D';
    const char e = 'E';
    const char f = 'F';
    char z;

    int i;
    
    pthread_create(&t1, NULL, (void*)&reading,NULL);

       // data = serialGetchar (fd);


    while(1){
        printf("Enter what you want to send\n");
        scanf("%c",&z);
        switch (z) {
            case 'a':
                serialPutchar (fd, a);
                break;
            case 'b':
                serialPutchar (fd, b);
                break;
            case 'c':
                serialPutchar (fd, c);
                break;
            case 'd':
                serialPutchar (fd, d);
                break;
            case 'e':
                serialPutchar (fd, e);
                break;
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

