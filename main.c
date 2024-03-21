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
static sem_t semaphore_command;
static int command;

typedef enum
{
   start_command = 1,
   pause_command = 2,
   reset_command = 3

}Command;

void* userInterface (void* args)
{
   int index = *(int*)args;
   int semVal;

   while (1)
   {
      printf("Enter the command: ");
      scanf("%d", &command);

      sem_post(&semaphore);
      //sem_post(&semaphore_command);
      //sem_getvalue(&semaphore, &semVal);
      //printf("(%d) Current semaphore value after post is %d\n", index, semVal);
   }
}

void* countAndSleep (void* args)
{
   int index = *(int*)args;
   int semVal;
   int sec = 0;
   
   while (1) 
   {
      sem_wait(&semaphore);
      //sem_getvalue(&semaphore, &semVal);
      //printf("(%d) Current semaphore value after wait is %d\n", index, semVal);
      pthread_mutex_lock(&mutex); 
      
      if (command == start_command) 
      {
         pthread_mutex_unlock(&mutex);
         while (1) 
         {
            printf("%d sec\n", sec);
            sec++;
            sleep(1); 
            pthread_mutex_lock(&mutex); 
            if (command != start_command)
            {
               pthread_mutex_unlock(&mutex);
               break; 
            }
            pthread_mutex_unlock(&mutex);
            
         }
      } 
      else if (command == pause_command) 
      {
         printf("Pause!\n");
      } 
      else if (command == reset_command) 
      {
         sec = 0;
      }
      pthread_mutex_unlock(&mutex);
   }
}

int main(int argc, char *argv[])
{
   pthread_t t1, t2;

   sem_init(&semaphore, 0, 0);
   pthread_mutex_init(&mutex, NULL);

   int i;
   int* a = malloc(sizeof(int));
   *a = i;

   pthread_create(&t1, NULL, &userInterface , a);
   pthread_create(&t2, NULL, &countAndSleep , a);

   pthread_join(t1, NULL);
   pthread_join(t2, NULL);

   sem_destroy(&semaphore);
   pthread_mutex_destroy(&mutex);
   return 0;
}