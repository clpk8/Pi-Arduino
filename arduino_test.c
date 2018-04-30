

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <pthread.h>
#include <sqlite3.h>
int fd ;
sqlite3 *db;
sqlite3_stmt *stmt;

void reading(void* ptr){

    char command;

    while(1){
      //  sleep(5);
        command = (char)serialGetchar(fd);
        //printf (" -> %c", (char)serialGetchar(fd)) ;
        printf("%c received",command);
        fflush (stdout) ;

        switch (command) {
            case 'A':
                printf("Goto bathroom");
                fflush (stdout) ;

                break;
            case 'B':
                printf("Goto bedroom");
                fflush (stdout) ;

                break;
            case 'C':
                printf("Goto kitchen");
                fflush (stdout) ;

                break;
            case 'D':
                printf("Take meds");
                fflush (stdout) ;

                break;
            case 'E':
                printf("Drink water");
                fflush (stdout) ;

                break;
            case 'F':
                printf("Wakeup");
                fflush (stdout) ;

                break;
            default:
                printf("Stupid");
                fflush (stdout) ;

                break;
        }
    }



}
int main ()
{
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

    sqlite3_open("schedule.db", &db);

    if (db == NULL)
    {
        printf("Failed to open DB\n");
        return 1;
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
    int num_cols;
    pthread_create(&t1, NULL, (void*)&reading,NULL);

       // data = serialGetchar (fd);


    while(1){
        sqlite3_prepare_v2(db, "select julianday('now') - julianday(time) from schedules", -1, &stmt, NULL);
        num_cols = sqlite3_column_count(stmt);

        for(i = 0; i < num_cols; i++){
            printf("%g\n", sqlite3_column_double(stmt, i));
        }

        printf("Enter what you want to send\n");
        fflush (stdout) ;
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
            case 'q':
                serialClose(fd);
                return EXIT_FAILURE;
            default:
                break;
        }
    }








    printf ("\n") ;
    fflush (stdout) ;

    return 0 ;
}

