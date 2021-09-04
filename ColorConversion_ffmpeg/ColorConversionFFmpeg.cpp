#include "ColorConversionFFmpeg.h"

ColorConversionFFmpeg::ColorConversionFFmpeg()
	: m_imgConvertCtx(nullptr)
	, m_infun(nullptr)
	, m_outfun(nullptr)
	, m_srcHigh(0)
	, m_srcWide(0)
{

	m_PixelFormatMap.insert(std::pair<FFmpegAVPixelFormat, AVPixelFormat>(FFMPEG_AV_PIX_FMT_NV12, AV_PIX_FMT_NV12));
	m_PixelFormatMap.insert(std::pair<FFmpegAVPixelFormat, AVPixelFormat>(FFMPEG_AV_PIX_FMT_NV21, AV_PIX_FMT_NV21));
	m_PixelFormatMap.insert(std::pair<FFmpegAVPixelFormat, AVPixelFormat>(FFMPEG_AV_PIX_FMT_YUV420P, AV_PIX_FMT_YUV420P));
	m_PixelFormatMap.insert(std::pair<FFmpegAVPixelFormat, AVPixelFormat>(FFMPEG_AV_PIX_FMT_YUV422P, AV_PIX_FMT_YUV422P));
	m_PixelFormatMap.insert(std::pair<FFmpegAVPixelFormat, AVPixelFormat>(FFMPEG_AV_PIX_FMT_RGB24, AV_PIX_FMT_RGB24));
	m_PixelFormatMap.insert(std::pair<FFmpegAVPixelFormat, AVPixelFormat>(FFMPEG_AV_PIX_FMT_RGBA, AV_PIX_FMT_BGRA));

	m_srcFormatFunMap[FFMPEG_AV_PIX_FMT_NV12] = std::bind(&ColorConversionFFmpeg::BuffToAVPixFmtNV12,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_srcFormatFunMap[FFMPEG_AV_PIX_FMT_NV21] = std::bind(&ColorConversionFFmpeg::BuffToAVPixFmtNV21,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_srcFormatFunMap[FFMPEG_AV_PIX_FMT_YUV420P] = std::bind(&ColorConversionFFmpeg::BuffToAVPixFmtYUV420P,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_srcFormatFunMap[FFMPEG_AV_PIX_FMT_YUV422P] = std::bind(&ColorConversionFFmpeg::BuffToAVPixFmtYUV422P,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_srcFormatFunMap[FFMPEG_AV_PIX_FMT_RGB24] = std::bind(&ColorConversionFFmpeg::BuffToAVPixFmtRGB24,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_srcFormatFunMap[FFMPEG_AV_PIX_FMT_RGBA] = std::bind(&ColorConversionFFmpeg::BuffToAVPixFmtRGBA,
		this,
		std::placeholders::_1,
		std::placeholders::_2);


	m_dstFormatFunMap[FFMPEG_AV_PIX_FMT_NV12] = std::bind(&ColorConversionFFmpeg::AVPixFmtNV12ToBuff,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_dstFormatFunMap[FFMPEG_AV_PIX_FMT_NV21] = std::bind(&ColorConversionFFmpeg::AVPixFmtNV21ToBuff,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_dstFormatFunMap[FFMPEG_AV_PIX_FMT_YUV420P] = std::bind(&ColorConversionFFmpeg::AVPixFmtYUV420PToBuff,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_dstFormatFunMap[FFMPEG_AV_PIX_FMT_YUV422P] = std::bind(&ColorConversionFFmpeg::AVPixFmtYUV422PToBuff,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_dstFormatFunMap[FFMPEG_AV_PIX_FMT_RGB24] = std::bind(&ColorConversionFFmpeg::AVPixFmtRGB24ToBuff,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_dstFormatFunMap[FFMPEG_AV_PIX_FMT_RGBA] = std::bind(&ColorConversionFFmpeg::AVPixFmtRGBAToBuff,
		this,
		std::placeholders::_1,
		std::placeholders::_2);

}

ColorConversionFFmpeg::~ColorConversionFFmpeg()
{
	m_PixelFormatMap.clear();
	m_srcFormatFunMap.clear();
	m_dstFormatFunMap.clear();

}

long ColorConversionFFmpeg::Init(FFmpegSwscaleConfig* cfg)
{
	if (nullptr == cfg)
	{
		return -1;
	}
	auto srcIter = m_PixelFormatMap.find(cfg->srcFormat);
	auto dstIter = m_PixelFormatMap.find(cfg->dstFormat);
	if (srcIter == m_PixelFormatMap.end() ||
		dstIter == m_PixelFormatMap.end())
	{
		return -2;
	}
	auto srcFormatFunIter = m_srcFormatFunMap.find(cfg->srcFormat);
	auto dstFormatFunIter = m_dstFormatFunMap.find(cfg->dstFormat);
	if (dstFormatFunIter == m_dstFormatFunMap.end() ||
		srcFormatFunIter == m_srcFormatFunMap.end())
	{
		return -3;
	}

	m_infun = srcFormatFunIter->second;
	m_outfun = dstFormatFunIter->second;

	int nSrctBuffLen = 0, nDstBuffLen = 0;

	nSrctBuffLen = av_image_alloc(m_srcPointers, m_srcLinesizes, cfg->srcWide, cfg->srcHigh, srcIter->second, 1);
	if (nSrctBuffLen <= 0)
	{
		return -4;
	}
	nDstBuffLen = av_image_alloc(m_dstPointers, m_dstLinesizes, cfg->dstWide, cfg->dstHigh, dstIter->second, 1);
	if (nDstBuffLen <= 0 )
	{
		av_freep(&m_srcPointers[0]);
		return -5;
	}

	m_imgConvertCtx = sws_getContext(cfg->srcWide, cfg->srcHigh, srcIter->second,
		cfg->dstWide, cfg->dstHigh, dstIter->second, SWS_BICUBIC, NULL, NULL, NULL);
	
	if (nullptr == m_imgConvertCtx)
	{
		av_freep(&m_srcPointers);
		av_freep(&m_dstPointers);
		return -6;
	}

	m_srcHigh = cfg->srcHigh;
	m_srcWide = cfg->srcWide;

	return 0;
}

long ColorConversionFFmpeg::Conversion(const char* inputBuff, char* outputBuff)
{
	if (nullptr == m_infun ||
		nullptr == m_outfun ||
		nullptr == m_dstPointers[0] ||
		nullptr == m_srcPointers[0] ||
		nullptr == m_imgConvertCtx)
	{
		return 0;
	}
	
	m_infun(const_cast<char*>(inputBuff), m_srcPointers);

	sws_scale(m_imgConvertCtx, m_srcPointers, m_srcLinesizes, 0, m_srcHigh, m_dstPointers, m_dstLinesizes);

	m_outfun(m_dstPointers, outputBuff);
	return 0;
}

long ColorConversionFFmpeg::UnInit()
{
	if (m_srcPointers)
	{
		av_freep(&m_srcPointers);
	}
	if (m_dstPointers)
	{
		av_freep(&m_dstPointers);
	}

	m_dstPointers[0] = nullptr;
	m_srcPointers[0] = nullptr;

	if (nullptr != m_imgConvertCtx)
	{
		sws_freeContext(m_imgConvertCtx);
	}

	m_imgConvertCtx = nullptr;
	m_outfun = nullptr;
	m_infun = nullptr;

	return 0;
}

long ColorConversionFFmpeg::BuffToAVPixFmtYUV420P(char* inputBuff, unsigned char** pixBuff)
{
	memcpy(pixBuff[0], inputBuff, static_cast<size_t>(m_srcWide * m_srcHigh));											//Y
	memcpy(pixBuff[1], inputBuff + m_srcWide * m_srcHigh, m_srcWide * m_srcHigh / 4);				//U
	memcpy(pixBuff[2], inputBuff + m_srcWide * m_srcHigh * 5 / 4, m_srcWide * m_srcHigh / 4);		//V
	return 0;
}

long ColorConversionFFmpeg::BuffToAVPixFmtRGBA(char* inputBuff, unsigned char** pixBuff)
{
	memcpy(pixBuff[0], inputBuff, m_srcWide * m_srcHigh*4);
	return 0;
}

long ColorConversionFFmpeg::BuffToAVPixFmtRGB24(char* inputBuff, unsigned char** pixBuff)
{
	memcpy(pixBuff[0], inputBuff, m_srcWide * m_srcHigh * 3);
	return 0;
}

long ColorConversionFFmpeg::BuffToAVPixFmtNV12(char* inputBuff, unsigned char** pixBuff)
{
	memcpy(pixBuff[0], inputBuff, m_srcHigh*m_srcWide);                    //Y
	memcpy(pixBuff[1], inputBuff + m_srcHigh * m_srcWide, m_srcHigh*m_srcWide / 2);      //Uv
	return 0;
}

long ColorConversionFFmpeg::BuffToAVPixFmtNV21(char* inputBuff, unsigned char** pixBuff)
{
	memcpy(pixBuff[0], inputBuff, m_srcHigh * m_srcWide);                    //Y
	memcpy(pixBuff[1], inputBuff + m_srcHigh * m_srcWide, m_srcHigh * m_srcWide / 2);      //Uv
	return 0;
}

long ColorConversionFFmpeg::BuffToAVPixFmtYUV422P(char* inputBuff, unsigned char** pixBuff)
{
	memcpy(pixBuff[0], inputBuff, m_srcWide * m_srcHigh);											//Y
	memcpy(pixBuff[1], inputBuff + m_srcWide * m_srcHigh, m_srcWide * m_srcHigh / 2);				//U
	memcpy(pixBuff[2], inputBuff + m_srcWide * m_srcHigh * 3 / 2, m_srcWide * m_srcHigh / 2);		//V
	return 0;
}

long ColorConversionFFmpeg::AVPixFmtYUV420PToBuff(unsigned char** pixBuff, char* outputBuff)
{
	memcpy(outputBuff, pixBuff[0], m_srcWide * m_srcHigh);											//Y
	memcpy(outputBuff + m_srcWide * m_srcHigh, pixBuff[1], m_srcWide * m_srcHigh / 4);				//U
	memcpy(outputBuff + m_srcWide * m_srcHigh * 5 / 4, pixBuff[2], m_srcWide * m_srcHigh / 4);		//V
	return 0;
}

long ColorConversionFFmpeg::AVPixFmtNV12ToBuff(unsigned char** pixBuff, char* outputBuff)
{
	memcpy( outputBuff, pixBuff[0], m_srcHigh * m_srcWide);                    //Y
	memcpy( outputBuff + m_srcHigh * m_srcWide, pixBuff[1], m_srcHigh * m_srcWide / 2);      //Uv
	return 0;
}

long ColorConversionFFmpeg::AVPixFmtNV21ToBuff(unsigned char** pixBuff, char* outputBuff)
{
	memcpy(outputBuff, pixBuff[0], m_srcHigh * m_srcWide);                    //Y
	memcpy(outputBuff + m_srcHigh * m_srcWide, pixBuff[1], m_srcHigh * m_srcWide / 2);      //Uv
	return 0;
}

long ColorConversionFFmpeg::AVPixFmtYUV422PToBuff(unsigned char** pixBuff, char* outputBuff)
{
	memcpy(outputBuff, pixBuff[0], m_srcWide * m_srcHigh);											//Y
	memcpy(outputBuff + m_srcWide * m_srcHigh, pixBuff[1], m_srcWide * m_srcHigh / 2);				//U
	memcpy(outputBuff + m_srcWide * m_srcHigh * 3 / 2, pixBuff[2], m_srcWide * m_srcHigh / 2);		//V
	return 0;
}

long ColorConversionFFmpeg::AVPixFmtRGB24ToBuff(unsigned char** pixBuff, char* outputBuff)
{
	memcpy(outputBuff, pixBuff[0], m_srcWide * m_srcHigh * 3);
	return 0;
}

long ColorConversionFFmpeg::AVPixFmtRGBAToBuff(unsigned char** pixBuff, char* outputBuff)
{
	memcpy(outputBuff, pixBuff[0], m_srcWide * m_srcHigh * 4);
	return 0;
}
