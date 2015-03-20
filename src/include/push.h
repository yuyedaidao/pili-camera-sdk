//
//  push.h
//  camera-sdk
//
//  Created by 0day on 15/1/26.
//  Copyright (c) 2015年 qgenius. All rights reserved.
//

#ifndef camera_sdk_push_h
#define camera_sdk_push_h

#include "data_defines.h"
#include "flv_mux.h"

int pili_stream_push_open(pili_stream_context_p context, pili_metadata_packet_p metadata_packet, const char *push_url);
int pili_stream_push_close(pili_stream_context_p context);

int pili_write_video_packet(pili_stream_context_p context, pili_video_packet_p packet);
int pili_write_audio_packet(pili_stream_context_p context, pili_audio_packet_p packet);

#endif
