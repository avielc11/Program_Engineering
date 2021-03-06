#include "slast_func.h"  

int main(int ac, char *av[])
{    
    struct utmp utmp;
    int reclen = sizeof(utmp);
    long int linelen; 
    
    //check if request for limit lines. else -> print all the file wtmp
    if(ac == 2){
        linelen = strtol(av[1], NULL, 10);
        if(linelen <= 0){
            if(linelen == 0){
                linelen = LONG_MAX;
            }
            else{
                printf("Error - do not insert negative number\n");
                exit(1);        
            }
        }
    }
    else{
        printf("Error - insert only two arguments\n");
        exit(1);
    }

    //Open the file
    int fd = open(WTMP_FILE, O_RDONLY);  
    if(fd == -1){
        printf("Error - in open the file\n");
        exit(2);
    }

    int num = 1;

    //Read from file and print to the screen
    for(int i = 0 ; i < linelen ; i++){
        int cursor = -reclen * num++; 
        if(lseek(fd, cursor, SEEK_END) == -1){
            break;
        }
        if(read(fd, &utmp, reclen) == reclen){
            if(utmp.ut_type == USER_PROCESS || utmp.ut_type == BOOT_TIME){
                read_utmp(&utmp);                
            }
            else {
                i--;
            }
        }
    }

    //Read the first login and print wtmp begins <date>
    lseek(fd, 0, SEEK_SET);
    read(fd, &utmp, reclen);
    print_begin(&utmp); 

    // Close the file
    close(fd);

    return 0;
}