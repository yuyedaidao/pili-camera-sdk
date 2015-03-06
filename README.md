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

**提示：** pili_write_video_packet 和 pili_write_audio_packet 两个方法是线程安全的。

## 封包详解

### MetaData 包
TODO
### VideoTag
TODO
### AudioTag
TODO

## 参考文献

- [Adobe Flash Video File Format Specification Version 10.1](http://download.macromedia.com/f4v/video_file_format_spec_v10_1.pdf)
- [pili-camera-sdk-demo 源码](https://github.com/pili-io/pili-camera-sdk-demo)

## 版本历史
- 0.1.0
	- clib 版本创建
	- 实现 FLV 文件直推