
#include <pango/pangocairo.h>

void
render(cairo_surface_t* surface, char* text, char* author,
       PangoLayout *sizeLayout, cairo_t *sizeCr, int layoutWidth);
