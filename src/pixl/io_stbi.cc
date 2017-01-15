//
// Copyright (c) 2016. See AUTHORS file.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// Decoding
#define STBI_NO_HDR
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Encoding
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "io.h"
#include "image.h"

namespace pixl {

    // ----------------------------------------------------------------------------
    Image* StbiImageReader::read(const char* path) {
        i32 width, height, channels;
        u8* data = stbi_load(path, &width, &height, &channels, 0);
        if (data == 0) {
            // FATAL("Failed to load Pixmap: " << path)
            // FATAL("Failure Reason: " << stbi_failure_reason())
            return 0;
        }

        return new Image(width, height, channels, data);
    }

    // ----------------------------------------------------------------------------
    void StbiImageWriter::write(const char* path, Image* image) {
        stbi_write_png(path, image->getWidth(), image->getHeight(), image->getChannels(),
                       image->getData(), 0);
    }
}