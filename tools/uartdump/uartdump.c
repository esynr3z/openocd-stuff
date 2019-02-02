#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define PPRINT(...) fprintf(stderr, "uartdump> " __VA_ARGS__)

unsigned int dump_line = 0;
char line_str[1024] = "";

static void strcatc(char* s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

int main(int argc, char** argv)
{
    FILE* f = NULL;
    int c;
    int fd;
    int int_baud;
    speed_t baud = B9600;
    int rts_flag, dtr_flag;
    struct termios settings;

    // parse arguments
    while ((c = getopt(argc, argv, "f:b:")) != EOF) {
        switch (c) {
        case 'f':
            /* e.g. from UART connected to /dev/ttyUSB0 */
            fd = open(optarg, O_RDWR | O_NOCTTY);
            if (fd == -1) {
                perror(optarg);
                return 1;
            }
            break;
        case 'b':
            int_baud = atoi(optarg);
            if (int_baud == 9600)
                baud = B9600;
            else if (int_baud == 115200)
                baud = B115200;
            else {
                PPRINT("Unknown baudrate %s!\n", optarg);
                return 1;
            }
            break;
        default:
            printf("usage: uartdump -f /dev/ttyUSB0 -b 115200\n");
            return 1;
        }
    }

    // some boards have reset and boot pins connected to rts and dtr
    // make sure that they are in inactive state
    rts_flag = TIOCM_RTS; // RTS=0
    dtr_flag = TIOCM_DTR; // DTR=0
    ioctl(fd, TIOCMBIC, &rts_flag);
    ioctl(fd, TIOCMBIC, &dtr_flag);

    // configure
    tcgetattr(fd, &settings);
    cfsetospeed(&settings, baud);
    settings.c_cflag &= ~PARENB; // no parity
    settings.c_cflag &= ~CSTOPB; // 1 stop bit
    settings.c_cflag &= ~CSIZE;
    settings.c_cflag |= CS8 | CLOCAL; // 8 bits
    settings.c_lflag = ICANON;        // canonical mode
    settings.c_oflag &= ~OPOST;       // raw output
    tcsetattr(fd, TCSANOW, &settings);
    tcflush(fd, TCOFLUSH);

    f = fdopen(fd, "r");
    if (!f) {
        PPRINT("Open error!\n");
        return 1;
    }

    while ((c = getc(f)) != EOF) {
        if (c != '\r') {
            strcatc(line_str, c);
            if (c == '\n') {
                PPRINT("%s", line_str);
                line_str[0] = '\0';
            }
        }
    }

    return 0;
}
