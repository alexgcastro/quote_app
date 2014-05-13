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
