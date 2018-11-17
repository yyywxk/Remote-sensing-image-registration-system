#include "stdafx.h"
#include"CGISImage.h"


GISImage::GISImage()
{
}

GISImage::~GISImage()
{
}

GDALDataset* GISImage::Stretch_percent_16to8(const char* inFilename)//, const char* dstFilename
{
	GDALAllRegister();
	//Ϊ��֧������·��
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	int src_height = 0;
	int src_width = 0;
	GDALDataset *poIn = (GDALDataset *)GDALOpen(inFilename, GA_ReadOnly);   //��Ӱ��
																			//��ȡӰ���С
	src_width = poIn->GetRasterXSize();
	src_height = poIn->GetRasterYSize();
	//��ȡӰ�񲨶���
	int InBands = poIn->GetRasterCount();
	//��ȡӰ���ʽ
	GDALDataType eDataType = poIn->GetRasterBand(1)->GetRasterDataType();
	/*
	//����洢Ӱ��Ŀռ�ο�����
	double adfInGeoTransform[6] = { 0 };
	const char *pszWKT = NULL;
	//��ȡӰ��ռ�ο�
	poIn->GetGeoTransform(adfInGeoTransform);
	pszWKT = poIn->GetProjectionRef();
	//�����ļ�
	GDALDriver *poDriver=GetGDALDriverManager()->GetDriverByName("tif");
	if (!poDriver)
	{
		GDALClose(poIn);
		return FALSE;
	}

	//GDALDriver *poDriver = (GDALDriver *)GDALGetDriverByName("GTiff");
	GDALDataset *poOutputDS;
	poOutputDS = poDriver->Create(dstFilename, src_width, src_height, InBands, GDT_Byte, NULL);

	//���������ͼ��Ŀռ�ο��Լ���������
	poOutputDS->SetGeoTransform(adfInGeoTransform);
	poOutputDS->SetProjection(pszWKT);
	*/
	//��ȡӰ��

	//std::cout << "16λӰ�񽵵�8λӰ����..." << std::endl;
	for (int iBand = 0; iBand < InBands; iBand++)
	{
		//cout << "���ڴ���� " << iBand + 1 << " ����" << endl;
		//��ȡӰ��
		uint16_t *srcData = (uint16_t *)malloc(sizeof(uint16_t) *src_width * src_height * 1);
		memset(srcData, 0, sizeof(uint16_t) * 1 * src_width * src_height);
		int src_max = 0, src_min = 65500;
		//��ȡ�����Ӱ�񵽻���
		poIn->GetRasterBand(iBand + 1)->RasterIO(GF_Read, 0, 0, src_width, src_height, srcData + 0 * src_width * src_height, src_width, src_height, GDT_UInt16, 0, 0);
		//}
		//ͳ�����ֵ
		for (int src_row = 0; src_row < src_height; src_row++)
		{
			for (int src_col = 0; src_col < src_width; src_col++)
			{
				uint16_t src_temVal = *(srcData + src_row * src_width + src_col);
				if (src_temVal > src_max)
					src_max = src_temVal;
				if (src_temVal < src_min)
					src_min = src_temVal;
			}
		}

		double *numb_pix = (double *)malloc(sizeof(double)*(src_max + 1));      //������ֱֵ��ͼ����ÿ������ֵ�ĸ���
		memset(numb_pix, 0, sizeof(double) * (src_max + 1));
		//                 -------  ͳ������ֱֵ��ͼ  ------------         //

		for (int src_row = 0; src_row < src_height; src_row++)
		{
			for (int src_col = 0; src_col < src_width; src_col++)
			{
				uint16_t src_temVal = *(srcData + src_row * src_width + src_col);
				*(numb_pix + src_temVal) += 1;
			}
		}

		double *frequency_val = (double *)malloc(sizeof(double)*(src_max + 1));           //����ֵ���ֵ�Ƶ��
		memset(frequency_val, 0.0, sizeof(double)*(src_max + 1));

		for (int val_i = 0; val_i <= src_max; val_i++)
		{
			*(frequency_val + val_i) = *(numb_pix + val_i) / double(src_width * src_height);
		}

		double *accumlt_frequency_val = (double*)malloc(sizeof(double)*(src_max + 1));   //���س��ֵ��ۼ�Ƶ��
		memset(accumlt_frequency_val, 0.0, sizeof(double)*(src_max + 1));

		for (int val_i = 0; val_i <= src_max; val_i++)
		{
			for (int val_j = 0; val_j < val_i; val_j++)
			{
				*(accumlt_frequency_val + val_i) += *(frequency_val + val_j);
			}
		}
		//ͳ���������˽ض�ֵ
		int minVal = 0, maxVal = 0;
		for (int val_i = 1; val_i < src_max; val_i++)
		{
			double acc_fre_temVal0 = *(frequency_val + 0);
			double acc_fre_temVal = *(accumlt_frequency_val + val_i);
			if ((acc_fre_temVal - acc_fre_temVal0) > 0.0015)
			{
				minVal = val_i;
				break;
			}
		}
		for (int val_i = src_max - 1; val_i > 0; val_i--)
		{
			double acc_fre_temVal0 = *(accumlt_frequency_val + src_max);
			double acc_fre_temVal = *(accumlt_frequency_val + val_i);
			if (acc_fre_temVal < (acc_fre_temVal0 - 0.00012))
			{
				maxVal = val_i;
				break;
			}
		}

		for (int src_row = 0; src_row < src_height; src_row++)
		{
			uint8_t *dstData = (uint8_t*)malloc(sizeof(uint8_t)*src_width);
			memset(dstData, 0, sizeof(uint8_t)*src_width);
			for (int src_col = 0; src_col < src_width; src_col++)
			{
				uint16_t src_temVal = *(srcData + src_row * src_width + src_col);
				double stre_temVal = (src_temVal - minVal) / double(maxVal - minVal);
				if (src_temVal < minVal)
				{
					*(dstData + src_col) = (src_temVal)*(20.0 / double(minVal));
				}
				else if (src_temVal > maxVal)
				{
					stre_temVal = (src_temVal - src_min) / double(src_max - src_min);
					*(dstData + src_col) = 254;
				}
				else
					*(dstData + src_col) = pow(stre_temVal, 0.7) * 250;

			}
			//�Լ�
			poIn->GetRasterBand(iBand + 1)->RasterIO(GF_Write, 0, src_row, src_width, 1, dstData, src_width, 1, GDT_Byte, 0, 0);
			//poOutputDS->GetRasterBand(iBand + 1)->RasterIO(GF_Write, 0, src_row, src_width, 1, dstData, src_width, 1, GDT_Byte, 0, 0);
			free(dstData);
		}

		free(numb_pix);
		free(frequency_val);
		free(accumlt_frequency_val);
		free(srcData);

	}
	//GDALClose(poIn);
	//GDALClose(poOutputDS);

	return poIn;

}



