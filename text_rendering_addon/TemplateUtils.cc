/*
 * Copyright (c) 2014 AG Castro <a.garcia.castro@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TemplateUtils.h"

void
render_frame_gradient(cairo_pattern_t* pattern, double frame_width, double transparency)
{
  cairo_pattern_add_color_stop_rgba(pattern, 0.0, 0.0, 0.0, 0.0, transparency);
  cairo_pattern_add_color_stop_rgba(pattern, frame_width, 0.0, 0.0, 0.0, 0.0);
  cairo_pattern_add_color_stop_rgba(pattern, 1.0-frame_width, 0.0, 0.0, 0.0, 0.0);
  cairo_pattern_add_color_stop_rgba(pattern, 1.0, 0.0, 0.0, 0.0, transparency);
}
