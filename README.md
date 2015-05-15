# pili-camera-sdk

pili-camera-sdk 是 **pili 流媒体云服务** 开发团队为 IPCamera 量身定制的推流 SDK。这套 SDK 旨在为摄像头开发团队提供更开放、更通用的推流方案，缩短开发周期，更便捷、快速、轻松的接入 **pili 流媒体云服务**。


## 快速开始

### 下载配置 SDK

```
# step 1
git clone https://github.com/pili-io/pili-camera-sdk.git

# step 2
添加头文件 src/include 到 header search paths 中

# step 3
添加 src/lib 到 library search paths 中
```

如果你使用 cmake 编译项目，可以参考 [demo 项目](https://github.com/pili-io/pili-camera-sdk-demo) 的 CMakeFile

### 示例代码

```
# step 1
# 创建并初始化 stream context
ctx = pili_create_stream_context();
pili_init_stream_context(ctx, PILI_STREAM_DROP_FRAME_POLICY_RANDOM, PILI_STREAM_BUFFER_TIME_INTERVAL_DEFAULT, stream_state_cb);

# step 2
# 开启推流
char *url = "rtmp://YOUR_PUSH_URL";
pili_stream_push_open(ctx, url);

# step 3
# 封装音视频包并发送
pili_write_packet(ctx, flv_tag);

# step 4
# 结束推流并释放上下文
pili_stream_push_close(ctx);
pili_stream_context_release(ctx);
```

## 依赖库

- libz
- libssl
- libcrypto

## 功能特性

- 多种丢帧策略选择
	- [x] 随机丢帧
	- 视频关键帧连带丢帧 (Coming soon)
- [x] 网络队列支持
- [x] 低内存占用优化

## 参考文献

- [Adobe Flash Video File Format Specification Version 10.1](http://download.macromedia.com/f4v/video_file_format_spec_v10_1.pdf)
- [pili-camera-sdk-demo 源码](https://github.com/pili-io/pili-camera-sdk-demo)

## 版本历史

- 0.1.1
	- 更新接口
	- 优化内存占用
- 0.1.0
	- 实现 FLV 文件直推