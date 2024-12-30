/** Tool to write out addresses on cards. */

#include <cairo/cairo-pdf.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>

#include <iostream>

#include "rapidcsv.h"

constexpr int WIDTH_INCHES = 6;
constexpr double HEIGHT_INCHES = 4.5;

void WriteReturnAddress(const std::string &return_address, cairo_t *cr) {
  PangoLayout *layout = pango_cairo_create_layout(cr);

  PangoFontDescription *desc =
      pango_font_description_from_string("Times New Roman");
  pango_font_description_set_absolute_size(desc, PANGO_SCALE * 10);
  pango_layout_set_font_description(layout, desc);

  pango_layout_set_text(layout, return_address.c_str(), -1);
  pango_cairo_update_layout(cr, layout);

  cairo_move_to(cr, 10, 10);

  pango_cairo_show_layout(cr, layout);

  pango_font_description_free(desc);
}

void WriteAddress(const std::string &address, cairo_t *cr) {
  PangoLayout *layout = pango_cairo_create_layout(cr);

  PangoFontDescription *desc =
      pango_font_description_from_string("Times New Roman");
  pango_font_description_set_absolute_size(desc, PANGO_SCALE * 12);
  pango_layout_set_font_description(layout, desc);

  pango_layout_set_text(layout, address.c_str(), -1);
  pango_cairo_update_layout(cr, layout);

  cairo_move_to(cr, 2 * 72, 2 * 72);

  pango_cairo_show_layout(cr, layout);

  pango_font_description_free(desc);
}

int main(int argc, char **argv) {
  cairo_surface_t *surface = cairo_pdf_surface_create_for_stream(
      [](void *unused_closure, const unsigned char *data, unsigned int length) {
        std::cout.write(reinterpret_cast<const char *>(data), length);
        return CAIRO_STATUS_SUCCESS;
      },
      nullptr, WIDTH_INCHES * 72, HEIGHT_INCHES * 72);

  std::string input(std::istreambuf_iterator<char>(std::cin), {});
  std::stringstream string_stream(input);
  rapidcsv::Document csv_document(string_stream, rapidcsv::LabelParams(-1, -1));

  for (size_t i = 0; i < csv_document.GetRowCount(); i++) {
    const auto &row = csv_document.GetRow<std::string>(i);
    const auto &return_address = row[0];
    const auto &to_address = row[1];

    cairo_t *cr = cairo_create(surface);

    WriteReturnAddress(return_address, cr);
    WriteAddress(to_address, cr);
    cairo_show_page(cr);  // finishes the page

    cairo_destroy(cr);
  }

  cairo_surface_destroy(surface);
}
