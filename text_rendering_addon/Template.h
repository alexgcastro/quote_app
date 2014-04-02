
#ifndef Template_h
#define Template_h

#include <pango/pangocairo.h>
#include <v8.h>

namespace Quote {

class Template {
public:
  virtual ~Template() {};
  virtual void render(cairo_surface_t*, char*, char* , PangoLayout *, cairo_t *, int) = 0;

  static Template* createTemplate(uint32_t);
};

} // namespace Quote.

#endif // Template_h
