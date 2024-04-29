#ifndef PXLS
#define PXLS

namespace pxls {
    inline float to_float(int pixels, int total_pixels) {
      return (float)pixels / total_pixels;
    }
}

#endif // !PXLS
