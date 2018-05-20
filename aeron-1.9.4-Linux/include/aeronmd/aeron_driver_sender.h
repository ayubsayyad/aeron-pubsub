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

#ifndef AERON_AERON_DRIVER_SENDER_H
#define AERON_AERON_DRIVER_SENDER_H

#include "aeron_driver_context.h"
#include "aeron_driver_sender_proxy.h"
#include "aeron_system_counters.h"
#include "media/aeron_udp_transport_poller.h"
#include "aeron_network_publication.h"
#include "concurrent/aeron_distinct_error_log.h"

typedef struct aeron_driver_sender_network_publication_entry_stct
{
    aeron_network_publication_t *publication;
}
aeron_driver_sender_network_publication_entry_t;

#define AERON_DRIVER_SENDER_NUM_RECV_BUFFERS (2)

typedef struct aeron_driver_sender_stct
{
    aeron_driver_sender_proxy_t sender_proxy;
    aeron_udp_transport_poller_t poller;

    struct aeron_driver_sender_network_publications_stct
    {
        aeron_driver_sender_network_publication_entry_t *array;
        size_t length;
        size_t capacity;
    }
    network_publicaitons;

    struct aeron_driver_sender_buffers_stct
    {
        uint8_t *buffers[AERON_DRIVER_SENDER_NUM_RECV_BUFFERS];
        struct iovec iov[AERON_DRIVER_SENDER_NUM_RECV_BUFFERS];
        struct sockaddr_storage addrs[AERON_DRIVER_SENDER_NUM_RECV_BUFFERS];
    }
    recv_buffers;

    aeron_driver_context_t *context;
    aeron_distinct_error_log_t *error_log;
    int64_t status_message_read_timeout_ns;
    int64_t control_poll_timeout_ns;
    size_t round_robin_index;
    size_t duty_cycle_counter;
    size_t duty_cycle_ratio;

    int64_t *total_bytes_sent_counter;
    int64_t *errors_counter;
    int64_t *invalid_frames_counter;
    int64_t *status_messages_received_counter;
    int64_t *nak_messages_received_counter;
}
aeron_driver_sender_t;

#define AERON_DRIVER_SENDER_ERROR(sender, format, ...) \
do \
{ \
    char error_buffer[AERON_MAX_PATH]; \
    int err_code = aeron_errcode(); \
    snprintf(error_buffer, sizeof(error_buffer) - 1, format, __VA_ARGS__); \
    aeron_distinct_error_log_record(sender->error_log, err_code, aeron_errmsg(), error_buffer); \
    aeron_counter_increment(sender->errors_counter, 1); \
    aeron_set_err(0, "%s", "no error"); \
} \
while(0)

int aeron_driver_sender_init(
    aeron_driver_sender_t *sender,
    aeron_driver_context_t *context,
    aeron_system_counters_t *system_counters,
    aeron_distinct_error_log_t *error_log);

int aeron_driver_sender_do_work(void *clientd);
void aeron_driver_sender_on_close(void *clientd);

void aeron_driver_sender_on_add_endpoint(void *clientd, void *command);
void aeron_driver_sender_on_remove_endpoint(void *clientd, void *command);
void aeron_driver_sender_on_add_publication(void *clientd, void *command);
void aeron_driver_sender_on_remove_publication(void *clientd, void *command);
void aeron_driver_sender_on_add_destination(void *clientd, void *command);
void aeron_driver_sender_on_remove_destination(void *clientd, void *command);

int aeron_driver_sender_do_send(aeron_driver_sender_t *sender, int64_t now_ns);

#endif //AERON_AERON_DRIVER_SENDER_H
