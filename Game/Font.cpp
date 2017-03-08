#include "Font.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <cpplog/cpplog.hpp>

static cpplog::StdErrLogger logger;

Font::Font() {
    FT_Library library;
    auto error = FT_Init_FreeType(&library);
    if (error) {
        
    }
}


Font::~Font() {
}
