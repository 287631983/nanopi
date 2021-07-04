#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <linux/videodev2.h>
#include "video.h"
#include "v4l2.h"
#include "jpeglib.h"
#include "common_func.h"

int run = 1;

int compress_yuyv_to_jpeg (struct vdIn *vd, FILE * file, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  unsigned char *line_buffer, *yuyv;
  int z;

  line_buffer = (unsigned char *)calloc (vd->width * 3, 1);
  yuyv = vd->frameBuffer;

  cinfo.err = jpeg_std_error (&jerr);
  jpeg_create_compress (&cinfo);
  jpeg_stdio_dest (&cinfo, file);

  cinfo.image_width = vd->width;
  cinfo.image_height = vd->height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults (&cinfo);
  jpeg_set_quality (&cinfo, quality, TRUE);

  jpeg_start_compress (&cinfo, TRUE);

  z = 0;
  while (cinfo.next_scanline < cinfo.image_height) {
    int x;
    unsigned char *ptr = line_buffer;

    for (x = 0; x < vd->width; x++) {
      int r, g, b;
      int y, u, v;

      if (!z)
	y = yuyv[0] << 8;
      else
	y = yuyv[2] << 8;
      u = yuyv[1] - 128;
      v = yuyv[3] - 128;

      r = (y + (359 * v)) >> 8;
      g = (y - (88 * u) - (183 * v)) >> 8;
      b = (y + (454 * u)) >> 8;

      *(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
      *(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
      *(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

      if (z++) {
	z = 0;
	yuyv += 4;
      }
    }

    row_pointer[0] = line_buffer;
    jpeg_write_scanlines (&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress (&cinfo);
  jpeg_destroy_compress (&cinfo);

  free (line_buffer);

  return (0);
}

int main(int argc, char **argv)
{
  char *videodevice = "/dev/video0";
  char *outputfile = "snap.jpg";
  char *post_capture_command[3];
  int format = V4L2_PIX_FMT_MJPEG;
  int grabmethod = 1;
  int width = 320;
  int height = 240;
  int brightness = 0, contrast = 0, saturation = 0, gain = 0;
  int verbose = 0;
  int delay = 0;
  int quality = 95;
  int post_capture_command_wait = 0;
  time_t ref_time;
  struct vdIn *videoIn;
  FILE *file;

  // set post_capture_command to default values
  post_capture_command[0] = NULL;
  post_capture_command[1] = NULL;
  post_capture_command[2] = NULL;
    videoIn = (struct vdIn *) calloc (1, sizeof (struct vdIn));
  if (V4L2_CLASS->initVideoIn(videoIn, (char *) videodevice, width, height, format, grabmethod) < 0)
    exit (1);

unsigned char temp[256];
while(true) {
  if (V4L2_CLASS->uvcGrab (videoIn) < 0) {
    LOGE("uvc grab error!");
    V4L2_CLASS->closeV4l2(videoIn);
    return;
  }
  // file = fopen (outputfile, "wb");
  // fwrite (videoIn->frameBuffer, width*height*4, 1,file);
  // fclose (file);
  for (int i = 0; i < sizeof(temp); i++) {
    if (videoIn->frameBuffer[i] > 128)
      temp[i] = 0x31;
      else
        temp[i] =0x32;
  }
  videoIn->getPict = 0;
  printf("%s",temp);
  usleep(50*1000);
}

  //   while (run) {
  //   if (verbose >= 2)
  //     fprintf (stderr, "Grabbing frame\n");
  //   if (V4L2_CLASS->uvcGrab (videoIn) < 0) {
  //     fprintf (stderr, "Error grabbing\n");
  //     V4L2_CLASS->closeV4l2 (videoIn);
  //     free (videoIn);
  //     exit (1);
  //   }

  //   if ((difftime (time (NULL), ref_time) > delay) || delay == 0) {
  //       if (verbose >= 1)
  //           fprintf (stderr, "Saving image to: %s\n", outputfile);
  //       file = fopen (outputfile, "wb");
  //       if (file != NULL) {
  //       switch (videoIn->formatIn) {
  //           case V4L2_PIX_FMT_YUYV:
  //               compress_yuyv_to_jpeg (videoIn, file, quality);
  //               break;
  //           default:
  //               fwrite (videoIn->tmpBuffer, videoIn->buf.bytesused + DHT_SIZE, 1,
  //               file);
  //           break;
  //       }
  //       fclose (file);
  //       videoIn->getPict = 0;
  //     }

  //     ref_time = time (NULL);
  //   }
  //   if (delay == 0)
  //     break;
  // }
  V4L2_CLASS->closeV4l2 (videoIn);
  free (videoIn);

    return 0;
}