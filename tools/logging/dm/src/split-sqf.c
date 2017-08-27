//utility split SQF filter by command tail (0x7e) then send to DM port
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#define MAX_FILTER_SZ (10*1024*1024)
#define MAX_RESP_SZ (1024)
#define TAIL (0x7e)
#define VER_LEN (3)

void set_tty_raw_mode(int fd)
{
    /* Reconfigure the serial port */
    struct termios newtio;
    memset(&newtio, 0, sizeof(newtio));
    newtio.c_cflag = CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;

    /* Select the read() termination parameters as follows:
     * Intercharacter timeout = 1/10th of a second.
     * Minimum number of characters read = 1
     *
     * This is to be interpreted as follows:
     * The intercharacter timer is not started until the
     * first character is received. Then after VTIME 1/10ths
     * of a second, if no additional characters have been
     * received, return what we've got so far. The read()
     * call will also return when a minimum of VMIN chars
     * have been received.
     */
    newtio.c_cc[VTIME]    = 1;  /* inter-character timer */
    newtio.c_cc[VMIN]     = 1; /* blocking read until n chars received */

    tcflush(fd, TCIFLUSH);

    if( tcsetattr(fd, TCSANOW, &newtio) < 0 )
    {
        printf("USB tcsetattr error: %d", errno);
    }
}

int
main(int argc, char** argv) {

    int olen, ilen;
    uint8_t *start, *stop;
    int osize;
    int tty = open(argv[2], O_RDWR );
    if (tty == -1)
    {
        exit(1);
    }
    FILE* filter_fd = fopen(argv[1], "rb");
    if (filter_fd == NULL)
    {
        close(tty);
        exit(1);
    }

    uint8_t *buf = malloc(MAX_FILTER_SZ);
    if (buf == NULL)
    {
        fclose(filter_fd);
        close(tty);
        exit(1);
    }

    uint8_t *resp = malloc(MAX_RESP_SZ);
    if (resp == NULL)
    {
        free(buf);
        fclose(filter_fd);
        close(tty);
        exit(1);
    }

    osize = fread(buf, 1, MAX_FILTER_SZ, filter_fd);

    set_tty_raw_mode(tty);

    start = buf + VER_LEN;
    stop = start;
    while( (stop - start) < osize) {
        if ( *stop != TAIL) {
            stop++;
        } else {
            olen = write(tty, start, stop-start+1);
            ilen = read(tty, resp, MAX_RESP_SZ);
            //printf("olen, ilen: %d, %d\n", olen, ilen);
            start = stop+1;
            stop = start;
        }
    }

    free(resp);
    free(buf);
    fclose(filter_fd);
    close(tty);
    return 0;
}
