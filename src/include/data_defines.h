//
//  data_defines.h
//  camera-sdk
//
//  Created by 0day on 15/1/26.
//  Copyright (c) 2015年 Pili Engineering. All rights reserved.
//

#ifndef camera_sdk_data_defines_h
#define camera_sdk_data_defines_h

#include <stdbool.h>
#include <stdlib.h>

#include "librtmp/rtmp.h"

/**
 * @struct pili_metadata_packet
 * metadata 包
 * 
 * @pointer pili_metadata_packet_p
 */
typedef struct pili_metadata_packet {
    uint32_t data_size;
    uint32_t timestamp;
    uint32_t stream_id;
    void *data;
} pili_metadata_packet_t, *pili_metadata_packet_p;

int pili_metadata_packet_create(pili_metadata_packet_p *packet);
int pili_metadata_packet_init(pili_metadata_packet_p packet,
                              uint32_t data_size,
                              uint32_t timestamp,
                              uint32_t stream_id,
                              void *data);
int pili_metadata_packet_release(pili_metadata_packet_p packet);

/**
 * @struct pili_video_packet
 * 待发送的视频帧
 *
 * @pointer pili_video_packet_p
 */
typedef struct pili_video_packet {
    uint8_t *video_tag;
    size_t  tag_size; // nalu_len + 5 + 4
    int64_t pts; // Presentation timestamp (ms)
    int64_t dts; // Decode timestamp (ms)
    bool keyframe; // Is a keyframe
    uint8_t res[3];
} pili_video_packet_t, *pili_video_packet_p;

int pili_video_packet_create(pili_video_packet_p *packet);
int pili_video_packet_init(pili_video_packet_p packet,
                           uint8_t *video_tag,
                           size_t tag_size,
                           int64_t pts,
                           int64_t dts,
                           bool keyframe);
int pili_video_packet_release(pili_video_packet_p packet);

#define AUDIO_TAG_SAMPLERATE_5_5KHZ 0 << 0
// ...

/**
 * @struct pili_audio_packet
 * 待发送的音频帧
 *
 * @pointer pili_audio_packet_p
 */
typedef struct pili_audio_packet {
    uint8_t *audio_tag; // Packet data
    size_t tag_size; // Packet size
    // TS/PS标准时间,单位为90kHz时钟,取其高32位,即45kHz时钟
    int64_t ts; // timestamp
    uint8_t res[3];
} pili_audio_packet_t, *pili_audio_packet_p;

int pili_audio_packet_create(pili_audio_packet_p *packet);
int pili_audio_packet_init(pili_audio_packet_p packet,
                           uint8_t *audio_tag,
                           size_t size,
                           int64_t ts);
int pili_audio_packet_release(pili_audio_packet_p packet);

typedef enum pili_rtmp_packet_type {
    pili_rtmp_packet_type_audio,
    pili_rtmp_packet_type_video,
    pili_rtmp_packet_type_metadata
} pili_rtmp_packet_type;

typedef struct pili_rtmp_packet {
    pili_video_packet_p v_packet;
    pili_audio_packet_p a_packet;
    pili_metadata_packet_p m_packet;
    pili_rtmp_packet_type type;
    RTMPPacket          rtmp_packet;
} pili_rtmp_packet_t, *pili_rtmp_packet_p;

int pili_rtmp_packet_create(pili_rtmp_packet_p *packet);
int pili_rtmp_packet_init(pili_rtmp_packet_p packet,
                          pili_video_packet_p v_packet,
                          pili_audio_packet_p a_packet,
                          pili_metadata_packet_p m_packet,
                          pili_rtmp_packet_type type,
                          RTMP  *rtmp);
int pili_rtmp_packet_release(pili_rtmp_packet_p packet);

typedef struct pili_packet_buffer {
    pili_rtmp_packet_p data;
    struct pili_packet_buffer *prev;
    struct pili_packet_buffer *next;
} pili_packet_buffer_t, *pili_packet_buffer_p;

/**
 * Alloc a pili_packet_buffer_p.
 * @param buffer a pointer wait for memory alloc.
 * @return The alloc result, 0 for success, -1 for memory alloc failure.
 */
int pili_packet_buffer_create(pili_packet_buffer_p *buffer);

/**
 * Init a pili_packet_buffer_p.
 * @param buffer    the buffer to be inited.
 * @param data      inside rtmp packet.
 * @param prev      queue's prev node.
 * @param next      queue's next node
 * @return Init result, 0 for success, -1 for failure. Failure happens when buffer is NULL.
 */
int pili_packet_buffer_init(pili_packet_buffer_p buffer,
                            pili_rtmp_packet_p data,
                            pili_packet_buffer_p prev,
                            pili_packet_buffer_p next);

/**
 * Release a pili_packet_buffer_p.
 * @param buffer to be released.
 * @return Release result, 0 for success.
 */
int pili_packet_buffer_release(pili_packet_buffer_p buffer);


struct pili_stream_context;
typedef struct pili_stream_context *pili_stream_context_p;

typedef void (*pili_packet_queue_release_cb)(pili_stream_context_p ctx, pili_rtmp_packet_p packet);
typedef void (*pili_packet_queue_drop_one_packet_cb)(pili_stream_context_p ctx, pili_rtmp_packet_p packet);

typedef struct pili_packet_queue {
    pili_packet_buffer_p                    head;
    int                                     len;
    int                                     capacity;
    int                                     drop_frame_strategy;
    pili_stream_context_p                   context;
    pili_packet_queue_release_cb            release_cb;
    pili_packet_queue_drop_one_packet_cb    drop_packet_cb;
} pili_packet_queue_t, *pili_packet_queue_p;

/**
 * Alloc a pili_packet_queue.
 * @param queue a pointer wait for memory alloc.
 * @return The alloc result, 0 for success, -1 for memory alloc failure.
 */
int pili_queue_create(pili_packet_queue_p *queue);

/**
 * Init a pili_packt_queue.
 * @param queue                 the queue to be inited, you should always call this function after pili_queue_create() called before you use the queue.
 * @param capacity              the capacity of the queue.
 * @param drop_frame_strategy   this param will influence how a packet be droped when queue is full.
 * @param release_cb            when queue will be released, this callback function will be invoked. It give you a chance to release pili_packet_buffer_p.
 * @param drop_packet_cb        when a packet will be droped, this callback function will be invoked. It give you a chance to release pili_packet_buffer_p.
 * @return              Init result, 0 for success, -1 for failure. Failure happens when queue is NULL.
 */
int pili_queue_init(pili_packet_queue_p queue,
                    int capacity,
                    int drop_frame_strategy,
                    pili_stream_context_p context,
                    pili_packet_queue_release_cb release_cb,
                    pili_packet_queue_drop_one_packet_cb drop_packet_cb);

/**
 * Release a pili_packet_queue.
 * @param queue the queue to be released.
 * @return Release result, 0 for success.
 */
int pili_queue_release(pili_packet_queue_p queue);

/**
 * Check to see if the queue is full.
 * @param queue the queue to be checked.
 * @return Check result, true for full, false for not full.
 */
bool pili_queue_is_full(pili_packet_queue_p queue);

/**
 * Check to see if the queue is empty.
 * @param queue the queue to be checked.
 * return Check result, true for empty, false for not empty.
 */
bool pili_queue_is_empty(pili_packet_queue_p queue);

/**
 * Length of a queue.
 * @param queue the queue you wanna check.
 * @return Length of the queue. -1 will be returned if queue is NULL.
 */
int pili_queue_length(pili_packet_queue_p queue);

/**
 * Get a packet buffer from queue.
 * @param queue         the queue to access.
 * @param index         which one you want to pick.
 * @param packet_buffer the result.
 * @return 0 for success, -1 for failure. Failure happens when queue is NULL or index is out of range.
 */
int pili_queue_get_packet_buffer(pili_packet_queue_p queue, int index, pili_packet_buffer_p *packet_buffer);

/**
 * Get first packet buffer from queue.
 * @param queue     the queue to access.
 * @packet_buffer   the result.
 * @return 0 for success, -1 for failure. Failure happens when queue is NULL or index is out of range.
 */
int pili_queue_get_first_packet_buffer(pili_packet_queue_p queue, pili_packet_buffer_p *packet_buffer);

/**
 * Get last packet buffer from queue.
 * @param queue     the queue to access.
 * @packet_buffer   the result.
 * @return 0 for success, -1 for failure. Failure happens when queue is NULL or index is out of range.
 */
int pili_queue_get_last_packet_buffer(pili_packet_queue_p queue, pili_packet_buffer_p *packet_buffer);

/**
 * Drop a packet from a queue.
 * @param queue the queue to access.
 * @return 0 for success, -1 for failure. Failure happens when queue is NULL.
 */
int pili_queue_drop_one_packet(pili_packet_queue_p queue);

/**
 * Write a packet to a queue.
 * @param queue the queue to access.
 * @return 0 for success, -1 & -2  for failure. -1 happens when queue is NULL or packet is NULL, -2 returned when queue is full.
 */
int pili_queue_write_one_packet(pili_packet_queue_p queue, pili_rtmp_packet_p packet);

/**
 * Drop a packt.
 * @param queue the queue to access.
 * @return 0 for success, -1 & -2 for failure. -1 returned when queue is NULL, -2 returned when queue is empty.
 */
int pili_queue_read_one_packet(pili_packet_queue_p queue, pili_rtmp_packet_p *packet);

/**
 * @struct pili_context
 * 推流上下文，核心结构
 * 
 * tips: 山下文总是通过 pili_stream_context_create 和 pili_stream_context_init 完成二部创建初始化，通过 pili_stream_context_release 销毁
 */

typedef void (*context_audio_sent_callback)(pili_audio_packet_p packet);
typedef void (*context_video_sent_callback)(pili_video_packet_p packet);

typedef struct pili_stream_context {
    RTMP                        *rtmp;
    pili_packet_queue_p         queue;
    context_audio_sent_callback audio_callback;
    context_video_sent_callback video_callback;
} pili_stream_context_t;

int pili_stream_context_create(pili_stream_context_p *ctx);
int pili_stream_context_init(pili_stream_context_p ctx,
                             context_audio_sent_callback audio_cb,
                             context_video_sent_callback video_cb);
int pili_stream_context_release(pili_stream_context_p ctx);


#endif
