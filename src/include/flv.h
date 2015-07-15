//
//  flv.h
//  pili-camera-sdk
//
//  Created on 15/3/30.
//  Copyright (c) Pili Engineering, Qiniu Inc. All rights reserved.
//

#ifndef __PILI_CAMERA_SDK__FLV__
#define __PILI_CAMERA_SDK__FLV__

#include <stdlib.h>
#include <stdint.h>
#include "pili_macro.h"

/**
 * FLV tag 类型声明
 */
#define FLV_TAG_TYPE_AUDIO  ((uint8_t)0x08)
#define FLV_TAG_TYPE_VIDEO  ((uint8_t)0x09)
#define FLV_TAG_TYPE_SCRIPT ((uint8_t)0x12)

PILI_TYPE(struct flv_tag, flv_tag_t);
PILI_TYPE_POINTER(struct flv_tag, flv_tag_p);

/**
 * FLV tag 类型定义
 */
struct flv_tag {
    uint8_t     tag_type;
    uint32_t    data_size;
    uint32_t    timestamp;
    uint32_t    stream_id;
    void        *data;  // 指向 flv_audio_data_p 或 flv_video_data_p
};

/**
 * AMF 声明
 */
enum {
    kAMFNumber = 0,
    kAMFBoolean,
    kAMFString,
    kAMFObject,
    kAMFMovieClip,		/* reserved, not used */
    kAMFNull,
    kAMFUndefined,
    kAMFReference,
    kAMFEMCAArray,
    kAMFObjectEnd,
    kAMFStrictArray,
    kAMFDate,
    kAMFLongString,
    kAMFUnsupported,
    kAMFRecordSet,		/* reserved, not used */
    kAMFXmlDoc,
    kAMFTypedObject,
    kAMFAvmPlus,		/* switch to AMF3 */
    kAMFInvalid = 0xff
};

// ---------------- Audio tag -------------------
/**
 * Audio Sound Format 定义
 */
#define FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET              4

#define FLV_AUDIO_TAG_SOUND_FORMAT_LINEAR_PCM          0
#define FLV_AUDIO_TAG_SOUND_FORMAT_ADPCM               1 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_MP3                 2 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_LINEAR_PCM_LE       3 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_NELLYMOSER_16_MONO  4 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_NELLYMOSER_8_MONO   5 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_NELLYMOSER          6 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_G711_A              7 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_G711_MU             8 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_RESERVED            9 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_AAC                 10 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_SPEEX               11 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_MP3_8               14 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET
#define FLV_AUDIO_TAG_SOUND_FORMAT_DEVICE_SPECIFIC     15 << FLV_AUDIO_TAG_SOUND_FORMAT_OFFSET

/**
 * Audio Sound Rate 定义
 */
#define FLV_AUDIO_TAG_SOUND_RATE_OFFSET  2

#define FLV_AUDIO_TAG_SOUND_RATE_5_5     0
#define FLV_AUDIO_TAG_SOUND_RATE_11      1 << FLV_AUDIO_TAG_SOUND_RATE_OFFSET
#define FLV_AUDIO_TAG_SOUND_RATE_22      2 << FLV_AUDIO_TAG_SOUND_RATE_OFFSET
#define FLV_AUDIO_TAG_SOUND_RATE_44      3 << FLV_AUDIO_TAG_SOUND_RATE_OFFSET

/**
 * Audio Sound Type 定义
 */
#define FLV_AUDIO_TAG_SOUND_TYPE_MONO    0x00
#define FLV_AUDIO_TAG_SOUND_TYPE_STEREO  0x01

/**
 * Audio Sound Size 定义
 */
#define FLV_AUDIO_TAG_SOUND_SIZE_OFFSET  1

#define FLV_AUDIO_TAG_SOUND_SIZE_8       0
#define FLV_AUDIO_TAG_SOUND_SIZE_16      1 << FLV_AUDIO_TAG_SOUND_SIZE_OFFSET


// ---------------- Video tag -------------------
/**
 * Video Frame Type 定义
 */
#define FLV_VIDEO_TAG_FRAME_TYPE_OFFSET                 4

#define FLV_VIDEO_TAG_FRAME_TYPE_KEYFRAME               1 << FLV_VIDEO_TAG_FRAME_TYPE_OFFSET
#define FLV_VIDEO_TAG_FRAME_TYPE_INTERFRAME             2 << FLV_VIDEO_TAG_FRAME_TYPE_OFFSET
#define FLV_VIDEO_TAG_FRAME_TYPE_DISPOSABLE_INTERFRAME  3 << FLV_VIDEO_TAG_FRAME_TYPE_OFFSET
#define FLV_VIDEO_TAG_FRAME_TYPE_GENERATED_KEYFRAME     4 << FLV_VIDEO_TAG_FRAME_TYPE_OFFSET
#define FLV_VIDEO_TAG_FRAME_TYPE_COMMAND_FRAME          5 << FLV_VIDEO_TAG_FRAME_TYPE_OFFSET

/**
 * Video Codec 定义
 */
#define FLV_VIDEO_TAG_CODEC_JPEG            1
#define FLV_VIDEO_TAG_CODEC_SORENSEN_H263   2
#define FLV_VIDEO_TAG_CODEC_SCREEN_VIDEO    3
#define FLV_VIDEO_TAG_CODEC_ON2_VP6         4
#define FLV_VIDEO_TAG_CODEC_ON2_VP6_ALPHA   5
#define FLV_VIDEO_TAG_CODEC_SCREEN_VIDEO_V2 6
#define FLV_VIDEO_TAG_CODEC_AVC             7

// ---------------- functions -------------------

flv_tag_p flv_create_tag();
int flv_init_tag(flv_tag_p tag,
                 uint8_t   tag_type,
                 uint32_t  data_size,
                 uint32_t  timestamp,
                 uint32_t  stream_id,
                 void      *data);

int flv_release_tag(flv_tag_p tag);

#endif /* defined(__PILI_CAMERA_SDK__FLV__) */
