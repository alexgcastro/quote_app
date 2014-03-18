
#include "TemplateUtils.h"

void
render_frame_gradient(cairo_pattern_t* pattern, double frame_width)
{
  cairo_pattern_add_color_stop_rgba(pattern, 0.0, 0.0, 0.0, 0.0, 0.1);
  cairo_pattern_add_color_stop_rgba(pattern, frame_width, 0.0, 0.0, 0.0, 0.0);
  cairo_pattern_add_color_stop_rgba(pattern, 1.0-frame_width, 0.0, 0.0, 0.0, 0.0);
  cairo_pattern_add_color_stop_rgba(pattern, 1.0, 0.0, 0.0, 0.0, 0.1);
}

