
#ifndef TemplateBlackX_h
#define TemplateBlackX_h

#include "Template.h"

namespace Quote {

class TemplateBlackX : public Template {
public:
  TemplateBlackX();
  virtual ~TemplateBlackX();
  virtual void render(cairo_surface_t*, char*, char*, PangoLayout *, cairo_t *, int);
};

} // namespace Quote.

#endif // TemplateBlackX_h
