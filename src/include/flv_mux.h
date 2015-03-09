/*
 * @brief flv_mux.h
 * @author Akagi201
 * @date 2015/02/1
 */

#ifndef FLV_MUX_H_
#define FLV_MUX_H_ (1)

#include "data_defines.h"

typedef enum flv_type {
    flv_h264,
    flv_aac,
    flv_aac_plus,
    flv_g711
} flv_type_t;

// video
int flv_h264_mux(pili_rtmp_packet_p packet, RTMP *rtmp);

// audio
int flv_aac_mux(pili_rtmp_packet_p packet, RTMP *rtmp);

int flv_aac_plus_mux(pili_rtmp_packet_p packet);
int flv_g711_mux(pili_rtmp_packet_p packet);

int rtmp_metadata_mux(pili_rtmp_packet_p packet, RTMP *rtmp);

//int flv_packet_mux(pili_rtmp_packet_p packet, flv_type_t type);

#endif // FLV_MUX_H_
