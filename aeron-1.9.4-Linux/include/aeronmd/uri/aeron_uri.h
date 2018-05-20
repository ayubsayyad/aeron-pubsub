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

#ifndef AERON_AERON_URI_H
#define AERON_AERON_URI_H

#include <errno.h>
#include "aeron_driver_common.h"

typedef struct aeron_uri_param_stct
{
    const char *key;
    const char *value;
}
aeron_uri_param_t;

typedef struct aeron_uri_params_stct
{
    aeron_uri_param_t *array;
    size_t length;
}
aeron_uri_params_t;

#define AERON_UDP_CHANNEL_ENDPOINT_KEY "endpoint"
#define AERON_UDP_CHANNEL_INTERFACE_KEY "interface"
#define AERON_UDP_CHANNEL_TTL_KEY "ttl"
#define AERON_UDP_CHANNEL_CONTROL_KEY "control"

#define AERON_UDP_CHANNEL_CONTROL_MODE_KEY "control-mode"
#define AERON_UDP_CHANNEL_CONTROL_MODE_MANUAL_VALUE "manual"
#define AERON_UDP_CHANNEL_CONTROL_MODE_DYNAMIC_VALUE "dynamic"

#define AERON_URI_INITIAL_TERM_ID_KEY "init-term-id"
#define AERON_URI_TERM_ID_KEY "term-id"
#define AERON_URI_TERM_OFFSET_KEY "term-offset"
#define AERON_URI_TERM_LENGTH_KEY "term-length"
#define AERON_URI_MTU_LENGTH_KEY "mtu"

#define AERON_UDP_CHANNEL_RELIABLE_STREAM_KEY "reliable"

typedef struct aeron_uri_publication_params_stct
{
    size_t term_length;
    size_t mtu_length;
    int64_t initial_term_id;
    int64_t term_id;
    uint64_t term_offset;
    bool is_replay;
}
aeron_uri_publication_params_t;

typedef struct aeron_udp_channel_subscription_params_stct
{
    bool reliable;
}
aeron_udp_channel_subscription_params_t;

typedef struct aeron_udp_channel_params_stct
{
    const char *endpoint_key;
    const char *interface_key;
    const char *ttl_key;
    const char *control_key;
    aeron_uri_params_t additional_params;
}
aeron_udp_channel_params_t;

typedef struct aeron_ipc_channel_params_stct
{
    aeron_uri_params_t additional_params;
}
aeron_ipc_channel_params_t;

typedef enum aeron_uri_type_enum
{
    AERON_URI_UDP, AERON_URI_IPC, AERON_URI_UNKNOWN
}
aeron_uri_type_t;

typedef struct aeron_uri_stct
{
    char mutable_uri[AERON_MAX_PATH];
    aeron_uri_type_t type;

    union
    {
        aeron_udp_channel_params_t udp;
        aeron_ipc_channel_params_t ipc;
    }
    params;
}
aeron_uri_t;

typedef int (*aeron_uri_parse_callback_t)(void *clientd, const char *key, const char *value);

int aeron_uri_parse_params(char *uri, aeron_uri_parse_callback_t param_func, void *clientd);

int aeron_udp_uri_parse(char *uri, aeron_udp_channel_params_t *params);
int aeron_ipc_uri_parse(char *uri, aeron_ipc_channel_params_t *params);

int aeron_uri_parse(const char *uri, aeron_uri_t *params);

void aeron_uri_close(aeron_uri_t *params);

uint8_t aeron_uri_multicast_ttl(aeron_uri_t *uri);

const char *aeron_uri_find_param_value(aeron_uri_params_t *uri_params, const char *key);

typedef struct aeron_driver_context_stct aeron_driver_context_t;

int aeron_uri_publication_params(
    aeron_uri_t *uri,
    aeron_uri_publication_params_t *params,
    aeron_driver_context_t *context,
    bool is_exclusive);
int aeron_udp_channel_subscription_params(
    aeron_uri_t *uri,
    aeron_udp_channel_subscription_params_t *params,
    aeron_driver_context_t *context);

#endif //AERON_AERON_URI_H
