#pragma once
#include"gdal_priv.h"
#include"RSIDoc.h"
#include<opencv2\opencv.hpp>
#include "highgui/highgui.hpp"  
#include "opencv2/nonfree/nonfree.hpp"  
#include "opencv2/legacy/legacy.hpp"
class GISImage
{
public:
	GISImage();
	~GISImage();

	GDALDataset* Stretch_percent_16to8(const char* inFilename);//, const char* dstFilename


private:

};

