#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

#define BUFF_SIZE 512 /*buffer size:512 Bytes */
#define FMT8BITS AFMT_U8 /*unsigned 8 bits(for almost PC) */
#define FMT16BITS AFMT_S16_LE /*signed 16 bits,little endian */
#define FMT8K 8000 /*default sampling rate */
#define FMT11K 11025 /*11,22,44,48 are three pop rate */
#define FMT22K 22050
#define FMT44K 44100
#define FMT48K 48000
#define MONO 1
#define STEREO 2
int main(int argc, char *argv[]) {
    if (argc != 2)
        printf("input error! ./playvoice filename");
    int fd; //for device
    fd = open("/dev/dsp", O_WRONLY);
    if (fd < 0)
    {
        perror("Can't open /dev/dsp");
        return -1;
    }
    int outfile;
    outfile = open(argv[1], O_RDONLY);
    if (outfile < 0)
    {
        perror("Cannot open file for writing");
        return -1;
    }
    /* set bit format */
    int bits = FMT16BITS;
    if (ioctl(fd, SNDCTL_DSP_SETFMT, &bits) == -1)
    {
        fprintf(stderr, "Set fmt to bit %d failed:%s/n", bits,
                strerror(errno));
        return (-1);
    }
    if (bits != FMT16BITS)
    {
        fprintf(stderr, "do not support bit %d, supported 8、16/n", bits);
        return (-1);
    }
}