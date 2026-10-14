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
    message_buf sbuf, rbuf;

    // Crear la cola de mensajes
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget P1");
        exit(1);
    }
    // 1. Envía "Are you hearing me?" (mtype = 1)
    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Are you hearing me?");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd P1 (mtype 1)");
        exit(1);
    }
    printf("Process 1 sent: %s\n", sbuf.mtext);
    // 2. Recibe la respuesta (mtype = 2) - Bloqueante
    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0) {
        perror("msgrcv P1 (mtype 2)");
        exit(1);
    }
    printf("Process 1 received: %s\n", rbuf.mtext);
    // 3. Replica "I can hear you too" (mtype = 3)
    sbuf.mtype = 3;
    strcpy(sbuf.mtext, "I can hear you too");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd P1 (mtype 3)");
        exit(1);
    }
    printf("Process 1 sent: %s\n", sbuf.mtext);
    return 0;
}
