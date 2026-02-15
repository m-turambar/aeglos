#include <cstdint>
#include <cstring>
#include <vector>

#if defined(LINUX) || defined(__linux__) || defined(__linux)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
// robo! calumnia!
void ImageFromDisplay(std::vector<uint8_t>& Pixels, int& Width, int& Height, int& BitsPerPixel)
{
  Display* display = XOpenDisplay(nullptr);
  Window root = DefaultRootWindow(display);

  XWindowAttributes attributes = {0};
  XGetWindowAttributes(display, root, &attributes);

  //Width = attributes.width;
  Width = 1920;
  Height = attributes.height;

  XImage* img = XGetImage(display, root, 0, 0 , Width, Height, AllPlanes, ZPixmap);
  BitsPerPixel = img->bits_per_pixel;
  Pixels.resize(Width * Height * 4);

  memcpy(&Pixels[0], img->data, Pixels.size());

  XDestroyImage(img);
  XCloseDisplay(display);
}
#else
void ImageFromDisplay(std::vector<uint8_t>& Pixels, int& Width, int& Height, int& BitsPerPixel)
{
    
}
#endif