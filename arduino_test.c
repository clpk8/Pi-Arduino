

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

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    
    for(i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
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

    sqlite3 *db;
    char* error = 0;
    int rc;
    char *sql;
    const char* data = "Called back function called";
    
    rc = sqlite3_open("schedule.db", &db);

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

    sql = "select julianday('now') - julianday(time) from schedules";
    
    rc = sqlite3_exec(db, sql, callback, (void*)data, &error);
    
    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);


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





    }


    printf ("\n") ;
    fflush (stdout) ;

    return 0 ;
}

