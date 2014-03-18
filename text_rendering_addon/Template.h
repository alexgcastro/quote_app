
#ifndef Template_h
#define Template_h

#include <pango/pangocairo.h>

namespace Quote {

class Template {
public:
  virtual ~Template() {};
  virtual void render(cairo_surface_t*, char*, char* , PangoLayout *, cairo_t *, int) = 0;
};

} // namespace Quote.

#endif // Template_h
