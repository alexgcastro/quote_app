#include <node.h>
#include <v8.h>

#include <stdlib.h>
#include <string.h>
#include <pango/pangocairo.h>

#define DEFAULT_LAYOUT_WIDTH 630

using namespace v8;

static void
render_blue_template(cairo_surface_t* surface, char* text, char* author,
                     PangoLayout *sizeLayout, cairo_t *sizeCr, int layoutWidth)
{
  cairo_t *cr;
  PangoLayout *layout;
  PangoFontDescription *desc;
  PangoAttribute* attribute;
  PangoAttrList *attribs_list;
  int text_byte_length;
  char * quote;

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

  quote = g_strconcat(text, "\n\n", author, NULL);
  text_byte_length = strlen(text);

  pango_layout_set_text(layout, quote, -1);

  attribs_list = pango_attr_list_new ();

  desc = pango_font_description_from_string("Impact 28");
  attribute = pango_attr_font_desc_new(desc);
  attribute->start_index = 0;
  attribute->end_index = text_byte_length;
  pango_attr_list_insert(attribs_list, attribute);

  desc = pango_font_description_from_string("Impact 18");
  attribute = pango_attr_font_desc_new(desc);
  attribute->start_index = text_byte_length;
  pango_attr_list_insert(attribs_list, attribute);

  pango_layout_set_attributes (layout, attribs_list);
  pango_attr_list_unref (attribs_list);

  pango_layout_set_width(layout, pango_units_from_double(layoutWidth));
  pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

  cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);

  cairo_move_to(cr, 10, 50);

  pango_cairo_show_layout(cr, layout);

  if (!sizeLayout) {
    g_object_unref(layout);
    cairo_destroy(cr);
  }

  free(quote);
}

static void calculate_image_size(char *text, char* author, int* width, int* height,
                                 int layoutWidth)
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
  render_blue_template(surface, text, author, layout, cr, layoutWidth);

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
generate_image(char* text, char* author, char* filename)
{
  cairo_status_t status;
  cairo_surface_t* surface;
  int width, height, layoutWidth;

  layoutWidth = DEFAULT_LAYOUT_WIDTH;
  calculate_image_size(text, author,  &width, &height, layoutWidth);

  while (height > 2*width) {
    width = 0;
    height = 0;
    layoutWidth += 20;
    calculate_image_size(text, author, &width, &height, layoutWidth);
  }

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                       width, height);

  /* Choose the right template to render. */
  render_blue_template(surface, text, author, NULL, NULL, layoutWidth);

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
  Local<String> stringAuthor;
  char *filename;
  char *text;
  char *author;

  if (args.Length() < 3) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    return scope.Close(Undefined());
  }

  stringFilename = Local<String>::Cast(args[0]);
  stringText = Local<String>::Cast(args[1]);
  stringAuthor = Local<String>::Cast(args[2]);

  text = (char *)malloc(stringText->Utf8Length()+1);
  stringText->WriteUtf8(text);

  author = (char *)malloc(stringAuthor->Utf8Length()+1);
  stringAuthor->WriteUtf8(author);

  filename = (char *)malloc(stringFilename->Utf8Length()+1);
  stringFilename->WriteUtf8(filename);

  generate_image(text, author, filename);

  free(filename);
  free(text);
  free(author);

  return scope.Close(String::Empty());
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("render"),
      FunctionTemplate::New(Render)->GetFunction());
}

NODE_MODULE(text_rendering, init)
