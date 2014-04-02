
#include "Template.h"

#include "TemplateBlueX.h"
#include "TemplateBlackX.h"

static const uint32_t BlueX = 1;
static const uint32_t BlackX = 2;

namespace Quote {

Template* Template::createTemplate(uint32_t templateType)
{
  Template* imageTemplate;

  switch (templateType) {
    case BlueX:
      imageTemplate = new Quote::TemplateBlueX();
      break;
    case BlackX:
    default:
      imageTemplate = new Quote::TemplateBlackX();
  }

  return imageTemplate;
}

} // namespace Quote.
