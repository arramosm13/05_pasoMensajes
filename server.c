#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MSGSZ 128
#define MSG_TO_SERVER 100 // Tipo común para recibir peticiones

typedef struct {
    long mtype;
    int client_id;
    char mtext[MSGSZ];
} client_msg;

int main() {
    int msqid;
    key_t key = 5678; // Clave única para el servidor
    client_msg msg;

    // Crear cola de mensajes IPC
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget server");
        exit(1);
    }

    printf("Servidor iniciado. Esperando mensajes...\n");

    // Procesar peticiones de 2 clientes
    for (int i = 0; i < 2; i++) {
        if (msgrcv(msqid, &msg, sizeof(client_msg) - sizeof(long), MSG_TO_SERVER, 0) < 0) {
            perror("msgrcv server");
            exit(1);
        }

        printf("[Servidor] Recibido de Cliente %d: '%s'\n", msg.client_id, msg.mtext);

        // Preparar respuesta privada para ese cliente específico
        int dest_client = msg.client_id;
        msg.mtype = dest_client;

        if (dest_client == 1) {
            strcpy(msg.mtext, "Hola Cliente 1, tu solicitud fue procesada.");
        } else if (dest_client == 2) {
            strcpy(msg.mtext, "Hola Cliente 2, tu solicitud fue procesada.");
        }

        // Enviar respuesta privada
        if (msgsnd(msqid, &msg, sizeof(client_msg) - sizeof(long), 0) < 0) {
            perror("msgsnd server");
            exit(1);
        }
        printf("[Servidor] Respuesta privada enviada a Cliente %d.\n", dest_client);
    }

    printf("Servidor finalizó de responder a ambos clientes.\n");
    return 0;
}
