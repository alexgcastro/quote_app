#include <node.h>
#include <v8.h>

#include <stdlib.h>
#include <pango/pangocairo.h>

#define DEFAULT_LAYOUT_WIDTH 630

using namespace v8;

static void
render_blue_template(cairo_surface_t* surface, char* text, PangoLayout *sizeLayout,
                     cairo_t *sizeCr, int layoutWidth)
{
  cairo_t *cr;
  PangoLayout *layout;
  PangoFontDescription *desc;

  if (!sizeLayout) {
    cr = cairo_create(surface);
    layout = pango_cairo_create_layout(cr);
  }
  else {
    cr = sizeCr;
    layout = sizeLayout;
  }

  cairo_set_source_rgb(cr, 0.5, 0.625, 0.773);
  cairo_paint(cr);

  pango_layout_set_text(layout, text, -1);
  desc = pango_font_description_from_string("Impact 28");
  pango_layout_set_font_description(layout, desc);
  pango_font_description_free(desc);
  pango_layout_set_width(layout, pango_units_from_double(layoutWidth));
  pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

  cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);

  cairo_move_to(cr, 10, 50);

  pango_cairo_show_layout(cr, layout);

  if (!sizeLayout) {
    g_object_unref(layout);
    cairo_destroy(cr);
  }
}

static void calculate_image_size(char *text, int* width, int* height, int layoutWidth)
{
  cairo_t *cr;
  cairo_surface_t* surface;
  PangoLayout *layout;

  /* Load with 0x0 surface to calculate extends. */
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                       0, 0);

  cr = cairo_create(surface);

  layout = pango_cairo_create_layout(cr);

  /* Choose the right template to render. */
  render_blue_template(surface, text, layout, cr, layoutWidth);

  pango_layout_get_pixel_size(layout, width, height);

  if (width)
    *width += 40;
  if (height)
    *height += 100;

  g_object_unref(layout);
  cairo_surface_destroy(surface);
  cairo_destroy(cr);
}

static void
generate_image(char* text, char* filename)
{
  cairo_status_t status;
  cairo_surface_t* surface;
  int width, height, layoutWidth;

  layoutWidth = DEFAULT_LAYOUT_WIDTH;
  calculate_image_size(text, &width, &height, layoutWidth);

  while (height > 2*width) {
    width = 0;
    height = 0;
    layoutWidth += 20;
    calculate_image_size(text, &width, &height, layoutWidth);
  }

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                       width, height);

  /* Choose the right template to render. */
  render_blue_template(surface, text, NULL, NULL, layoutWidth);

  status = cairo_surface_write_to_png(surface, filename);
  cairo_surface_destroy(surface);

  if (status != CAIRO_STATUS_SUCCESS) {
      g_printerr("Could not save png to '%s'\n", filename);
  }
}

Handle<Value> Render(const Arguments& args) {
  HandleScope scope;
  Local<String> stringFilename;
  Local<String> stringText;
  char *filename;
  char *text;

  if (args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    return scope.Close(Undefined());
  }

  stringFilename = Local<String>::Cast(args[0]);
  stringText = Local<String>::Cast(args[1]);

  text = (char *)malloc(stringText->Utf8Length()+1);
  stringText->WriteUtf8(text);

  filename = (char *)malloc(stringFilename->Utf8Length()+1);
  stringFilename->WriteUtf8(filename);

  generate_image(text, filename);

  free(filename);
  free(text);

  return scope.Close(String::Empty());
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("render"),
      FunctionTemplate::New(Render)->GetFunction());
}

NODE_MODULE(text_rendering, init)
