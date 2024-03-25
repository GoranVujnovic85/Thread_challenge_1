/* Stoperica treba da broji sekunde. Korisnik unosi
1 - start/continue
2 - pause
3 - reset counter
Na konzoli se ispisuje trenutno vreme.
*/
/* UI za unos i brojac za backend, koristiti mutex za pristup brojacu, semafor za slanje komandi i koristiti state masinu.
   1 thread za UI
   1 thread za brojac + sleep
   1 thread za state masinu
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

static pthread_mutex_t mutex;
static sem_t semaphore;
static int command;

typedef enum
{
   start_command = 1,
   pause_command = 2,
   reset_command = 3

}Command;


void* userInterface (void* args)
{
   while (1)
   {
      printf("Enter the command: ");
      scanf("%d", &command);
      
      if (command < 0 || command > 3)
      {
         printf("You should enter 1, 2 or 3!\n");
         continue;
      }
      sem_post(&semaphore);   
   }
}

void* countAndSleep (void* args)
{
   int sec = 0;

   while (1)
   {
      sem_wait(&semaphore);

      switch (command)
      {
         case start_command:

            while (command == start_command)
            {
               pthread_mutex_lock(&mutex);
               printf("%d sec\n", sec);
               sec++;
               sleep(1);  
               pthread_mutex_unlock(&mutex);    

            }  
         break;  
           
         case pause_command:
         
            pthread_mutex_lock(&mutex);
            printf("Pause!\n");
            pthread_mutex_unlock(&mutex); 

         break;
         
         case reset_command:
         
            pthread_mutex_lock(&mutex); 
            sec = 0;
            pthread_mutex_unlock(&mutex); 
         
         break;

         default:
         break;
      }
   }  
}


int main(int argc, char *argv[])
{
   pthread_t t1;
   pthread_t t2;
   
   sem_init(&semaphore, 0, 0);
   pthread_mutex_init(&mutex, NULL);

   pthread_create(&t1, NULL, &userInterface , 0);
   pthread_create(&t2, NULL, &countAndSleep , 0);
   
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
  
   sem_destroy(&semaphore);
   pthread_mutex_destroy(&mutex);

   return 0;

}