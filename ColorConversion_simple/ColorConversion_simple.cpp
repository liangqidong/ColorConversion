/**
 * 简单的颜色空间转换 
 * YUV Transformation
 *
 * 梁启东 qidong.liang
 * 18088708700@163.com
 * https://blog.csdn.net/u011645307
 *
 *
 * 本程序实现了简单的YUV数据之间的转换和YUV与RGB的转换。
 * This program realizes the simple conversion between
 * YUV data and the conversion between YUV and RGB.
 *
 */

#include <iostream>

#define YV12_To_I420	0
#define I420_To_YV12	0
#define NV12_To_I420	0
#define I420_To_NV12	0
#define NV21_To_YV12	0
#define YV12_To_NV21	0
#define I420_To_RGB32	0
#define RGB32_To_I420	0
#define I420_To_RGB24	0
#define RGB24_To_I420	1

int YV12_2_I420(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
int I420_2_YV12(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
int NV12_2_I420(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
int I420_2_NV12(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
int NV21_2_YV12(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
int YV12_2_NV21(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
int I420_2_rgb32(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
int rgb32_2_I420(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
int I420_2_rgb24(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
int rgb24_2_I420(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);


typedef  struct  RGB32 {
	uint8_t     rgbBlue;       // 蓝色分量
	uint8_t     rgbGreen;      // 绿色分量
	uint8_t     rgbRed;        // 红色分量
	uint8_t     rgbReserved;   // 保留字节（用作Alpha通道或忽略）
	RGB32()
	{
		rgbBlue = 0;
		rgbGreen = 0;
		rgbRed = 0;
		rgbReserved = 0;
	}
} RGB32;

typedef  struct  RGB24 {
	uint8_t     rgbBlue;       // 蓝色分量
	uint8_t     rgbGreen;      // 绿色分量
	uint8_t     rgbRed;        // 红色分量
	RGB24()
	{
		rgbBlue = 0;
		rgbGreen = 0;
		rgbRed = 0;
	}
} RGB24;

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
    int (*pfun)(uint8_t * src, uint8_t * dst, uint32_t width, uint32_t height);
	pfun = nullptr;
#if YV12_To_I420
    input_name = const_cast<char*>("../in/yv12_480x272.yuv");
    output_name = const_cast<char*>("../out/i420_480x272.yuv");
	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;
    pfun = YV12_2_I420;
#endif

#if I420_To_YV12
    input_name = const_cast<char*>("../in/I420_480x272.yuv");
    output_name = const_cast<char*>("../out/yv12_480x272.yuv");
    w = 480;
    h = 272;
    flotScale = 1.5;
    out_w = 480;
    out_h = 272;
    out_flotScale = 1.5;
    pfun = I420_2_YV12;
#endif

#if NV12_To_I420
    input_name = const_cast<char*>("../in/nv12_480x272.yuv");
    output_name = const_cast<char*>("../out/I420_480x272.yuv");
    
	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;
    pfun = NV12_2_I420;
#endif

#if I420_To_NV12
	input_name = const_cast<char*>("../in/I420_480x272.yuv");
	output_name = const_cast<char*>("../out/nv12_480x272.yuv");
	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;
    pfun = I420_2_NV12;
#endif

#if NV21_To_YV12
	input_name = const_cast<char*>("../in/nv21_480x272.yuv");
	output_name = const_cast<char*>("../out/yv12_480x272.yuv");

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;
    pfun = NV21_2_YV12;
#endif

#if YV12_To_NV21
	input_name = const_cast<char*>("../in/yv12_480x272.yuv");
	output_name = const_cast<char*>("../out/nv21_480x272.yuv");

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;
	pfun = YV12_2_NV21;
#endif

#if I420_To_RGB32
	input_name = const_cast<char*>("../in/I420_480x272.yuv");
	output_name = const_cast<char*>("../out/rgb32_480x272.yuv");

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 4;
	pfun = I420_2_rgb32;
#endif

#if RGB32_To_I420
	input_name = const_cast<char*>("rgb32_480x272.yuv");
	output_name = const_cast<char*>("../out/i420_480x272.yuv");

	w = 480;
	h = 272;
	flotScale = 4;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;
	pfun = rgb32_2_I420;

#endif


#if I420_To_RGB24
	input_name = const_cast<char*>("../in/I420_480x272.yuv");
	output_name = const_cast<char*>("../out/rgb24_480x272.yuv");

	w = 480;
	h = 272;
	flotScale = 1.5;
	out_w = 480;
	out_h = 272;
	out_flotScale = 3;
	pfun = I420_2_rgb24;
#endif

#if RGB24_To_I420
	input_name = const_cast<char*>("../in/rgb24_480x272.yuv");
	output_name = const_cast<char*>("../out/i420_480x272.yuv");

	w = 480;
	h = 272;
	flotScale = 3;
	out_w = 480;
	out_h = 272;
	out_flotScale = 1.5;
	pfun = rgb24_2_I420;

#endif
	int in_buff_len = w * h * flotScale;
    int out_buff_len = out_w * out_h * out_flotScale;
	char* inbuff = new char[in_buff_len];
	char* outbuff = new char[out_buff_len];
    fopen_s(&file_in, input_name, "rb+");
    fopen_s(&file_out, output_name, "wb+");

    while (true)
    {
		if (fread(inbuff, 1, in_buff_len, file_in) != in_buff_len)
		{
			break;
		}

        pfun((uint8_t*)inbuff, (uint8_t*)outbuff, w,h);

        fwrite(outbuff, 1,out_buff_len, file_out);
    }

    fclose(file_in);
    fclose(file_out);
    file_in = nullptr;
    file_out = nullptr;

    std::cout << "Hello World!\n";
    return 0;
}

int YV12_2_I420(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	uint8_t* src_y = src;
	uint8_t* src_u = src + width * height;
	uint8_t* src_v = src + width * height * 5 / 4;

	uint8_t* dst_y = dst;
	uint8_t* dst_v = dst + width * height;
	uint8_t* dst_u = dst + width * height * 5 / 4;

	memcpy(dst_y, src_y, width * height);
	memcpy(dst_v, src_v, width * height / 4);
	memcpy(dst_u, src_u, width * height / 4);
    return 0;
}

int I420_2_YV12(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
    uint8_t* src_y = src;
    uint8_t* src_u = src + width* height;
    uint8_t* src_v = src + width * height*5/4;

	uint8_t* dst_y = dst;
	uint8_t* dst_v = dst + width * height;
    uint8_t* dst_u = dst + width * height * 5 / 4;

    memcpy(dst_y, src_y, width * height);
    memcpy(dst_v, src_v, width * height/4);
    memcpy(dst_u, src_u, width * height/4);
    return 0;
}

int NV12_2_I420(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	uint8_t* src_y = src;
	uint8_t* src_uv = src + width * height;

	uint8_t* dst_y = dst;
	uint8_t* dst_u = dst + width * height;
	uint8_t* dst_v = dst + width * height * 5 / 4;

    memcpy(dst_y, src_y, width * height);
	for (int i = 0; i < width/2 * height / 2; i++)
	{
        dst_u[i] = src_uv[2 * i];
        dst_v[i] = src_uv[2 * i + 1];
	}
    return 0;
}

int I420_2_NV12(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	uint8_t* src_y = src;
	uint8_t* src_u = src + width * height;
	uint8_t* src_v = src + width * height * 5 / 4;

	uint8_t* dst_y = dst;
	uint8_t* dst_uv = dst + width * height;
    memcpy(dst_y, src_y, width * height);
	for (int i = 0; i < width / 2 * height / 2; i++)
    {
        dst_uv[2*i] = src_u[i];
        dst_uv[2 * i+1] = src_v[i];
	}
    return 0;
}

int NV21_2_YV12(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	uint8_t* src_y = src;
	uint8_t* src_uv = src + width * height;

	uint8_t* dst_y = dst;
	uint8_t* dst_v = dst + width * height;
	uint8_t* dst_u = dst + width * height * 5 / 4;

	memcpy(dst_y, src_y, width * height);
	for (int i = 0; i < width / 2 * height / 2; i++)
	{
		dst_v[i] = src_uv[2 * i];
		dst_u[i] = src_uv[2 * i + 1];
	}
    return 0;
}

int YV12_2_NV21(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	uint8_t* src_y = src;
	uint8_t* src_v = src + width * height;
	uint8_t* src_u = src + width * height * 5 / 4;

	uint8_t* dst_y = dst;
	uint8_t* dst_uv = dst + width * height;
	memcpy(dst_y, src_y, width * height);
	for (int i = 0; i < width / 2 * height / 2; i++)
	{
		dst_uv[2 * i] = src_v[i];
		dst_uv[2 * i + 1] = src_u[i];
	}
	return 0;
}

int I420_2_rgb32(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	uint8_t* src_y = src;
	uint8_t* src_u = src + width * height;
	uint8_t* src_v = src + width * height * 5 / 4;

	RGB32* prgb = reinterpret_cast<RGB32*>(dst);
	int rgbindex = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			RGB32* rgbNode = &prgb[rgbindex];
			
			//读取YUV
			uint8_t Y = src_y[y * width + x];
			uint8_t U = src_u[y / 2 * width / 2 + x / 2];
			uint8_t V = src_v[y / 2 * width / 2 + x / 2];

			//计算rgb
			int tmpr = Y + 1.402 * (V - 128);
			int tmpg = Y - 0.34413 * (U - 128) - 0.71414 * (V - 128);
			int tmpb = Y + 1.772 * (U - 128);
			
			//越界保护
			rgbNode->rgbRed = tmpr > 255 ? 255 : (tmpr < 0 ? 0 : tmpr);
			rgbNode->rgbGreen =tmpg > 255 ? 255 : (tmpg < 0 ? 0 : tmpg);
			rgbNode->rgbBlue = tmpb > 255 ? 255 : (tmpb < 0 ? 0 : tmpb);

			rgbindex++;

		}
	}
	return 0;
}

int rgb32_2_I420(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	RGB32* prgb = reinterpret_cast<RGB32*>(src);
	uint8_t* dst_y = dst;
	uint8_t* dst_u = dst + width * height;
	uint8_t* dst_v = dst + width * height * 5 / 4;

	//Y = 0.298R + 0.612G + 0.117B;
	//U = -0.168R - 0.330G + 0.498B + 128;
	//V = 0.449R - 0.435G - 0.083B + 128;

	int rgbindex = 0;
	int u_index = 0;
	int v_index = 0;
	int y_index = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			rgbindex = y * width + x;
			RGB32* rgbNode = &prgb[rgbindex];
			//Y = 0.298R + 0.612G + 0.117B;
			int tmp_y = 0.298*rgbNode->rgbRed + 0.612 * rgbNode->rgbGreen + 0.117*rgbNode->rgbBlue;
			dst_y[y_index] = tmp_y > 255 ? 255 : (tmp_y < 0 ? 0 : tmp_y);
			y_index++;

			if (y%2 ==0 &&
				x%2 == 0)
			{
				//U = -0.168R - 0.330G + 0.498B + 128;
				int tmp_u = -0.168 * rgbNode->rgbRed - 0.330 * rgbNode->rgbGreen + 0.498 * rgbNode->rgbBlue + 128;
				//V = 0.449R - 0.435G - 0.083B + 128;
				int tmp_v = 0.449 * rgbNode->rgbRed - 0.435 * rgbNode->rgbGreen + 0.083 * rgbNode->rgbBlue + 128;

				dst_u[u_index] = tmp_u > 255 ? 255 : (tmp_u < 0 ? 0 : tmp_u);
				dst_v[v_index] = tmp_v > 255 ? 255 : (tmp_v < 0 ? 0 : tmp_v);
				v_index++;
				u_index++;
			}
		}
	}
	
	return 0;
}

int I420_2_rgb24(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	uint8_t* src_y = src;
	uint8_t* src_u = src + width * height;
	uint8_t* src_v = src + width * height * 5 / 4;

	RGB24* prgb = reinterpret_cast<RGB24*>(dst);
	int rgbindex = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			RGB24* rgbNode = &prgb[rgbindex];

			//读取YUV
			uint8_t Y = src_y[y * width + x];
			uint8_t U = src_u[y / 2 * width / 2 + x / 2];
			uint8_t V = src_v[y / 2 * width / 2 + x / 2];

			//计算rgb
			//R = Y + 1.4075(V - 128);
			//G = Y - 0.3455(U - 128) - 0.7169(V - 128);
			//B = Y + 1.779(U - 128);
			int tmpr = Y + 1.402 * (V - 128);
			int tmpg = Y - 0.34413 * (U - 128) - 0.71414 * (V - 128);
			int tmpb = Y + 1.772 * (U - 128);

			//越界保护
			rgbNode->rgbRed = tmpr > 255 ? 255 : (tmpr < 0 ? 0 : tmpr);
			rgbNode->rgbGreen = tmpg > 255 ? 255 : (tmpg < 0 ? 0 : tmpg);
			rgbNode->rgbBlue = tmpb > 255 ? 255 : (tmpb < 0 ? 0 : tmpb);

			rgbindex++;

		}
	}
	return 0;
}

int rgb24_2_I420(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height)
{
	RGB24* prgb = reinterpret_cast<RGB24*>(src);
	uint8_t* dst_y = dst;
	uint8_t* dst_u = dst + width * height;
	uint8_t* dst_v = dst + width * height * 5 / 4;

	//Y = 0.298R + 0.612G + 0.117B;
	//U = -0.168R - 0.330G + 0.498B + 128;
	//V = 0.449R - 0.435G - 0.083B + 128;

	int rgbindex = 0;
	int u_index = 0;
	int v_index = 0;
	int y_index = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			rgbindex = y * width + x;
			RGB24* rgbNode = &prgb[rgbindex];
			//Y = 0.298R + 0.612G + 0.117B;
			int tmp_y = 0.298 * rgbNode->rgbRed + 0.612 * rgbNode->rgbGreen + 0.117 * rgbNode->rgbBlue;
			dst_y[y_index] = tmp_y > 255 ? 255 : (tmp_y < 0 ? 0 : tmp_y);
			y_index++;

			if (y % 2 == 0 &&
				x % 2 == 0)
			{
				//U = -0.168R - 0.330G + 0.498B + 128;
				int tmp_u = -0.168 * rgbNode->rgbRed - 0.330 * rgbNode->rgbGreen + 0.498 * rgbNode->rgbBlue + 128;
				//V = 0.449R - 0.435G - 0.083B + 128;
				int tmp_v = 0.449 * rgbNode->rgbRed - 0.435 * rgbNode->rgbGreen + 0.083 * rgbNode->rgbBlue + 128;

				dst_u[u_index] = tmp_u > 255 ? 255 : (tmp_u < 0 ? 0 : tmp_u);
				dst_v[v_index] = tmp_v > 255 ? 255 : (tmp_v < 0 ? 0 : tmp_v);
				v_index++;
				u_index++;
			}
		}
	}

	return 0;
}