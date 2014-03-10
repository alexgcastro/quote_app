#include <node.h>
#include <v8.h>

#include <stdlib.h>
#include <pango/pangocairo.h>

using namespace v8;

static void
draw_text (cairo_t *cr, Local<String> stringText)
{
#define FONT "Sans Bold 18"

  PangoLayout *layout;
  PangoFontDescription *desc;
  char *text;

  cairo_translate (cr, 225, 10);

  layout = pango_cairo_create_layout (cr);

  text = (char *)malloc(stringText->Utf8Length()+1);
  stringText->WriteUtf8(text);

  pango_layout_set_text (layout, text, -1);
  free(text);
  desc = pango_font_description_from_string (FONT);
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
  pango_layout_set_alignment (layout, PANGO_ALIGN_CENTER);
  pango_layout_set_width (layout, 700);

  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

  pango_cairo_show_layout (cr, layout);

  g_object_unref (layout);
}

Handle<Value> Render(const Arguments& args) {
  HandleScope scope;
  cairo_t *cr;
  Local<String> stringFilename = Local<String>::Cast(args[0]);
  char *filename;
  cairo_status_t status;
  cairo_surface_t *surface;

  if (args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    return scope.Close(Undefined());
  }

  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                        450, 600);

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 0.5, 0.625, 0.773);
  cairo_paint (cr);
  draw_text (cr, Local<String>::Cast(args[1]));
  cairo_destroy (cr);

  filename = (char *)malloc(stringFilename->Utf8Length()+1);
  stringFilename->WriteUtf8(filename);
  status = cairo_surface_write_to_png (surface, filename);
  cairo_surface_destroy (surface);

  if (status != CAIRO_STATUS_SUCCESS) {
      g_printerr ("Could not save png to '%s'\n", filename);
  }

  free(filename);
  return scope.Close(args[1]);
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("render"),
      FunctionTemplate::New(Render)->GetFunction());
}

NODE_MODULE(text_rendering, init)
