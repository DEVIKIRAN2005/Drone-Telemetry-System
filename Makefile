# Compiler
CC = qcc

# Flags
CFLAGS = -Wall
LDFLAGS = -lsocket -lm

# Targets
TARGET1 = DroneSender
TARGET2 = client

# Source files
SRC1 = src/drone_sender.c
SRC2 = src/client.c

# Default build
all: $(TARGET1) $(TARGET2)

# Build Drone Sender
$(TARGET1): $(SRC1)
	$(CC) $(SRC1) -o $(TARGET1) $(LDFLAGS)

# Build Client
$(TARGET2): $(SRC2)
	$(CC) $(SRC2) -o $(TARGET2) $(LDFLAGS)

# Clean build files
clean:
	rm -f $(TARGET1) $(TARGET2)