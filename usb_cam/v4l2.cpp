#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/videodev.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "v4l2.h"

int v4l2::init_videoIn(struct vdIn *vd, char *device, int width, int height, int format, int grabmethod)
{
    if (vd == nullptr || device == nullptr) {
        return -1;
    }
    if (width == 0 || height == 0) {
        return -1;
    }
    if (grabmethod < 0 || grabmethod > 1) {
        grabmethod = 1;
    }

    vd->videoDevice = nullptr;
    vd->status = nullptr;
    vd->picName = nullptr;
    vd->videoDevice = (char *)calloc(1, 16 * sizeof(char));
    vd->status = (char *)calloc(1, 100 * sizeof(char));
    vd->picName = (char *)calloc(1, 80 * sizeof(char));
    snprintf(vd->videoDevice, 12, "%s", device);
    vd->toggleAvi = 0;
    vd->getPict = 0;
    vd->signalQuit = 1;
    vd->width = width;
    vd->height = height;
    vd->formatIn = format;
    vd->grabMethod = grabmethod;
    if (this->init_v4l2(vd) < 0) {
        LOGE("Init v4l2 failed!");
        goto error;
    }
    /* alloc a temp buffer to reconstruct the pict */
    vd->frameSizeIn = vd->width * vd->height << 1;
    switch (vd->formatIn) {
        case V4L2_PIX_FMT_MJPEG:
            vd->tmpBuffer = (unsigned char *)calloc(1, vd->frameSizeIn);
            if (!vd->tmpBuffer) {
                goto error;
            }
            vd->frameBuffer = (unsigned char *)calloc(1, vd->width * (vd->height + 8) * 2);
            break;
        case V4L2_PIX_FMT_YUYV:
            vd->frameBuffer = (unsigned char *)calloc(1, vd->frameSizeIn);
            break;
        default:
            LOGE("unsupport this format!");
            goto error;
            break;
    }
    if (vd->frameBuffer == nullptr) {
        goto error;
    }
    return 0;
error:
    free(vd->videoDevice);
    free(vd->status);
    free(vd->picName);
    close(vd->fd);
    return -1;
}