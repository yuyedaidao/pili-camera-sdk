//
//  push.h
//  camera-sdk
//
//  Created on 15/1/26.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#ifndef camera_sdk_push_h
#define camera_sdk_push_h

#include "pili_type.h"

int pili_stream_push_open(pili_stream_context_p context, const char *push_url);
int pili_stream_push_close(pili_stream_context_p context);

int pili_write_packet(pili_stream_context_p context, flv_tag_p flv_tag);

int pili_write_h264_key_frame(pili_stream_context_p context, pili_h264_key_frame_t key_frame, uint32_t ts);
int pili_write_h264_slice(pili_stream_context_p context, pili_h264_nalu_t slice_nalu, uint32_t ts);

#endif
