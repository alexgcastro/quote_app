
#ifndef Template_h
#define Template_h

#include <pango/pangocairo.h>
#include <v8.h>

namespace Quote {

class Template {
public:
  virtual ~Template() {};
  virtual void render(cairo_surface_t*, char*, char* , PangoLayout *, cairo_t *, int) = 0;

  static const uint32_t BlueX = 1;
  static const uint32_t BlackX = 2;
};

} // namespace Quote.

#endif // Template_h
