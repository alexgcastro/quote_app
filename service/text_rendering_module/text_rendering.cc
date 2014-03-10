#include <node.h>
#include <v8.h>

#include <stdlib.h>
#include <pango/pangocairo.h>

using namespace v8;

static void
render_blue_template(cairo_surface_t* surface, char* text)
{
  cairo_t *cr;

  PangoLayout *layout;
  PangoFontDescription *desc;

  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 0.5, 0.625, 0.773);
  cairo_paint(cr);

  layout = pango_cairo_create_layout(cr);

  pango_layout_set_text(layout, text, -1);
  desc = pango_font_description_from_string("Sans Bold 20");
  pango_layout_set_font_description(layout, desc);
  pango_font_description_free(desc);
  pango_layout_set_width(layout, pango_units_from_double(600));
  pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

  cairo_move_to(cr, 25, 50);

  pango_cairo_show_layout(cr, layout);

  g_object_unref(layout);

  cairo_destroy(cr);
}

static void
generate_image(char* text, char* filename)
{
  cairo_status_t status;
  cairo_surface_t* surface;

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                       650, 750);
  /* Choose the right template to render.*/
  render_blue_template(surface, text);

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
