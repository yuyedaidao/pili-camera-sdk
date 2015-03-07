# pili-camera-sdk

pili-camera-sdk 是 **pili 流媒体云服务** 开发团队为 IPCamera 量身定制的推流 SDK。这套 SDK 旨在为摄像头开发团队提供更开放、更通用的推流方案，缩短开发周期，更便捷、快速、轻松的接入 **pili 流媒体云服务**。


## 快速开始

### 下载配置 SDK

pili-camera-sdk 提供了两种配置方式供开发者选择：

- clib 安装 [推荐]
- 手动安装

#### clib 安装

```
# no more than 1 step
clib install pili-camera-sdk
```

你只需要这一句就搞定了，可以开始使用 sdk 了。

#### 手动安装

```
# step 1
git clone https://github.com/pili-io/pili-camera-sdk.git

# step 2
添加头文件 search path

# step 3
添加 lib search path
```

推荐使用 clib 方式管理 pili-camera-sdk，减少不必要的依赖管理不仅轻松而且更 cool。

### 示例代码

文档最后会附带 demo 的源码地址，这里只做整体流程和主要代码块的解释。

```
# step 1
# 你需要先完成 RTMP 的一些封包，首先完成 MetaData 的封包
pili_metadata_packet_p metadata_packet = NULL;
pili_metadata_packet_create(&metadata_packet);
pili_metadata_packet_init(metadata_packet, data_size, timestamp, stream_id, data);

# step 2
# 初始化 stream context
# pili_stream_context_init 方法中的 audio_sent_callback 和 video_sent_callback 是在发送完后调用的，交给你释放视频包和音频包的时机
pili_stream_context_p ctx = NULL;
pili_stream_context_create(&ctx);
pili_stream_context_init(ctx, audio_sent_callback, video_sent_callback);

# step 3
# 开启推流
char *url = "rtmp://YOUR_PUSH_URL";
pili_stream_push_open(ctx, metadata_packet, url);

# step 4
# 封装音视频包并发送
# 当拿到视频信息时，封包并发送
pili_video_packet_p video_packet = NULL;
pili_video_packet_create(&video_packet);
pili_video_packet_init(video_packet, video_tag, tag_size, pts, dts, keyframe);

pili_write_video_packet(ctx, video_packet);

# 当拿到音频信息时，封包并发送
pili_audio_packet_p audio_packet = NULL;
pili_audio_packet_create(&audio_packet);
pili_audio_packet_init(audio_packet, audio_tag, size, ts);

pili_write_audio_packet(ctx, audio_packet);

# step 5
# 在回调时释放最初创建的对象
void video_sent_callback(pili_video_packet_p v_packet) {
	// 你可以在这里对 v_packet->video_tag 做释放
	// ...
	// 释放 pili_video_packet_p
	pili_video_packet_release(v_packet);
}
void audio_sent_callback(pili_audio_packet_p a_packet) {
	// 你可以在这里对 a_packet->audio_tag 做释放
	// ...
	// 释放 pili_video_packet_p
	pili_audio_packet_release(a_packet);
}

# step 6
# 结束推流并释放上下文
pili_stream_push_close(ctx);
pili_stream_context_release(ctx);
```

**提示：** `pili_write_video_packet` 和 `pili_write_audio_packet` 两个方法是线程安全的。

## 封包详解

sdk对外结构中结构体`pili_video_packet`中的`video_tag`, `pili_audio_tag`中的`audio_tag`都是与一个flv标准封装中的一个`flv tag`的`tag data`部分是一致的.

### MetaData 包

#### 第一个AMF包

```
| 02 | 00 | 0D | @ | s | e | t | D | a | t | a | F | r | a | m | e |
```

#### 第二个AMF包

```
| 02 | 00 | 0A | o | n | M | e | t | a | D | a | t | a |
```

#### 第三个AMF包

```
| 08 | 00 | 00 | 00 | 0C |
```

Metadata元素个数暂定为12个 = 音频5个 + 视频5个 + 2个(duration和filesize). metadata元素的顺序不固定, 此处采用ffmpeg中的顺序.

duration(19 bytes)

```
| 00 | 08 | d | u | r | a | t | i | o | n | 00 |  |  |  |  |  |  |  |  |
```

width(16 bytes)

```
| 00 | 05 | w | i | d | t | h | 00 |  |  |  |  |  |  |  |  |
```

height(17 bytes)

```
| 00 | 06 | h | e | i | g | h | t | 00 |  |  |  |  |  |  |  |  |
```

videodatarate(24 bytes)

```
| 00 | 0D | v | i | d | e | o | d | a | t | a | r | a | t | e | 00 |  |  |  |  |  |  |  |  |
```

framerate(20 bytes)

```
| 00 | 09 | f | r | a | m | e | r | a | t | e | 00 |  |  |  |  |  |  |  |  |
```

videocodecid(23 bytes)

```
| 00 | 0C | v | i | d | e | o | c | o | d | e | c | i | d | 00 |  |  |  |  |  |  |  |  |
```

audiodatarate(24 bytes)

```
| 00 | 0D | a | u | d | i | o | d | a | t | a | r | a | t | e | 00 |  |  |  |  |  |  |  |  |
```

audiosamplerate(26 bytes)

```
| 00 | 0F | a | u | d | i | o | s | a | m | p | l | e | r | a | t | e | 00 |  |  |  |  |  |  |  |  |
```

audiosamplesize(26 bytes)

```
| 00 | 0F | a | u | d | i | o | s | a | m | p | l | e | s | i | z | e | 00 |  |  |  |  |  |  |  |  |
```

stereo(10 bytes)

```
| 00 | 06 | s | t | e | r | e | o | 01 | 00/01 |
```

audiocodecid(23 bytes)

```
| 00 | 0C | a | u | d | i | o | c | o | d | e | c | i | d | 00 |  |  |  |  |  |  |  |  |
```

major_brand

```
| 00 | 0B | m | a | j | o | r | _ | b | r | a | n | d | 02 | 00 | 04 | d | b | y | `1` |
```

minor_version

```
| 00 | 0D | m | i | n | o | r | _ | v | e | r | s | i | o | n | 02 | 00 | 01 | 00 |
```

compatible_brands

```
| 00| 11 | c | o | m | p | a | t | i | b | l | e | _ | b | r | a | n | d | s | 
```

```
| 02 | 00 | 0C | i | s | o | m | m | p | 4 | 2 | d | b | y | `1` |
```

encoder

```
| 00 | 07 | e | n | c | o | d | e | r | 02 | 00 | 0C | L | a | v | f | 5 | 5 | . | 9 | . | 1 | 0 | 0 |
```

filesize

```
| 00 | 08 | f | i | l | e | s | i | z | e | 00 |  |  |  |  |  |  |  |  | 00 | 00 | 09 |
```

### VideoTag

#### 第一个Video Tag

```
| 17 | 00 | 00 | 00 | 00 |
```

```
| 01 | sps[1] | sps[2] | sps[3] | FF |
```

sps[1]: Profile
sps[2]: Profile Compact
sps[3]: Level

```
| E1 |  |  | --- SPS --- | 01 |  |  | --- PPS --- |
      SPS Size                 PPS Size
```

E1: (Reserved << 5) | Number_Of_SPS = (0x07 << 5) | 0x01 = 0xE1

#### 后续普通的Video Tag

```
| 17/27 | 01 | 00 | 00 | 00 |  |  |  |  | ... ... |
                             NALU Length NALU Data
```

### AudioTag

#### 第一个Audio Tag

```
| AF | 00 | 12 | 30 | 56 | E5 | 00 |
            AudioSpecificConfig
```
AF: (SoundFormat << 4) | (SoundRate << 2) | (SoundSize << 1) | SoundType + (0x0A << 4) | (0x03 << 2) | (0x01 << 1) | 0x01 = 0xAF

AudioSpecificConfig: 
Object Type(5 bits): 2-AAC-LC, 5-SBR, 29-PS
Samplerate Index(4 bits): 0-9600, 1-88200, 3-64000, 4-44100, 5-32000, 6-24000, 7-22050, 8-16000.
Channels(4 bits): 1-单声道, 2-双声道
Frame Length Flag(1 bit): 0 - 1024 samples
Depend On Core Coder(1 bit): 0 - 不依赖, 1 - 依赖
Extension Flag(1 bit): 0 - Is not extension 1 - Is extension

#### 后续普通的Audio Tag

```
| AF | 01 | ... ... |
            AAC Data
```
AF: (SoundFormat << 4) | (SoundRate << 2) | (SoundSize << 1) | SoundType = 0x0A << 4) | (0x03 << 2) | (0x01 << 1) | 0x01 = 0xAF

## 参考文献

- [Adobe Flash Video File Format Specification Version 10.1](http://download.macromedia.com/f4v/video_file_format_spec_v10_1.pdf)
- [pili-camera-sdk-demo 源码](https://github.com/pili-io/pili-camera-sdk-demo)

## 版本历史
- 0.1.0
	- clib 版本创建
	- 实现 FLV 文件直推