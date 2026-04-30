import socket
import matplotlib.pyplot as plt
import math

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", 5006))

drones = {}
trail = {}

THRESHOLD = 0.005

def distance(d1, d2):
    return math.sqrt((d1[0] - d2[0])**2 + (d1[1] - d2[1])**2)

plt.ion()
print("🚀 Ground Station Running...")

while True:
    data, _ = sock.recvfrom(1024)
    msg = data.decode()
    parts = msg.split(",")

    # ✅ unpack data (FIXED INDENTATION)
    drone_id = int(parts[0])
    lat = float(parts[1])
    lon = float(parts[2])
    vx = float(parts[3])
    vy = float(parts[4])
    vz = float(parts[5])
    speed = float(parts[6])
    heading = float(parts[7])
    timestamp = int(parts[8])

    # ✅ store data (including timestamp)
    drones[drone_id] = (lat, lon, speed, heading, timestamp)

    # 🔥 trail
    if drone_id not in trail:
        trail[drone_id] = []

    trail[drone_id].append((lat, lon))

    if len(trail[drone_id]) > 20:
        trail[drone_id].pop(0)

    plt.clf()

    # ✅ FIXED colors initialization
    colors = {d: "green" for d in drones}

    # 🔥 collision detection (FIXED)
    for i in drones:
        for j in drones:
            if i < j:
                d = distance(drones[i][:2], drones[j][:2])
                if d < THRESHOLD:
                    print(f"⚠️ Collision Risk: Drone {i} & {j}")

                    colors[i] = "red"
                    colors[j] = "red"

                    mid_lon = (drones[i][1] + drones[j][1]) / 2
                    mid_lat = (drones[i][0] + drones[j][0]) / 2

                    plt.scatter(mid_lon, mid_lat, color='red', s=100, marker='x')
                    plt.text(mid_lon, mid_lat, "COLLISION", color='red')

    # 🔥 draw trail
    for d_id in trail:
        lats = [p[0] for p in trail[d_id]]
        lons = [p[1] for p in trail[d_id]]
        plt.plot(lons, lats, linewidth=2)

    # 🔥 fixed axis (MUST be after clf)
    plt.xlim(77.49, 77.61)
    plt.ylim(12.89, 13.01)

    # 🔥 plot drones
    for d_id, (lat, lon, speed, heading, timestamp) in drones.items():
        plt.scatter(lon, lat, color=colors[d_id], s=60)

        # direction arrow
        dx = 0.002 * speed * math.cos(math.radians(heading))
        dy = 0.002 * speed * math.sin(math.radians(heading))

        plt.arrow(lon, lat, dx, dy, head_width=0.0002)

        # text info
        plt.text(lon, lat,
                 f"D{d_id}\nS:{speed:.5f}\nH:{heading:.1f}\nT:{timestamp}",
                 fontsize=8)

    plt.xlabel("Longitude")
    plt.ylabel("Latitude")
    plt.title("Drone Monitoring System (Speed + Heading)")
    plt.pause(0.1)