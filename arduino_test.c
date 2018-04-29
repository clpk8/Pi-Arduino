

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <pthread.h>

void * reading(void* ptr){
    for(i = 0; i < 20; i++){
        printf("%c",serialGetchar(fd));
    }
}
int main ()
{
    int fd ;
    int count ;
    unsigned int nextTime ;

    pthread_t t1;
    pthread_create(&t1, NULL, reading,NULL);
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
    char data;
    const char c = 'A';
    int i;

       // data = serialGetchar (fd);

    for(i = 0; i < 10; i++){
        serialPutchar (fd, c) ;
        printf("1");
    }
    pthread_exit(t1);


serialClose(fd);






//    nextTime = millis () + 300 ;
//
//    for (count = 0 ; count < 256 ; )
//    {
//        if (millis () > nextTime)
//        {
//            printf ("\nOut: %3d: ", count) ;
//            fflush (stdout) ;
//            serialPutchar (fd, count) ;
//            nextTime += 300 ;
//            ++count ;
//        }
//
//        delay (3) ;
//
//        while (serialDataAvail (fd))
//        {
//            printf (" -> %3d", serialGetchar (fd)) ;
//            fflush (stdout) ;
//        }
//    }

    printf ("\n") ;
    return 0 ;
}

