#ifndef __V4L2_H
#define __V4L2_H

#define NB_BUFFER   16
#define V4L2_CID_BACKLIGHT_COMPENSATION	(V4L2_CID_PRIVATE_BASE+0)
#define V4L2_CID_POWER_LINE_FREQUENCY	(V4L2_CID_PRIVATE_BASE+1)
#define V4L2_CID_SHARPNESS		(V4L2_CID_PRIVATE_BASE+2)
#define V4L2_CID_HUE_AUTO		(V4L2_CID_PRIVATE_BASE+3)
#define V4L2_CID_FOCUS_AUTO		(V4L2_CID_PRIVATE_BASE+4)
#define V4L2_CID_FOCUS_ABSOLUTE		(V4L2_CID_PRIVATE_BASE+5)
#define V4L2_CID_FOCUS_RELATIVE		(V4L2_CID_PRIVATE_BASE+6)

#define V4L2_CID_PANTILT_RELATIVE	(V4L2_CID_PRIVATE_BASE+7)
#define V4L2_CID_PANTILT_RESET		(V4L2_CID_PRIVATE_BASE+8)

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
    int isStreaming;
    int grabMethod;
    int width;
    int height;
    int formatIn;
    int formatOut;
    int frameSizeIn;
    int signalQuit;
    int toggleAvi;
    int getPict;

}
class v4l2 {
    public:
        int init_videoIn(struct vdIn *vd, char *device, int width, int height, int format, int grabmethod);
        int uvcGrab(struct vdIn *vd);
        int init_v4l2 (struct vdIn *vd);
        int closeV4l2(struct vdIn *vd);

        int v4l2GetControl(struct vdIn *vd, int control);
        int v4l2SetControl(struct vdIn *vd, int control, int value);
        int v4l2UpControl(struct vdIn *vd, int control);
        int v4l2DownControl(struct vdIn *vd, int control);
        int v4l2ToggleControl(struct vdIn *vd,, int control);
        int v4l2ResetControl(struct vdIn *vd,, int control);
    private:
}

#endif