//
//  pili_type.h
//  pili-camera-sdk
//
//  Created on 15/3/31.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#ifndef __PILI_CAMERA_SDK__TYPE__
#define __PILI_CAMERA_SDK__TYPE__

#include "pili_macro.h"
#include "flv.h"
#include "librtmp/rtmp.h"

#define PILI_STREAM_DROP_FRAME_POLICY_RANDOM        (0x00)

#define PILI_STREAM_BUFFER_TIME_INTERVAL_DEFAULT    3

PILI_TYPE_POINTER(struct pili_packet_queue, pili_packet_queue_p);

PILI_TYPE(struct pili_stream_context, pili_stream_context_t);
PILI_TYPE_POINTER(struct pili_stream_context, pili_stream_context_p);

PILI_TYPE(struct pili_h264_nalu, pili_h264_nalu_t);
PILI_TYPE_POINTER(struct pili_h264_nalu, pili_h264_nalu_p);

PILI_TYPE(struct pili_h264_key_frame, pili_h264_key_frame_t);
PILI_TYPE_POINTER(struct pili_h264_key_frame, pili_h264_key_frame_p);

#define PILI_STREAM_STATE_UNKNOW        0x00
#define PILI_STREAM_STATE_CONNECTING    0x01
#define PILI_STREAM_STATE_CONNECTED     0x02
#define PILI_STREAM_STATE_DISCONNECTED  0x03
#define PILI_STREAM_STATE_ERROR         0x04

typedef void (*pili_stream_state_cb)(uint8_t state);

/**
 * stream context
 */
pili_stream_context_p pili_create_stream_context();
int pili_init_stream_context(pili_stream_context_p ctx,
                             uint8_t drop_frame_policy,
                             uint32_t buffer_time_interval,
                             pili_stream_state_cb stream_state_cb);
int pili_release_stream_context(pili_stream_context_p ctx);

struct pili_stream_context {
    char                    *push_url;
    RTMP                    *rtmp;
    pili_packet_queue_p     queue;
    uint8_t                 drop_frame_policy;
    uint32_t                buffer_time_interval;
    pili_stream_state_cb    stream_state_cb;
};

/**
 * h264
 */
struct pili_h264_nalu {
    uint8_t *data;
    size_t  length;
};

struct pili_h264_key_frame {
    pili_h264_nalu_t sps;
    pili_h264_nalu_t pps;
    pili_h264_nalu_t sei;
    pili_h264_nalu_t idr;
};

#endif  // __PILI_CAMERA_SDK__TYPE__
