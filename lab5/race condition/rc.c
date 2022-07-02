#include <pthread.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
int index =0;
char content[60];

pthread_mutex_t mutex;

void *t2(void *arg)
{
   puts("break in!");
   for(;;)
   {
       {
          if (index == 30)
              index = 0;
          
          if (pthread_mutex_lock(&mutex) != 0) {
              fprintf(stdout, "lock error\n");
          }
          usleep(rand()%2);
          printf("%d%c",*(int*)arg,content[index++]);
          usleep(rand()%2);
          pthread_mutex_unlock(&mutex);
       }
       if(index > 60)        
       {
           puts("get it");
           exit(0); 
       }
   }
   puts("t2 exit");
}
int main()
{
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        return 1;
    }
setvbuf(stdout, 0, 2, 0);
   FILE *f1 = fopen("secret.txt","r");
   FILE *f2 = fopen("/tmp/common.txt","r");
   fread(content,30,1,f2);
   fread(content+30,30,1,f1);
   fclose(f1);
   fclose(f2);
   pthread_t th1;
   pthread_t th2;
   int pstr1 = 1;
   int pstr2 = 2;
   int * th_ret = NULL;
   int ret1;
   int ret2;
   ret1 = pthread_create(&th1,NULL,t2,&pstr1);
   ret2 = pthread_create(&th2,NULL,t2,&pstr2);
   pthread_join(th1,&th_ret);
   pthread_join(th2,&th_ret);
   return 1;
}
