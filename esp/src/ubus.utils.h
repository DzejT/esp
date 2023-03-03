#include <stdint.h>

int initSerial(int *fd, char *devpath);
char *getSerialBuff(int *fd, char *buffer);
char *handle_request(uint32_t pin, char *action);
void start_controller(void *args);

