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

#ifndef TemplateBlackX_h
#define TemplateBlackX_h

#include "Template.h"

namespace Quote {

class TemplateBlackX : public Template {
public:
  TemplateBlackX();
  virtual ~TemplateBlackX();
  virtual void render(cairo_surface_t*, char*, char*, PangoLayout *, cairo_t *, int);
};

} // namespace Quote.

#endif // TemplateBlackX_h
