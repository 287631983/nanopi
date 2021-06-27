#ifndef __V4L2_H
#define __V4L2_H

#include <linux/videodev2.h>
#include <linux/v4l2-controls.h>

#define NB_BUFFER   16
#define DHT_SIZE 420

#define V4L2_CLASS v4l2::Instance()

struct vdIn {
    int fd;
    char *videoDevice;
    char *status;
    char *picName;
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers rb;
    void *mem[NB_BUFFER];
    unsigned char *tmpBuffer;
    unsigned char *frameBuffer;
    bool isStreaming;
    int grabMethod;
    unsigned int width;
    unsigned int height;
    int formatIn;
    int formatOut;
    unsigned int frameSizeIn;
    int signalQuit;
    int toggleAvi;
    int getPict;

};

class v4l2 {
    public:
        static v4l2* Instance();
        int initVideoIn(struct vdIn *vd, char *device, int width, int height, int format, int grabmethod);
        int initV4l2 (struct vdIn *vd);
        int videoEnable(struct vdIn *vd);
        int closeV4l2(struct vdIn *vd);
        int uvcGrab(struct vdIn *vd);
        int v4l2GetControl(struct vdIn *vd, int control);
        int v4l2SetControl(struct vdIn *vd, int control, int value);
        int v4l2ResetControl(struct vdIn *vd, int control);
    private:
        static v4l2 *mInstance;
        int videoDisable(struct vdIn *vd);
        int isV4l2Control(struct vdIn *vd, int control, struct v4l2_queryctrl *queryctrl);
};

#endif