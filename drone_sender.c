#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <math.h>

#define PORT 5005

int main() {
    int sock;
    struct sockaddr_in serverAddr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("VM DEVICE IP"); // change if needed

    float lat[3], lon[3];
    float vx[3] = {0}, vy[3] = {0}, vz[3] = {0};
    float prevLat[3], prevLon[3];

    float battery[3] = {100, 100, 100};

    // Initial positions
    lat[0] = 12.9; lon[0] = 77.5;
    lat[1] = 12.9; lon[1] = 77.5;
    lat[2] = 12.9; lon[2] = 77.6;

    for(int i = 0; i < 3; i++) {
        prevLat[i] = lat[i];
        prevLon[i] = lon[i];
    }

    char buffer[256];

    float minLat = 12.9, maxLat = 13.0;
    float minLon = 77.5, maxLon = 77.6;
    float step = 0.001;

    int rect_phase = 0;
    static int dir2 = 1;
    static int dir3 = 1;

    srand(time(NULL));

    printf("🚀 Smart Drone Sender Running...\n");

    while(1) {

        // 🔵 Drone 1: Rectangle
        switch(rect_phase) {
            case 0: lat[0] = minLat; lon[0] += step;
                if(lon[0] >= maxLon) { lon[0] = maxLon; rect_phase = 1; }
                break;
            case 1: lon[0] = maxLon; lat[0] += step;
                if(lat[0] >= maxLat) { lat[0] = maxLat; rect_phase = 2; }
                break;
            case 2: lat[0] = maxLat; lon[0] -= step;
                if(lon[0] <= minLon) { lon[0] = minLon; rect_phase = 3; }
                break;
            case 3: lon[0] = minLon; lat[0] -= step;
                if(lat[0] <= minLat) { lat[0] = minLat; rect_phase = 0; }
                break;
        }

        // 🟢 Drone 2: diagonal
        lat[1] += dir2 * step;
        lon[1] += dir2 * step;

        if(lat[1] >= maxLat || lon[1] >= maxLon) {
            lat[1] = maxLat; lon[1] = maxLon; dir2 = -1;
        }
        if(lat[1] <= minLat || lon[1] <= minLon) {
            lat[1] = minLat; lon[1] = minLon; dir2 = 1;
        }

        // 🔴 Drone 3: diagonal opposite
        lat[2] += dir3 * step;
        lon[2] -= dir3 * step;

        if(lat[2] >= maxLat || lon[2] <= minLon) {
            lat[2] = maxLat; lon[2] = minLon; dir3 = -1;
        }
        if(lat[2] <= minLat || lon[2] >= maxLon) {
            lat[2] = minLat; lon[2] = maxLon; dir3 = 1;
        }

        for(int i = 0; i < 3; i++) {

            // Velocity
            vx[i] = lat[i] - prevLat[i];
            vy[i] = lon[i] - prevLon[i];
            vz[i] = 0;

            prevLat[i] = lat[i];
            prevLon[i] = lon[i];

            float speed = sqrt(vx[i]*vx[i] + vy[i]*vy[i]);
            float heading = atan2(vy[i], vx[i]) * 180 / 3.14159;

            time_t now = time(NULL);

            // 🔋 Battery drain
            battery[i] -= 0.3;
            if(battery[i] < 0) battery[i] = 0;

            // 🧠 Mode logic
            char *mode;
            if(battery[i] < 20)
                mode = "LOW_BATTERY";
            else if(speed < 0.0005)
                mode = "IDLE";
            else
                mode = "MOVING";

            // ❌ Random fault
            int gps_fault = (rand() % 50 == 0);

            // 🔹 Send data (same format + battery)
            sprintf(buffer, "%d,%f,%f,%f,%f,%f,%f,%f,%ld,%f",
                    i+1, lat[i], lon[i],
                    vx[i], vy[i], vz[i],
                    speed, heading, now, battery[i]);

            sendto(sock, buffer, strlen(buffer), 0,
                   (struct sockaddr*)&serverAddr, sizeof(serverAddr));

            // 🔥 SMART LOGGING (EVENT-BASED)
            printf("--------------------------------------------------\n");
            printf("[DRONE %d]\n", i+1);
            printf("Lat: %.4f | Lon: %.4f\n", lat[i], lon[i]);
            printf("Speed: %.5f | Heading: %.2f\n", speed, heading);
            printf("Battery: %.2f%% | Mode: %s\n", battery[i], mode);
            printf("Time: %s", ctime(&now));

            if(battery[i] < 20)
                printf("⚠️ LOW BATTERY ALERT!\n");

            if(speed > 0.001)
                printf("🚀 HIGH SPEED ALERT!\n");

            if(gps_fault)
                printf("❌ GPS SIGNAL LOST!\n");

            printf("--------------------------------------------------\n\n");
        }

        sleep(1);
    }

    close(sock);
    return 0;
}
