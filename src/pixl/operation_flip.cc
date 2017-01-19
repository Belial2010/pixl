//
// Copyright (c) 2017. See AUTHORS file.
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

#include "iostream"

#include "operation.h"
#include "image.h"
#include "types.h"
#include "utils.h"

namespace pixl {

    // ----------------------------------------------------------------------------
    void flip_vertically(Image* img) {
        const auto lineSizeInBytes = img->width * img->channels;
        for (i32 column = 0; column < img->width; column++) {
            u8* start = img->data + (column * img->channels);
            u8* end = start + (img->height * lineSizeInBytes);

            while (start <= end) {
                aswap(start, end, img->channels);
                start += lineSizeInBytes;
                end -= lineSizeInBytes;
            }
        }
    }

    // ----------------------------------------------------------------------------
    void flip_horizontally(Image* img) {
        const auto lineSizeInBytes = img->width * img->channels;
        for (i32 line = 0; line < img->height; line++) {
            u8* start = img->data + (line * lineSizeInBytes);
            u8* end = start + lineSizeInBytes - img->channels;

            while (start <= end) {
                aswap(start, end, img->channels);
                start += img->channels;
                end -= img->channels;
            }
        }
    }

    // ----------------------------------------------------------------------------
    void FlipTransformation::apply(Image* image) {
        if (this->orientation == Orientation::HORIZONTAL) {
            flip_horizontally(image);
        } else if (this->orientation == Orientation::VERTICAL) {
            flip_vertically(image);
        }
    }
}