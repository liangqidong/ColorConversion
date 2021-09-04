/**
 * FFmpeg的颜色空间转换
 * YUV Transformation
 *
 * 梁启东 qidong.liang
 * 18088708700@163.com
 * https://blog.csdn.net/u011645307
 *
 *
 * 本程序实现了FFmpeg的YUV数据之间的转换和YUV与RGB的转换。
 * 提供了如下：
 * 	FFMPEG_AV_PIX_FMT_NOKNOW,
 *	FFMPEG_AV_PIX_FMT_NV12,
 *	FFMPEG_AV_PIX_FMT_NV21,
 *	FFMPEG_AV_PIX_FMT_YUV420P,
 *	FFMPEG_AV_PIX_FMT_YUV422P,
 *	FFMPEG_AV_PIX_FMT_RGB24,
 *	FFMPEG_AV_PIX_FMT_RGBA
 *  相互转换功能
 */
#ifndef COLOR_CONVERSION_FFMPEG_H
#define	COLOR_CONVERSION_FFMPEG_H

#ifdef _WIN32
//Windows
extern "C"
{
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
};
#endif
#endif

#include <map>
#include <functional>

#ifndef FFMPEG_PIX_FORMAT
#define	FFMPEG_PIX_FORMAT
typedef enum FFmpegAVPixelFormat
{
	FFMPEG_AV_PIX_FMT_NOKNOW,
	FFMPEG_AV_PIX_FMT_NV12,
	FFMPEG_AV_PIX_FMT_NV21,
	FFMPEG_AV_PIX_FMT_YUV420P,
	FFMPEG_AV_PIX_FMT_YUV422P,
	FFMPEG_AV_PIX_FMT_RGB24,
	FFMPEG_AV_PIX_FMT_RGBA

}FFmpegAVPixelFormat;

#endif//FFMPEG_PIX_FORMAT
#ifndef FFMPEG_SCALE_CONFIG
#define	FFMPEG_SCALE_CONFIG
typedef struct FFmpegSwscaleConfig
{
	unsigned int srcWide;
	unsigned int srcHigh;
	FFmpegAVPixelFormat srcFormat;
	unsigned int dstWide;
	unsigned int dstHigh;
	FFmpegAVPixelFormat dstFormat;

	FFmpegSwscaleConfig()
	{
		srcWide = 0;
		srcHigh = 0;
		srcFormat = FFMPEG_AV_PIX_FMT_NOKNOW;
		dstWide = 0;
		dstHigh = 0;
		dstFormat = FFMPEG_AV_PIX_FMT_NOKNOW;
	}
}FFmpegSwscaleConfig;
#endif // !FFMPEG_SCALE_CONFIG

class ColorConversionFFmpeg
{
public:
	ColorConversionFFmpeg();
	~ColorConversionFFmpeg();

	long Init(FFmpegSwscaleConfig* cfg);
	long Conversion(const char* inputBuff, char* outputBuff);
	long UnInit();

private:
	long BuffToAVPixFmtYUV420P(char* inputBuff, unsigned char** pixBuff);
	long BuffToAVPixFmtRGBA(char* inputBuff, unsigned char** pixBuff);
	long BuffToAVPixFmtRGB24(char* inputBuff, unsigned char** pixBuff);
	long BuffToAVPixFmtNV12(char* inputBuff, unsigned char** pixBuff);
	long BuffToAVPixFmtNV21(char* inputBuff, unsigned char** pixBuff);
	long BuffToAVPixFmtYUV422P(char* inputBuff, unsigned char** pixBuff);

	long AVPixFmtYUV420PToBuff(unsigned char** pixBuff, char* outputBuff);
	long AVPixFmtNV12ToBuff(unsigned char** pixBuff, char* outputBuff);
	long AVPixFmtNV21ToBuff(unsigned char** pixBuff, char* outputBuff);
	long AVPixFmtYUV422PToBuff(unsigned char** pixBuff, char* outputBuff);
	long AVPixFmtRGB24ToBuff(unsigned char** pixBuff, char* outputBuff);
	long AVPixFmtRGBAToBuff(unsigned char** pixBuff, char* outputBuff);

private:
	SwsContext* m_imgConvertCtx;
	uint8_t* m_srcPointers[4]{ nullptr,nullptr,nullptr,nullptr };
	int m_srcLinesizes[4]{0,0,0,0};
	uint8_t* m_dstPointers[4]{ nullptr,nullptr,nullptr,nullptr };
	int m_dstLinesizes[4]{ 0,0,0,0 };

	int m_srcHigh;
	int m_srcWide;
	std::function < long(char* inputBuff, unsigned char** pixBuff) > m_infun;
	std::function < long(unsigned char** pixBuff, char* outputBuff) > m_outfun;
	std::map<FFmpegAVPixelFormat, AVPixelFormat>			m_PixelFormatMap;
	std::map<FFmpegAVPixelFormat,
		std::function < long(
			char* inputBuff,
			unsigned char** pixBuff) >>					    m_srcFormatFunMap;
	std::map<FFmpegAVPixelFormat,
		std::function < long(
			unsigned char** pixBuff,
			char* outputBuff) >>						    m_dstFormatFunMap;
};
#endif//COLOR_CONVERSION_FFMPEG_H
