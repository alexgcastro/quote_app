
#include "Template.h"

namespace Quote {

class TemplateBlueX : public Template {
public:
  TemplateBlueX();
  virtual ~TemplateBlueX();
  virtual void render(cairo_surface_t*, char*, char*, PangoLayout *, cairo_t *, int);
};

} // namespace Quote.
