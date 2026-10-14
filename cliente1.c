#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#define MSGSZ 128
#define MSG_TO_SERVER 100
#define CLIENT_ID 1

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
        perror("msgget client1");
        exit(1);
    }
    // 1. Enviar mensaje al servidor
    msg.mtype = MSG_TO_SERVER;
    msg.client_id = CLIENT_ID;
    strcpy(msg.mtext, "Petición del Cliente 1");

    if (msgsnd(msqid, &msg, sizeof(client_msg) - sizeof(long), 0) < 0) {
        perror("msgsnd client1");
        exit(1);
    }
    printf("[Cliente 1] Mensaje enviado al servidor.\n");
    // 2. Leer solo la respuesta privada enviada a mtype = CLIENT_ID (1)
    if (msgrcv(msqid, &msg, sizeof(client_msg) - sizeof(long), CLIENT_ID, 0) < 0) {
        perror("msgrcv client1");
        exit(1);
    }

    printf("[Cliente 1] Respuesta recibida: '%s'\n", msg.mtext);
    return 0;
}
