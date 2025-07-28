#include <libserialport.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    struct sp_port **ports;
    enum sp_return rc;

    rc = sp_list_ports(&ports);
    if (rc != SP_OK) {
        fprintf(stderr, "sp_list_ports failed\n");
        return 1;
    }

    if (!ports[0]) {
        fprintf(stderr, "No serial port found\n");
        sp_free_port_list(ports);
        return 1;
    }

    struct sp_port *port = ports[0];
    printf("Using port: %s\n", sp_get_port_name(port));

    sp_set_baudrate(port, 115200);
    sp_set_bits(port, 8);
    sp_set_parity(port, SP_PARITY_NONE);
    sp_set_stopbits(port, 1);

    if (sp_open(port, SP_MODE_READ_WRITE) != SP_OK) {
        fprintf(stderr, "Cannot open port\n");
        sp_free_port_list(ports);
        return 1;
    }

    const char *msg = "Hello\r\n";
    sp_blocking_write(port, msg, strlen(msg), 1000);

    sp_close(port);
    sp_free_port_list(ports);
    printf("Done.\n");
    return 0;
}