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

#ifndef AERON_AERON_ALLOC_H
#define AERON_AERON_ALLOC_H

#include <stddef.h>

int aeron_alloc_no_err(void **ptr, size_t size);
int aeron_alloc(void **ptr, size_t size);
int aeron_alloc_aligned(void **ptr, size_t *offset, size_t size, size_t alignment);
int aeron_reallocf(void **ptr, size_t size);
void aeron_free(void *ptr);

#endif //AERON_AERON_ALLOC_H
