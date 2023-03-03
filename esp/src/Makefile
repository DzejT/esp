CFLAGS:=-Wall -Wextra
LDGLAGS:=-Wl,--copy-dt-needed-entries -lubus -lusb-1.0 -lserialport

.PHONY: all clean

all: esp_launcher esp_ubus

esp_launcher: esp_launcher.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDGLAGS)

esp_ubus: esp_ubus.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDGLAGS)

clean:
	rm -f esp_launcher esp_ubus
 