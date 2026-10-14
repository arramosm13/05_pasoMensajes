#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 1234;
    message_buf rbuf, sbuf;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget P2");
        exit(1);
    }

    // 1. Recibe el primer mensaje (mtype = 1)
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv P2 (mtype 1)");
        exit(1);
    }
    printf("Process 2 received: %s\n", rbuf.mtext);

    // 2. Responde "Loud and clear" (mtype = 2)
    sbuf.mtype = 2;
    strcpy(sbuf.mtext, "Loud and clear");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd P2 (mtype 2)");
        exit(1);
    }
    printf("Process 2 sent: %s\n", sbuf.mtext);

    // 3. Recibe la réplica (mtype = 3)
    if (msgrcv(msqid, &rbuf, MSGSZ, 3, 0) < 0) {
        perror("msgrcv P2 (mtype 3)");
        exit(1);
    }
    printf("Process 2 received: %s\n", rbuf.mtext);

    // Limpieza de la cola del kernel
    msgctl(msqid, IPC_RMID, NULL);

    return 0;
}
