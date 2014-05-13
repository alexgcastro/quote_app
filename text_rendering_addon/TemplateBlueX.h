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

#ifndef TemplateBlueX_h
#define TemplateBlueX_h

#include "Template.h"

namespace Quote {

class TemplateBlueX : public Template {
public:
  TemplateBlueX();
  virtual ~TemplateBlueX();
  virtual void render(cairo_surface_t*, char*, char*, PangoLayout *, cairo_t *, int);
};

} // namespace Quote.

#endif // TemplateBlueX_h
