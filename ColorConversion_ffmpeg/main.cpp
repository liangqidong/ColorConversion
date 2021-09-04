/**
* FFmpeg的颜色空间转换的测试程序
* YUV Transformation
*
* 梁启东 qidong.liang
* 18088708700@163.com
* https://blog.csdn.net/u011645307
*
*
* FFmpeg的颜色空间转换的测试程序
*/

#include <iostream>
#include "ColorConversionFFmpeg.h"


#define NV12_To_I420	0
#define I420_To_NV12	0
#define NV21_To_I420	0
#define I420_To_NV21	0
#define I420_To_RGB32	0
#define RGB32_To_I420	0
#define I420_To_RGB24	0
#define RGB24_To_I420	0
#define NV12_To_YUV422P	0
#define YUV422P_To_NV12	1
int main()
{
	FILE* file_in = nullptr;
	FILE* file_out = nullptr;
	char* input_name = nullptr;
	char* output_name = nullptr;

	int w = 0, h = 0;
	float flotScale = 0;
	int out_w = 0, out_h = 0;
	float out_flotScale = 0;

	FFmpegSwscaleConfig cfg;
	ColorConversionFFmpeg obj;

#if NV12_To_YUV422P
	input_name = const_cast<char*>("../in/nv21_480x272.yuv");
	output_name = const_cast<char*>("../out/yuvv422p_480x272.yuv");

	cfg.srcWide = 480;
	cfg.dstWide = 480;
	cfg.dstHigh = 272;
	cfg.srcHigh = 272;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_NV12;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_YUV422P;

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 2;

#endif

#if YUV422P_To_NV12

	input_name = const_cast<char*>("../in/YV16(422)_480x272.yuv");
	output_name = const_cast<char*>("../out/nv21_480x272.yuv");
	cfg.srcWide = 480;
	cfg.dstWide = 480;
	cfg.dstHigh = 272;
	cfg.srcHigh = 272;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_YUV422P;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_NV12;

	w = 480;
	h = 272;
	flotScale = 2;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;

#endif

#if NV21_To_I420
	input_name = const_cast<char*>("../in/nv21_480x272.yuv");
	output_name = const_cast<char*>("../out/I420_480x272.yuv");

	cfg.srcWide = 480;
	cfg.dstWide = 480;
	cfg.dstHigh = 272;
	cfg.srcHigh = 272;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_NV21;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_YUV420P;

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;

#endif

#if I420_To_NV21

	input_name = const_cast<char*>("../in/I420_480x272.yuv");
	output_name = const_cast<char*>("../out/nv21_480x272.yuv");
	cfg.srcWide = 480;
	cfg.dstWide = 480;
	cfg.dstHigh = 272;
	cfg.srcHigh = 272;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_YUV420P;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_NV21;

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;

#endif

#if NV12_To_I420
	input_name = const_cast<char*>("../in/nv12_480x272.yuv");
	output_name = const_cast<char*>("../out/I420_480x272.yuv");

	cfg.srcWide = 480;
	cfg.dstWide = 480;
	cfg.dstHigh = 272;
	cfg.srcHigh = 272;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_NV12;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_YUV420P;

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;

#endif

#if I420_To_NV12

	input_name = const_cast<char*>("../in/I420_480x272.yuv");
	output_name = const_cast<char*>("../out/nv12_480x272.yuv");
	cfg.srcWide = 480;
	cfg.dstWide = 480;
	cfg.dstHigh = 272;
	cfg.srcHigh = 272;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_YUV420P;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_NV12;

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;

#endif

#if I420_To_RGB24
	input_name = const_cast<char*>("../in/I420_480x272.yuv");
	output_name = const_cast<char*>("../out/rgb_480x272.rgb");

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 3;

	cfg.srcWide = w;
	cfg.dstWide = out_w;
	cfg.dstHigh = out_h;
	cfg.srcHigh = h;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_YUV420P;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_RGB24;


#endif

#if RGB24_To_I420
	input_name = const_cast<char*>("../in/rgb_480x272.rgb");
	output_name = const_cast<char*>("../out/I420_480x272.yuv");

	w = 480;
	h = 272;
	flotScale = 3;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;

	cfg.srcWide = w;
	cfg.dstWide = out_w;
	cfg.dstHigh = out_h;
	cfg.srcHigh = h;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_YUV420P;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_RGB24;

#endif

#if I420_To_RGB32
	input_name = const_cast<char*>("../in/I420_480x272.yuv");
	output_name = const_cast<char*>("../out/rgba_480x272.rgb");

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 4;

	cfg.srcWide = w;
	cfg.dstWide = out_w;
	cfg.dstHigh = out_h;
	cfg.srcHigh = h;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_YUV420P;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_RGBA;

#endif

#if RGB32_To_I420
	input_name = const_cast<char*>("../in/rgba_480x272.rgb");
	output_name = const_cast<char*>("../out/I420_480x272.yuv");

	w = 480;
	h = 272;
	flotScale = 4;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;

	cfg.srcWide = w;
	cfg.dstWide = out_w;
	cfg.dstHigh = out_h;
	cfg.srcHigh = h;
	cfg.dstFormat = FFMPEG_AV_PIX_FMT_YUV420P;
	cfg.srcFormat = FFMPEG_AV_PIX_FMT_RGBA;

#endif

	int in_buff_len = w * h * flotScale;
	int out_buff_len = out_w * out_h * out_flotScale;
	char* inbuff = new char[in_buff_len];
	char* outbuff = new char[out_buff_len];
	fopen_s(&file_in, input_name, "rb+");
	fopen_s(&file_out, output_name, "wb+");


	int ret = obj.Init(&cfg);
	if (0 != ret)
	{
		printf("ColorConversionFFmpeg::Init ret:%d\n", ret);
		fclose(file_in);
		fclose(file_out);
		file_in = nullptr;
		file_out = nullptr;
		return -1;
	}
	while (true)
	{
		if (fread(inbuff, 1, in_buff_len, file_in) != in_buff_len)
		{
			break;
		}

		ret = obj.Conversion(inbuff, outbuff);
		if (0 != ret)
		{
			printf("ColorConversionFFmpeg::Conversion ret:%d\n", ret);
			continue;
		}
		fwrite(outbuff, 1, out_buff_len, file_out);
	}
	ret = obj.UnInit();
	if (0 != ret)
	{
		printf("ColorConversionFFmpeg::UnInit ret:%d\n", ret);
	}
	fclose(file_in);
	fclose(file_out);
	file_in = nullptr;
	file_out = nullptr;


    std::cout << "Hello World!\n";
}
