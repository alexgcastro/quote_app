
#include <node.h>
#include <pango/pangocairo.h>
#include <stdlib.h>
#include <v8.h>

#include "TemplateBlueX.h"

#define DEFAULT_LAYOUT_WIDTH 630

using namespace v8;

typedef struct AsyncData {
  char *filename;
  char *text;
  char *author;
  Quote::Template* imageTemplate;
  Persistent<Function> callback;
} AsyncData;

static void
calculate_image_size(char *text, char* author, int* width, int* height,
                     int layoutWidth, Quote::Template* imageTemplate)
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
  imageTemplate->render(surface, text, author, layout, cr, layoutWidth);

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
generate_image(uv_work_t *work_request)
{
  cairo_status_t status;
  cairo_surface_t* surface;
  int width, height, layoutWidth;
  AsyncData *asyncData = (AsyncData *)work_request->data;
  char *text = asyncData->text;
  char *author= asyncData->author;
  char *filename = asyncData->filename;
  Quote::Template* imageTemplate = asyncData->imageTemplate;

  layoutWidth = DEFAULT_LAYOUT_WIDTH;
  calculate_image_size(text, author,  &width, &height, layoutWidth, imageTemplate);

  if (width < layoutWidth)
    width = layoutWidth;

  while (height > 2*width) {
    width = 0;
    height = 0;
    layoutWidth += 20;
    calculate_image_size(text, author, &width, &height, layoutWidth, imageTemplate);
  }

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                       width, height);

  /* Choose the right template to render. */
  imageTemplate->render(surface, text, author, NULL, NULL, layoutWidth);

  status = cairo_surface_write_to_png(surface, filename);
  cairo_surface_destroy(surface);

  if (status != CAIRO_STATUS_SUCCESS) {
      g_printerr("Could not save png to '%s'\n", filename);
  }
}

static void
generate_image_after(uv_work_t *work_request, int status) {
  AsyncData *asyncData = (AsyncData *)work_request->data;

  asyncData->callback->Call(Context::GetCurrent()->Global(), 0, NULL);

  free(asyncData->filename);
  free(asyncData->text);
  free(asyncData->author);
  asyncData->callback.Dispose();
  delete asyncData->imageTemplate;

  delete asyncData;
  delete work_request;
}

Handle<Value> Render(const Arguments& args) {
  HandleScope scope;
  Local<String> stringFilename;
  Local<String> stringText;
  Local<String> stringAuthor;
  AsyncData *asyncData;
  char *filename;
  char *text;
  char *author;
  uv_work_t *workRequest;
  Quote::Template* imageTemplate;

  if (args.Length() < 4) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsString() || !args[1]->IsString() ||
      !args[2]->IsString() || !args[3]->IsFunction()) {
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

  workRequest = new uv_work_t;
  imageTemplate = new Quote::TemplateBlueX();
  asyncData = new AsyncData();
  workRequest->data = asyncData;

  asyncData->filename = filename;
  asyncData->text = text;
  asyncData->author = author;
  asyncData->imageTemplate = imageTemplate;
  asyncData->callback = Persistent<Function>::New(Local<Function>::Cast(args[3]));

  uv_queue_work(
    uv_default_loop(),
    workRequest,
    generate_image,
    generate_image_after
  );

  return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("render"),
      FunctionTemplate::New(Render)->GetFunction());
}

NODE_MODULE(text_rendering, init)
