# pili-camera-sdk

pili-camera-sdk 是一个适用于 IPCamera 的 RTMP 直播推流 SDK，开发者可以直接写入 h264 NAL 数据，无需做视频内容的封装，小巧、简洁。

## 功能特性

- [x] h264 NAL 数据无需开发者封装
- [x] 低内存占用
- [x] 简单握手支持

## 快速开始

### 下载配置 SDK

```
# step 1
git clone https://github.com/pili-io/pili-camera-sdk.git

# step 2
创建 CMakeList.txt

# step 3
编译
```

CMakeList.txt 可以参考 [demo 项目](https://github.com/pili-io/pili-camera-sdk-demo)

### 示例代码

文档最后会附带 demo 的源码地址，这里只做整体流程和主要代码块的解释。

```
// 初始化 stream context
// pili_stream_context_init 方法中的 stream_state_cb 回调
// 是用于在流状态变更后获取状态的回调函数
pili_stream_context_p ctx = pili_create_stream_context();
pili_init_stream_context(ctx, 
                         PILI_STREAM_DROP_FRAME_POLICY_RANDOM,
                         PILI_STREAM_BUFFER_TIME_INTERVAL_DEFAULT,
                         stream_state_cb);
```

```
// 开启推流
char *url = "rtmp://YOUR_PUSH_URL";
pili_stream_push_open(ctx, url);
```

```
// 封装 FLV Tag
flv_tag_p tap = flv_create_tag();
int flv_init_tag(tag,
                 tag_type,
                 data_size,
                 timestamp,
                 stream_id,
                 data);
```

```
// h264 NAL 数据直接发送
// 对于 h264 编码的视频数据，SDK 提供了直接写入的方法，FLV Tag 的封装在 SDK 内部完成，开发者无需关心封装琐事。

// 写入关键帧
pili_h264_key_frame_t key_frame;
key_frame.sps = sps;
key_frame.pps = pps;
key_frame.sei = sei;
key_frame.idr = idr;
pili_write_h264_key_frame(ctx, key_frame, ts);

// 写入非关键帧
pili_h264_nalu_t nalu;
nalu.data = data;
nalu.length = data_length;
pili_write_h264_slice(ctx, nalu, ts)
```

```
// 发送数据
pili_write_packet(ctx, tag);
```

```
// 结束推流并释放上下文
pili_stream_push_close(ctx);
pili_stream_context_release(ctx);
```

## 封包详解

可参考 Adobe 的 [Video File Format Specification Version 10](http://www.adobe.com/content/dam/Adobe/en/devnet/flv/pdfs/video_file_format_spec_v10.pdf)

## 参考文献

- [Adobe Flash Video File Format Specification Version 10.1](http://download.macromedia.com/f4v/video_file_format_spec_v10_1.pdf)
- [pili-camera-sdk-demo 源码](https://github.com/pili-io/pili-camera-sdk-demo)

## 版本历史

- 0.2.0
   - 更新接口
   - 添加 FLV 结构
   - 添加 h264 nalu 数据直接推送
   - 更新为简单握手
- 0.1.0
	- 实现 FLV 文件直推