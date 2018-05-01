

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <pthread.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/types.h>
int fd ;

char string[20];
static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for(i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        printf("%s\n",argv[i]);
        sprintf(string,"%s",argv[i]);
       // strcpy(event,argv[i]);
    }

    printf("\n");
    return 0;
}


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
                system("omxplayer -o local rec_4s.mp3");
                fflush (stdout) ;

                break;
            case 'B':
                printf("Goto bedroom");
                system("omxplayer -o local gotoBed.mp3");
                fflush (stdout) ;

                break;
            case 'C':
                printf("Goto kitchen");
                system("omxplayer -o local gotoKitchen.mp3");
                fflush (stdout) ;

                break;
            case 'D':
                printf("Take meds");
                system("omxplayer -o local takeMeds.mp3");
                fflush (stdout) ;

                break;
            case 'E':
                printf("Drink water");
                system("omxplayer -o local drinkWater.mp3");
                fflush (stdout) ;

                break;
            case 'F':
                printf("Wakeup");
                system("omxplayer -o local wakeUp.mp3");
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
    if ((fd = serialOpen ("/dev/ttyACM1", 9600)) < 0)
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

    sqlite3 *db;
    char* error = 0;
    int rc;
    char *sql;
    const char* data1 = "Called back function called";

    rc = sqlite3_open("db", &db);

    if (db == NULL)
    {
        printf("Failed to open DB\n");
        return 1;
    }

    if (rc != SQLITE_OK){
        printf("error");
        sqlite3_close(db);
        return EXIT_FAILURE;
    }

    char pbuf[20] =" ";
    sql = "select task from schedules where ( julianday(time) - julianday('now','-5 hours') ) * 1440 < 1 and ( julianday(time) - julianday('now','-5 hours') ) * 1440 > -1";

    while(1){
        rc = sqlite3_exec(db, sql, callback, (void*)data1, &error);

        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", error);
            sqlite3_free(error);
        } else {
            fprintf(stdout, "Operation done successfully\n");
        }
        sleep(10);

        // data = serialGetchar (fd);

        //need to be changed

        printf("event is %s and p buf is %s\n",string,pbuf);
        if(strcmp(string,pbuf) != 0){

            strcpy(pbuf,string);

            if(strcmp(pbuf,"gotoBath") == 0){
                printf("AAAAAAAAAAAAA");
                serialPutchar (fd, a);
            }
            else if(strcmp(pbuf,"gotoBed") == 0){
                serialPutchar (fd, b);
                printf("BBBBBBBBBBBb");
            }
            else if(strcmp(pbuf,"gotoKitchen") == 0){
                serialPutchar (fd, c);
                printf("CCCCCCCCCCCCCCC");
            }
            else if(strcmp(pbuf,"takeMeds") == 0){
                serialPutchar (fd, d);
                printf("dddddddddddddddddd");
            }
            else if(strcmp(pbuf,"drinkWaters") == 0){
                serialPutchar (fd, e);
                printf("eeeeeeeeeeee");
            }
            else if(strcmp(pbuf,"wakeUp") == 0){
                serialPutchar (fd, f);
                printf("fffffffffff");
            }
            else{
                printf("pbuf is %s\n",pbuf);
            }

            printf("in side compare event is %s and p buf is %s\n",string,pbuf);

        }
        else
            printf("They are euqla\n");



    }


    sqlite3_close(db);







    printf ("\n") ;
    fflush (stdout) ;

    return 0 ;
}

