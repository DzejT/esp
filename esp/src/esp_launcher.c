#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <libusb-1.0/libusb.h>
#include <libserialport.h>
#include <signal.h>

#define VENDOR_ID 0x10c4
#define PRODUCT_ID 0xea60

struct devices{
    int address;
	char *port;
	pid_t pid;
};

void sigHandler(int signo);
static int run_loop = 1;

static struct devices active_devices [10];
static int count = 0;



void handle_left_event(struct libusb_device *dev, struct libusb_device_descriptor desc, libusb_device_handle **dev_handle);
int hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data);
void handle_arrive_event(struct libusb_device *dev, struct libusb_device_descriptor desc, libusb_device_handle **dev_handle);
void delete_from_array(int index);
void init_connected_devices();
int fork_program();

int main(void)
{	
	
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	openlog ("esp_controller", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

	libusb_hotplug_callback_handle callback_handle;

	int rc;
	libusb_init(NULL);

	init_connected_devices();
	
	rc = libusb_hotplug_register_callback(NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
											LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 0, VENDOR_ID, PRODUCT_ID,
											LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, NULL,
											&callback_handle);
	if (LIBUSB_SUCCESS != rc) {
		syslog(LOG_ERR, "Error creating a hotplug callback\n");
		libusb_exit(NULL);
		return EXIT_FAILURE;
	}


	while (run_loop) {
		libusb_handle_events_completed(NULL, NULL);
		sleep(1);
	}

	libusb_hotplug_deregister_callback(NULL, callback_handle);
	libusb_exit(NULL);
   
	return 0;

}

void init_connected_devices(){
	struct sp_port **ports;

	int rc = sp_list_ports(&ports);
    if(rc != SP_OK){
        syslog(LOG_ERR, "Error listing ports: %d\n", rc);
    }

    int bus;
    int address;
	int vid;
	int pid;
	char *port;
    for (int i = 0; ports[i] != NULL; i++) {
        sp_get_port_usb_bus_address(ports[i], &bus, &address);
		sp_get_port_usb_vid_pid(ports[i], &vid, &pid);
		if(strstr(sp_get_port_name(ports[i]), "ttyUSB") != NULL && vid == VENDOR_ID && pid == PRODUCT_ID){
			sp_get_port_usb_bus_address(ports[i], &bus, &address);
           	active_devices[count].port = sp_get_port_name(ports[i]);
			active_devices[count].address = address;
			fork_program();
        }
    }

	sp_free_port_list(ports);
}


int hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event, void *user_data) {

    libusb_device_handle *dev_handle = NULL;
	struct libusb_device_descriptor desc;
    (void)libusb_get_device_descriptor(dev, &desc);
    
	if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event) {
		handle_arrive_event(dev, desc, &dev_handle);
	} 


	else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event) {
		handle_left_event(dev, desc, &dev_handle);
	} 


	return 0;
}

void handle_arrive_event(struct libusb_device *dev, struct libusb_device_descriptor desc, libusb_device_handle **dev_handle){
    struct sp_port **ports;

    int rc;
    rc = libusb_open(dev, dev_handle);

    if (LIBUSB_SUCCESS != rc) {
        syslog(LOG_ERR,"Could not open USB device\n");
    }

    rc = sp_list_ports(&ports);
    if(rc != SP_OK){
        syslog(LOG_ERR,"Error listing ports: %d\n", rc);
    }

    int bus;
    int address;
	char *port;
    for (int i = 0; ports[i] != NULL; i++) {
        sp_get_port_usb_bus_address(ports[i], &bus, &address);
        if(libusb_get_device_address(dev) == address && strstr(sp_get_port_name(ports[i]), "ttyUSB") != NULL){
           	port = sp_get_port_name(ports[i]);
			break;
        }
    }

    syslog(LOG_INFO, "opened USB device\n");
	active_devices[count].port = port;
	active_devices[count].address = address;
	fork_program();
	sp_free_port_list(ports);
}

int fork_program(){
	pid_t pid;
	int status;

	if ( (pid = fork()) == -1 )
	{
		syslog(LOG_ERR, "Can't fork");
		return 1;
	}
	if (pid == 0)
	{
		if ( execl("/usr/bin/esp_ubus", "esp_ubus", active_devices[count].port, NULL) == -1 )
		{
			syslog(LOG_ERR, "Can't exec");
			return 1;
		}
	}
	else{
		signal(SIGCHLD,SIG_IGN);
		active_devices[count].pid = pid;
		count++;
	}
	
	return 0;
}

void handle_left_event(struct libusb_device *dev, struct libusb_device_descriptor desc, libusb_device_handle **dev_handle){
	int rc;
	for(int i = 0; i < count; i++){
		if(libusb_get_device_address(dev) == active_devices[i].address){
			kill(active_devices[i].pid, SIGTERM);
			delete_from_array(i);
			i--;
		}
	}
	syslog(LOG_ERR, "closed USB device\n");
	if (dev_handle) {
		libusb_close(*dev_handle);		
	}
	count++;
}

void delete_from_array(int index){
	for(int i = index; i < count-1; i++){
		active_devices[i] = active_devices[i+1];
	}
	count--;
}


void sigHandler(int signo) {
	syslog(LOG_INFO, "Received signal: %d\n", signo);
	for(int i = 0; i < count; i++){
		kill(active_devices[i].pid, SIGINT);
	}
	run_loop = 0;
}	
