#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <string.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer {
    void *start;
    size_t length;
};

static char *dev_name = "/dev/video0"; //摄像头设备名
static int fd = -1;
struct buffer *buffers = NULL;
static unsigned int n_buffers = 0;
static unsigned long file_length;
static unsigned char *file_name;

//获取一帧数据
static int read_frame(void) {
    static int iImageIndex = 0;
    char pcImageName[32] = {0};
    snprintf(pcImageName, sizeof(pcImageName), "%s-%d.jpg", "./tmp/image", iImageIndex++);
    FILE *file_fd1 = fopen(pcImageName, "w"); //图片文件名
    struct v4l2_buffer buf;
    unsigned int i;
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    int ff = ioctl(fd, VIDIOC_DQBUF, &buf);
    if (ff < 0)
        printf("failture\n"); //出列采集的帧缓冲
    assert(buf.index < n_buffers);
    //printf("buf.index dq is %d,\n", buf.index);
    fwrite(buffers[buf.index].start, buffers[buf.index].length, 1, file_fd1); //将其写入文件中
    ff = ioctl(fd, VIDIOC_QBUF, &buf);                                       //再将其入列
    if (ff < 0)
        printf("failture VIDIOC_QBUF\n");
    fclose(file_fd1);
    return 1;
}

int main(int argc, char **argv) {
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    unsigned int i;
    enum v4l2_buf_type type;
    fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0); //打开设备
    int ff = ioctl(fd, VIDIOC_QUERYCAP, &cap); //获取摄像头参数
    if (ff < 0)
        printf("failture VIDIOC_QUERYCAP\n");
    struct v4l2_fmtdesc fmt1;
    int ret;
    memset(&fmt1, 0, sizeof(fmt1));
    fmt1.index = 0;
    fmt1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt1)) == 0) {
        fmt1.index++;
        printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",
               fmt1.pixelformat & 0xFF, (fmt1.pixelformat >> 8) & 0xFF,
               (fmt1.pixelformat >> 16) & 0xFF, (fmt1.pixelformat >> 24) & 0xFF,
               fmt1.description);
    }
    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 1080;
    fmt.fmt.pix.height = 960;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG; //V4L2_PIX_FMT_YUYV;//V4L2_PIX_FMT_YVU420;//V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    ff = ioctl(fd, VIDIOC_S_FMT, &fmt); //设置图像格式
    if (ff < 0)
        printf("failture VIDIOC_S_FMT\n");
    file_length = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height; //计算图片大小
    struct v4l2_requestbuffers req;
    CLEAR(req);
    req.count = 3;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ff = ioctl(fd, VIDIOC_REQBUFS, &req); //申请缓冲，count是申请的数量
    if (ff < 0)
        printf("failture VIDIOC_REQBUFS\n");
    if (req.count < 1)
        printf("Insufficient buffer memory\n");
    
    printf("req.count: %d\n", req.count);
    buffers = calloc(req.count, sizeof(*buffers)); //内存中建立对应空间
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        struct v4l2_buffer buf; //驱动中的一帧
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;
        if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf)) //映射用户空间
            printf("VIDIOC_QUERYBUF error\n");
        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
            mmap(NULL /* start anywhere */, //通过mmap建立映射关系
                 buf.length,
                 PROT_READ | PROT_WRITE /* required */,
                 MAP_SHARED /* recommended */,
                 fd, buf.m.offset);
        if (MAP_FAILED == buffers[n_buffers].start)
            printf("mmap failed\n");
    }
    for (i = 0; i < n_buffers; ++i)
    {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (-1 == ioctl(fd, VIDIOC_QBUF, &buf)) //申请到的缓冲进入列队
            printf("VIDIOC_QBUF failed\n");
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl(fd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据
        printf("VIDIOC_STREAMON failed\n");
    for (;;) //这一段涉及到异步IO
    {
        fd_set fds;
        struct timeval tv;
        int r;
        FD_ZERO(&fds); //将指定的文件描述符集清空
        FD_SET(fd, &fds); //在文件描述符集合中增加一个新的文件描述符
        /* Timeout. */
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        r = select(fd + 1, &fds, NULL, NULL, &tv); //判断是否可读（即摄像头是否准备好），tv是定时
        if (-1 == r) {
            if (EINTR == errno)
                continue;
            printf("select err\n");
        }
        if (0 == r){
            fprintf(stderr, "select timeout\n");
            exit(EXIT_FAILURE);
        }
        if (read_frame()); //如果可读，执行read_frame ()函数，并跳出循环
            //break;
    }
unmap:
    for (i = 0; i < n_buffers; ++i)
        if (-1 == munmap(buffers[i].start, buffers[i].length))
            printf("munmap error");
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))
        printf("VIDIOC_STREAMOFF");
    close(fd);
    exit(EXIT_SUCCESS);
    return 0;
}



