#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "common_func.h"
#include "v4l2.h"

static unsigned char dht_data[DHT_SIZE] = {
    0xff, 0xc4, 0x01, 0xa2, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
    0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x01, 0x00, 0x03,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05,
    0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d, 0x01, 0x02, 0x03, 0x00, 0x04,
    0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22,
    0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15,
    0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
    0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66,
    0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
    0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95,
    0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
    0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2,
    0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5,
    0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
    0xfa, 0x11, 0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05,
    0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 0x01, 0x02, 0x03, 0x11, 0x04,
    0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22,
    0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33,
    0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25,
    0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
    0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66,
    0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
    0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94,
    0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba,
    0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
    0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa
};

v4l2* v4l2::mInstance = nullptr;

v4l2* v4l2::Instance()
{
    if (mInstance == nullptr) {
        mInstance = new v4l2();
    }
    return mInstance;
}

int v4l2::initV4l2(struct vdIn *vd)
{
    int i;
    int ret = 0;

    if ((vd->fd = open(vd->videoDevice, O_RDWR)) == -1) {
        LOGE("cannot open %s", vd->videoDevice);
        return -1;
    }
    memset(&vd->cap, 0, sizeof(struct v4l2_capability));
    ret = ioctl(vd->fd, VIDIOC_QUERYCAP, &vd->cap);
    if (ret < 0) {
        LOGE("cannot query video device:%s", vd->videoDevice);
        goto error;
    }

    if ((vd->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
        LOGE("video capture not support:%d", vd->cap.capabilities);
        goto error;
    }

    if (vd->grabMethod) {
        if (!(vd->cap.capabilities & V4L2_CAP_STREAMING)) {
            LOGE("video device:%s does not support streaming i/o", vd->videoDevice);
            goto error;
        } else {
            if (!(vd->cap.capabilities & V4L2_CAP_READWRITE)) {
                LOGE("video device:%s does not support read i/o", vd->videoDevice);
                // goto error;
            }
        }
    }

    /* set format in */
    memset(&vd->fmt, 0, sizeof(struct v4l2_format));
    vd->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vd->fmt.fmt.pix.width = vd->width;
    vd->fmt.fmt.pix.height = vd->height;
    vd->fmt.fmt.pix.pixelformat = vd->formatIn;
    vd->fmt.fmt.pix.field = V4L2_FIELD_ANY;
    ret = ioctl(vd->fd, VIDIOC_S_FMT, &vd->fmt);
    if (ret < 0) {
        LOGE("video device:%s set format failed!", vd->videoDevice);
        goto error;
    }
    if ((vd->fmt.fmt.pix.width != vd->width) || (vd->fmt.fmt.pix.height != vd->height)) {
        LOGE("video device:%s get unexpect width:%d or height:%d", vd->videoDevice, vd->fmt.fmt.pix.width, vd->fmt.fmt.pix.height);
        vd->width = vd->fmt.fmt.pix.width;
        vd->height = vd->fmt.fmt.pix.height;
    }

    /* request buffers */
    memset(&vd->rb, 0, sizeof(struct v4l2_requestbuffers));
    vd->rb.count = NB_BUFFER;
    vd->rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vd->rb.memory = V4L2_MEMORY_MMAP;

    ret = ioctl(vd->fd, VIDIOC_REQBUFS, &vd->rb);
    if (ret < 0) {
        LOGE("video device:%s request buff failed!", vd->videoDevice);
        goto error;
    }

    /* map the buffers */
    for (i = 0; i < NB_BUFFER; ++i) {
        memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
        vd->buf.index = i;
        vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        vd->buf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(vd->fd, VIDIOC_QUERYBUF, &vd->buf);
        if (ret < 0) {
            LOGE("query buffer failed:%d", errno);
            goto error;
        }
        vd->mem[i] = mmap(0, vd->buf.length, PROT_READ, MAP_SHARED, vd->fd, vd->buf.m.offset);
        if (vd->mem[i] == MAP_FAILED) {
            LOGE("map buffer failed:%d", errno);
            goto error;
        }
    }

    /* the buffers */
    for (i = 0; i < NB_BUFFER; ++i) {
        memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
        vd->buf.index = i;
        vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        vd->buf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(vd->fd, VIDIOC_QBUF, &vd->buf);
        if (ret < 0) {
            LOGE("queue buffer failed:%d", errno);
            goto error;
        }
    }
    return 0;
error:
    return -1;
}

int v4l2::closeV4l2(struct vdIn *vd)
{
    int i;

    if (vd->isStreaming) {
        videoDisable(vd);
    }

    /* if the memory maps are not released the device will remain opened even after a call to close() */
    for (i = 0; i < NB_BUFFER; ++i) {
        munmap(vd->mem[i], vd->buf.length);
    }

    if (vd->tmpBuffer != nullptr) {
        free(vd->tmpBuffer);
    }
    vd->tmpBuffer = nullptr;
    free(vd->frameBuffer);
    vd->frameBuffer = nullptr;
    free(vd->videoDevice);
    free(vd->status);
    free(vd->picName);
    vd->videoDevice = nullptr;
    vd->status = nullptr;
    vd->picName = nullptr;
    close(vd->fd);
    return 0;
}

int v4l2::initVideoIn(struct vdIn *vd, char *device, int width, int height, int format, int grabmethod)
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
    if (this->initV4l2(vd) < 0) {
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

int v4l2::videoEnable(struct vdIn *vd)
{
    int ret;
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(vd->fd, VIDIOC_STREAMON, &type);
    if (ret < 0) {
        LOGE("video device:%s set streamon failed:%d", vd->videoDevice, errno);
        return ret;
    }
    vd->isStreaming = true;
    return 0;
}

int v4l2::videoDisable(struct vdIn *vd)
{
    int ret;
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(vd->fd, VIDIOC_STREAMOFF, &type);
    if (ret < 0) {
        LOGE("video device:%s set streamoff failed:%d", vd->videoDevice, errno);
        return ret;
    }
    vd->isStreaming = false;
    return 0;
}

int v4l2::uvcGrab(struct vdIn *vd)
{
    #define HEADERFRAME1 0xaf
    int ret;

    if (vd->isStreaming == false) {
        if (videoEnable(vd)) {
            goto error;
        }
    }
    memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
    vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vd->buf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(vd->fd, VIDIOC_DQBUF, &vd->buf);
    if (ret < 0) {
        LOGE("dequeue buffer failed:%d", errno);
        goto error;
    }

    switch (vd->formatIn) {
        case V4L2_PIX_FMT_MJPEG:
            memcpy(vd->tmpBuffer, vd->mem[vd->buf.index], HEADERFRAME1);
            memcpy(vd->tmpBuffer + HEADERFRAME1, dht_data, DHT_SIZE);
            memcpy(vd->tmpBuffer + HEADERFRAME1 + DHT_SIZE, (unsigned char *)vd->mem[vd->buf.index] + HEADERFRAME1, vd->buf.bytesused - HEADERFRAME1);
            break;
        case V4L2_PIX_FMT_YUYV:
            if (vd->buf.bytesused > vd->frameSizeIn) {
                memcpy(vd->frameBuffer, vd->mem[vd->buf.index], (size_t)vd->frameSizeIn);
            } else {
                memcpy(vd->frameBuffer, vd->mem[vd->buf.index], (size_t)vd->buf.bytesused);
            }
            break;
        default:
            goto error;
            break;
    }
    ret = ioctl(vd->fd, VIDIOC_QBUF, &vd->buf);
    if (ret < 0) {
        LOGE("requeue buffer failed:%d", errno);
        goto error;
    }
    return 0;
error:
    vd->signalQuit = 0;
    return -1;
}

int v4l2::isV4l2Control(struct vdIn *vd, int control, struct v4l2_queryctrl *queryctrl)
{
    queryctrl->id = control;
    if (ioctl(vd->fd, VIDIOC_QUERYCTRL, queryctrl) < 0) {
        LOGE("ioctl querycontrol error:%d", errno);
    } else if (queryctrl->flags & V4L2_CTRL_FLAG_DISABLED) {
        LOGE("control %s disabled", (char *)queryctrl->name);
    } else if (queryctrl->flags & V4L2_CTRL_TYPE_BOOLEAN) {
        return 1;
    } else if (queryctrl->flags & V4L2_CTRL_TYPE_INTEGER) {
        return 0;
    } else {
        LOGE("control %s unsupported", (char *)queryctrl->name);
    }
    return -1;
}

int v4l2::v4l2GetControl(struct vdIn *vd, int control)
{
    struct v4l2_queryctrl queryctrl;
    struct v4l2_control control_s;

    if (isV4l2Control(vd, control, &queryctrl) < 0) {
        return -1;
    }
    control_s.id = control;
    if (ioctl(vd->fd, VIDIOC_G_CTRL, &control_s) < 0) {
        LOGE("ioctl get control error");
        return -1;
    }
    return control_s.value;
}

int v4l2::v4l2SetControl(struct vdIn *vd, int control, int value)
{
    struct v4l2_control control_s;
    struct v4l2_queryctrl queryctrl;
    int min, max;

    if (isV4l2Control(vd, control, &queryctrl) < 0) {
        return -1;
    }
    min = queryctrl.minimum;
    max = queryctrl.maximum;
    if ((value >= min) && (value <= max)) {
        control_s.id = control;
        control_s.value = value;
        if (ioctl (vd->fd, VIDIOC_S_CTRL, &control_s) < 0) {
            LOGE("ioctl set control error");
            return -1;
        }
    }
    return 0;
}

int v4l2::v4l2ResetControl(struct vdIn *vd, int control)
{
  struct v4l2_control control_s;
  struct v4l2_queryctrl queryctrl;
  int val_def;

    if (isV4l2Control(vd, control, &queryctrl) < 0) {
        return -1;
    }
    val_def = queryctrl.default_value;
    control_s.id = control;
    control_s.value = val_def;
    if (ioctl(vd->fd, VIDIOC_S_CTRL, &control_s) < 0) {
    LOGE("ioctl reset control error");
        return -1;
    }
    return 0;
}