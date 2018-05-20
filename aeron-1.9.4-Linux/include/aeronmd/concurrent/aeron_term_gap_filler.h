/*
 * Copyright 2014-2018 Real Logic Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AERON_AERON_TERM_GAP_FILLER_H
#define AERON_AERON_TERM_GAP_FILLER_H

#include <stdbool.h>
#include <stdint.h>
#include "aeron_logbuffer_descriptor.h"

bool aeron_term_gap_filler_try_fill_gap(
    aeron_logbuffer_metadata_t *log_meta_data,
    uint8_t *buffer,
    int32_t term_id,
    int32_t gap_offset,
    int32_t gap_length);

#endif //AERON_AERON_TERM_GAP_FILLER_H
