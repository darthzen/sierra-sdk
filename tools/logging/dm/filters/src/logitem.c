#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include "crc.h"
#include "ba.h"
#define BUFLEN  (0x1000)
#define LOG_HDR_LEN (16)
#define DM_PORT "/dev/ttyUSB0"
#define QMI_1_TX    (0x38f)
#define QMI_APP "~/r/3-cir/pkgs/qa/qatesthostx86_64 -nt411"

typedef struct __attribute__((__packed__)){
    uint32_t cmd_code;
    uint32_t operation;
    uint32_t equip_id;
    uint32_t num_items;
}logconf_s;

unsigned
tail(uint8_t *buf, unsigned sz)
{
    crc_t crc;
    crc = crc_init();
    crc = crc_update(crc, (void*)buf, sz);
    crc = crc_finalize(crc);
    printf("crc: 0x%04x\n", (unsigned)crc);
    buf[sz] = crc & 0xff;
    buf[sz+1] = crc >> 8;
    buf[sz+2] = 0x7e;
    return sz+3;
}

int
main(int argc)
{
    unsigned sz;
    bitarray_s bits;
    uint8_t *buf = malloc(BUFLEN);
    logconf_s *conf = (logconf_s*)buf;
    conf->cmd_code = 0x73;
    conf->operation = 3;
    conf->equip_id = 1;
    conf->num_items = QMI_1_TX+1;

    bitarray_init(&bits, &buf[LOG_HDR_LEN], BUFLEN);
    if (argc==1)
        bitarray_set(&bits, QMI_1_TX);

    sz = tail(buf, sizeof(logconf_s) + (conf->num_items+7)/8);

    //raw mode required
    system("stty raw -F " DM_PORT);

    int fd = open(DM_PORT, O_RDWR);

    printf("written: %d\n", write(fd, buf, sz ));

    system(QMI_APP " > /dev/null 2>&1");

    int len;
    while(1)
    {
        if ((len=read(fd, buf, BUFLEN))!=0)
        {
            const char delimter = 0x7e;
            char* token;
            token = strtok(buf, &delimter);
            while(token != NULL)
            {
                if (buf[0] == 0x10)
                {
                    printf("%02x %02x\n", buf[6], buf[7]);
                    if ( buf[7]<<8|buf[6] == QMI_1_TX)
                        printf("qmi tx log received\n");
                    exit(0);
                }
                token = strtok(NULL, &delimter);
            }
        }
    }
    close(fd);
}
