/* Stoperica treba da broji sekunde. Korisnik unosi 
1 - start/continue
2 - pause
3 - reset counter
Na konzoli se ispisuje trenutno vreme.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef enum
{
    start_counter = 1,
    pause_counter = 2,
    reset_counter = 3

}Commands;


/* https://www.youtube.com/watch?v=i1MeXMciy6Q */
void* startCounter()
{
    time_t t = time(NULL);

    struct tm date = *localtime(&t);

    char dateString[200];

    strftime(dateString, 200, "%S", &date);

    printf("Current: %s sec.\n", dateString);

/* Ili si mislio
int sec = 0
while(1)
{
    printf("Current: %s sec.\n", sec);
    sleep(1);
    sec++
}
*/

}

void* pauseCounter()
{
    printf("Sledi 1\n");
}

void* resetCounter()
{
      printf("Sledi 2\n");
}

int main(int* args, char *arv[])
{
    int commands;
    pthread_t p1, p2, p3;

    printf("Enter the command (1 for start, 2 for pause, 3 for reset): ");
    scanf("%d",&commands);

    switch(commands)
    {
        case start_counter:
        {
            if(pthread_create(&p1, NULL, &startCounter, NULL) != 0)
            {
                return 1;
            }
    
            if(pthread_join(p1, NULL) != 0)
            {
                return 2;
            } 
            break;  
        }

        case  pause_counter:
        {
            if(pthread_create(&p2, NULL, &pauseCounter, NULL) != 0)
            {
                return 3;
            }
    
            if(pthread_join(p2, NULL) != 0)
            {
                return 4;
            } 
            break;
        }

        case  reset_counter:
        {
            if(pthread_create(&p3, NULL, &resetCounter, NULL) != 0)
            {
                return 5;
            }
    
            if(pthread_join(p3, NULL) != 0)
            {
                return 6;
            } 
            break;
        }
        default:
        {
            printf("Wrong command.\n");
            break;
        }
    }
    
    return 0;
   
}


