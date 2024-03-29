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

#include "TemplateBlueX.h"

#include <stdlib.h>
#include <string.h>

#include "TemplateUtils.h"

#define FONT "Impact "
#define TEXT_FONT FONT"24"
#define AUTHOR_FONT FONT"14"
#define FRAME_SIDE_PIXELS 20.0
#define FRAME_DARKNESS 0.1
#define SHADOW_OFFSET 1

namespace Quote {

TemplateBlueX::TemplateBlueX()
{
}

TemplateBlueX::~TemplateBlueX()
{
}

void
TemplateBlueX::render(cairo_surface_t* surface, char* text, char* author,
                      PangoLayout *sizeLayout, cairo_t *sizeCr, int layoutWidth)
{
  cairo_t *cr;
  PangoLayout *layout;
  PangoFontDescription *desc;
  PangoAttribute* attribute;
  PangoAttrList *attribs_list;
  int text_byte_length;
  char* quote;
  int width = 0;
  int height = 0;

  if (!sizeLayout) {
    cr = cairo_create(surface);
    layout = pango_cairo_create_layout(cr);
  }
  else {
    cr = sizeCr;
    layout = sizeLayout;
  }

  /* Add a brackground frame. */
  height = cairo_image_surface_get_height(surface);
  if (height > 0) {
    cairo_pattern_t *pattern;
    double frame_width;

    cairo_set_source_rgb(cr, 0.5, 0.625, 0.773);
    cairo_paint(cr);

    pattern = cairo_pattern_create_linear(0, 0, 0, height);
    frame_width = FRAME_SIDE_PIXELS/height;
    render_frame_gradient(pattern, frame_width, FRAME_DARKNESS);
    cairo_set_source(cr, pattern);
    cairo_paint(cr);
    cairo_pattern_destroy(pattern);

    width = cairo_image_surface_get_width(surface);
    pattern = cairo_pattern_create_linear(0, 0, width, 0);
    frame_width = FRAME_SIDE_PIXELS/width;
    render_frame_gradient(pattern, frame_width, FRAME_DARKNESS);
    cairo_set_source(cr, pattern);
    cairo_paint(cr);
    cairo_pattern_destroy(pattern);
  }

  /* Render text. */
  quote = g_strconcat(text, "\n\n", author, NULL);
  text_byte_length = strlen(text);

  pango_layout_set_text(layout, quote, -1);

  attribs_list = pango_attr_list_new();

  desc = pango_font_description_from_string(TEXT_FONT);
  attribute = pango_attr_font_desc_new(desc);
  attribute->start_index = 0;
  attribute->end_index = text_byte_length;
  pango_attr_list_insert(attribs_list, attribute);

  desc = pango_font_description_from_string(AUTHOR_FONT);
  attribute = pango_attr_font_desc_new(desc);
  attribute->start_index = text_byte_length;
  pango_attr_list_insert(attribs_list, attribute);

  pango_layout_set_attributes(layout, attribs_list);
  pango_attr_list_unref(attribs_list);

  pango_layout_set_width(layout, pango_units_from_double(layoutWidth));
  pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

  if (width > 0)
    cairo_move_to(cr, (width-layoutWidth)/2+SHADOW_OFFSET, 50+SHADOW_OFFSET);

  pango_cairo_show_layout(cr, layout);

  cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
  pango_cairo_update_layout(cr, layout);

  if (width > 0)
    cairo_move_to(cr, (width-layoutWidth)/2, 50);

  pango_cairo_show_layout(cr, layout);

  if (!sizeLayout) {
    g_object_unref(layout);
    cairo_destroy(cr);
  }

  free(quote);
}

} // namespace Quote.
