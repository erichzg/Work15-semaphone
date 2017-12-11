#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define KEY 24601

int main(int argc, char* argv[]) {

  if (!strcmp(argv[1], "-c")) {
    int sd = semget(KEY, 1, 0644 | IPC_CREAT | IPC_EXCL);
    if (sd < 0)
      printf("File already exists\n");
    else {
      int fd = shmget(KEY, 8, 0644 | IPC_CREAT | IPC_EXCL);
      printf("Shared memory created: %d\n", fd);
      int * line_size = shmat(fd, 0, 0);
      *line_size = 0;
      shmdt(line_size);
      printf("Semaphore created: %d\n", sd);
      int filed = open("story.txt", O_CREAT | O_EXCL, 0644);
      printf("File created\n");
    }
    semctl(sd, 0, SETVAL, 1);
  }

  else if(!strcmp(argv[1], "-v")) {
    printf("Story so far:\n");
    execlp("cat", "cat", "story.txt", NULL);
  }

  else if(!strcmp(argv[1], "-r")) {
    int sd = semget(KEY, 1, 0644);
    int fd = shmget(KEY, 8, 0644);
    semctl(sd, 0, IPC_RMID);
    printf("Semaphore removed\n");
    shmctl(fd, IPC_RMID, NULL);
    printf("Shared memory removed\n");
    remove("story.txt");
    printf("File removed\n");
  }
}
