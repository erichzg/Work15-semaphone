#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define KEY 24601

int main() {
  int sd = semget(KEY, 1, 0644);
  struct sembuf operation;
  operation.sem_num = 0;
  operation.sem_op = -1;
  operation.sem_flg = SEM_UNDO;
  semop(sd, &operation, 1);

  int shmd = shmget(KEY, 8, 0644);
  int * line_size = shmat(shmd, 0, 0);
  
  int filed = open("story.txt", O_RDWR | O_APPEND);
  lseek(filed, -1 * *line_size, SEEK_END);
  
  char story[*line_size];
  read(filed, story, *line_size);
  
  printf("Previous line:\n%s\n", story);

  char to_write[256];
  fgets(to_write, 256, stdin);
  *line_size = strlen(to_write);
  write(filed, to_write, *line_size);
  close(filed);
  shmdt(line_size);
}
