#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>
#include <time.h>

#define LISTEN_PORT 5005
#define FORWARD_PORT 5006
#define MAX_DRONES 10
#define COLLISION_THRESHOLD 0.005

typedef struct {
    int id;
    float lat, lon;
    float speed;
    float heading;
    long timestamp;
    float battery;   // 🔥 NEW FIELD
} Drone;

Drone drones[MAX_DRONES];
int drone_count = 0;

// 🔹 Find drone by ID
int findDrone(int id) {
    for(int i = 0; i < drone_count; i++) {
        if(drones[i].id == id)
            return i;
    }
    return -1;
}

// 🔹 Add or update drone
void updateDrone(Drone d) {
    int index = findDrone(d.id);

    if(index == -1) {
        if(drone_count < MAX_DRONES) {
            drones[drone_count++] = d;
        }
    } else {
        drones[index] = d;
    }
}

// 🔹 Collision Detection
void checkCollisions() {
    for(int i = 0; i < drone_count; i++) {
        for(int j = i + 1; j < drone_count; j++) {

            float dist = sqrt(
                pow(drones[i].lat - drones[j].lat, 2) +
                pow(drones[i].lon - drones[j].lon, 2)
            );

            if(dist < COLLISION_THRESHOLD) {
                printf("⚠️ Collision Risk: Drone %d & %d\n",
                       drones[i].id, drones[j].id);
            }
        }
    }
}

int main() {
    int sock_recv, sock_send;
    struct sockaddr_in recvAddr, sendAddr;
    char buffer[1024];

    sock_recv = socket(AF_INET, SOCK_DGRAM, 0);
    sock_send = socket(AF_INET, SOCK_DGRAM, 0);

    // Receive setup
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(LISTEN_PORT);
    recvAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sock_recv, (struct sockaddr*)&recvAddr, sizeof(recvAddr));

    // Send setup (⚠️ replace with your laptop IP if needed)
    sendAddr.sin_family = AF_INET;
    sendAddr.sin_port = htons(FORWARD_PORT);
    sendAddr.sin_addr.s_addr = inet_addr("IP ADDRESS");

    printf("🚀 Smart Client Running...\n");

    while(1) {
        int len = recvfrom(sock_recv, buffer, sizeof(buffer), 0, NULL, NULL);
        buffer[len] = '\0';

        Drone d;
        float vx, vy, vz;

        // 🔥 UPDATED PARSING (WITH BATTERY)
        sscanf(buffer, "%d,%f,%f,%f,%f,%f,%f,%f,%ld,%f",
               &d.id, &d.lat, &d.lon,
               &vx, &vy, &vz,
               &d.speed, &d.heading,
               &d.timestamp,
               &d.battery);

        // 🔹 Filter noise
        if(d.speed < 0.00001) {
            continue;
        }

        // 🔹 Update drone state
        updateDrone(d);

        // 🔹 Logging
        printf("[CLIENT] Drone %d | Lat: %.4f Lon: %.4f | Speed: %.5f | Battery: %.2f%%\n",
               d.id, d.lat, d.lon, d.speed, d.battery);

        // 🔥 LOW BATTERY ALERT
        if(d.battery < 20) {
            printf("⚠️ CLIENT ALERT: Drone %d LOW BATTERY!\n", d.id);
        }

        // 🔥 Collision detection
        checkCollisions();

        // 🔹 Forward to Python
        sendto(sock_send, buffer, strlen(buffer), 0,
               (struct sockaddr*)&sendAddr, sizeof(sendAddr));
    }

    close(sock_recv);
    close(sock_send);
    return 0;
}
