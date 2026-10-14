#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#define MSGSZ 128
#define MSG_TO_SERVER 100
#define CLIENT_ID 2

typedef struct {
    long mtype;
    int client_id;
    char mtext[MSGSZ];
} client_msg;

int main() {
    int msqid;
    key_t key = 5678;
    client_msg msg;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget client2");
        exit(1);
    }
    // 1. Enviar mensaje al servidor
    msg.mtype = MSG_TO_SERVER;
    msg.client_id = CLIENT_ID;
    strcpy(msg.mtext, "Petición del Cliente 2");
    if (msgsnd(msqid, &msg, sizeof(client_msg) - sizeof(long), 0) < 0) {
        perror("msgsnd client2");
        exit(1);
    }
    printf("[Cliente 2] Mensaje enviado al servidor.\n");
    // 2. Leer SOLAMENTE la respuesta privada enviada a mtype = CLIENT_ID (2)
    if (msgrcv(msqid, &msg, sizeof(client_msg) - sizeof(long), CLIENT_ID, 0) < 0) {
        perror("msgrcv client2");
        exit(1);
    }
    printf("[Cliente 2] Respuesta recibida: '%s'\n", msg.mtext);
    return 0;
}
