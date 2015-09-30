
#include "vcc.h"
#include "../LeastSquares\least_squares.h"



VCC::VCC(int thres)
{
	VCCCrrMp = new char[65536];
	this->surroundCorrelationData = new std::vector < int > ;


	char VCCCrrMpB[256]={0,1,1,1,1,2,2,2,1,2,2,2,1,2,2,2,1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,
                       1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,
                       1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
                       2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
                       1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
                       2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
                       1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
                       2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4};

	//相関テーブル(4画素2バイト用）の作成
  char* MpG=VCCCrrMp;
  char* MpA=VCCCrrMpB;
  for(short a=256;a;a--,MpA++)
  {
    char* MpB=VCCCrrMpB;
		for(short b=256;b;b--,MpB++,MpG++)
      *MpG=-(*MpA+*MpB);
  }

	threshold = thres;
	vc_template = NULL;
	vc_inputimage = NULL;
	vc_inputimage_assembly = NULL;
	vc_inputimage_assembly2 = NULL;
	vc_inputimage_assembly2_2 = NULL;

	subpixel_x = 0;
	subpixel_y = 0;

	x = VCC_SEEK_AREA_SIZE / 2 -VCC_TEMPLATE_HALF_SIZE;
	y = VCC_SEEK_AREA_SIZE / 2 -VCC_TEMPLATE_HALF_SIZE;

	correlationPoint = 2048;
	this->subpixelFlag = true;
}

VCC::~VCC()
{
	delete vc_template;
	delete vc_inputimage;
}


//テンプレートサイズは4の倍数＋２画素で行う
int VCC::VCC_create_template(unsigned char *image, const int x, const int y)
{
	register unsigned char *Inpt;
	register unsigned short *Tmp;
	register short a,b,c;
	unsigned short VC;
	short VCX,VCY;

	if(vc_template == NULL)
	{
		temp_size_x = (x-2)/4;
		temp_size_y = (y-2);
		vc_template = new unsigned short[temp_size_x*temp_size_y];
	}
	else //if(temp_size_x != (x-2)/4 || temp_size_y != (y-2))
	{
		delete vc_template;
		temp_size_x = (x-2)/4;
		temp_size_y = (y-2);
		vc_template = new unsigned short[temp_size_x*temp_size_y];
	}
	
	Inpt= image + x + 1; 
	Tmp = vc_template;
  
  for(a=temp_size_y;a;a--,Inpt+=2)
    for(b=temp_size_x;b;b--,Tmp++)
    {
      VC=0;
      for(c=4;c;c--,Inpt++)
      {
        VC<<=4;     
        VCX=*(Inpt+x+1)+*(Inpt+1)+*(Inpt-x+1)-*(Inpt+x-1)-*(Inpt-1)-*(Inpt-x-1);
        VCY=*(Inpt+x+1)+*(Inpt+x)+*(Inpt+x-1)-*(Inpt-x+1)-*(Inpt-x)-*(Inpt-x-1);
        if(VCX>this->threshold)VC+=1;else if(VCX<-this->threshold)VC+=2;
        if(VCY>this->threshold)VC+=4;else if(VCY<-this->threshold)VC+=8;
      }
      *Tmp=VC;
	}
	return 0;

}
int VCC::VCC_create_inputimage(unsigned char *image, const int x, const int y)
{

	register unsigned char* Inpt;
	register unsigned short* VcCd;
	short VCX, VCY;
	register short a, b;
	unsigned short VC;

	if (vc_inputimage == NULL)
	{
		inptimg_size_x = (x - 2);
		inptimg_size_y = (y - 2);
		vc_inputimage = new unsigned short[(inptimg_size_x - 3)*inptimg_size_y];
	}
	else
	{
		delete vc_inputimage;
		inptimg_size_x = (x - 2);
		inptimg_size_y = (y - 2);
		vc_inputimage = new unsigned short[(inptimg_size_x - 3)*inptimg_size_y];
	}
	//ここに，入力画像のサイズを変更する様なものが必要だよね？

	/*---------------------ベクトル符号化-------------------------------------------*/
	Inpt = image + x + 1;
	VcCd = vc_inputimage;
	for (a = inptimg_size_y; a; a--, Inpt += 2)
	{
		VC = 0;
		for (b = 3; b; b--, Inpt++)
		{
			VC <<= 4;
			VCX = *(Inpt + x + 1) + *(Inpt + 1) + *(Inpt - x + 1) - *(Inpt + x - 1) - *(Inpt - 1) - *(Inpt - x - 1);
			VCY = *(Inpt + x + 1) + *(Inpt + x) + *(Inpt + x - 1) - *(Inpt - x + 1) - *(Inpt - x) - *(Inpt - x - 1);
			if (VCX>this->threshold)VC += 1; else if (VCX<-this->threshold)VC += 2;
			if (VCY>this->threshold)VC += 4; else if (VCY<-this->threshold)VC += 8;
		}
		for (b = inptimg_size_x - 3; b; b--, Inpt++, VcCd++)
		{
			VC <<= 4;
			VCX = *(Inpt + x + 1) + *(Inpt + 1) + *(Inpt - x + 1) - *(Inpt + x - 1) - *(Inpt - 1) - *(Inpt - x - 1);
			VCY = *(Inpt + x + 1) + *(Inpt + x) + *(Inpt + x - 1) - *(Inpt - x + 1) - *(Inpt - x) - *(Inpt - x - 1);
			if (VCX>this->threshold)VC += 1; else if (VCX<-this->threshold)VC += 2;
			if (VCY>this->threshold)VC += 4; else if (VCY<-this->threshold)VC += 8;
			*VcCd = VC;
		}
	}
	//printf("\n\n");
	return 0;
}

cv::Mat VCC::VCC_imaging(unsigned short *vcc_image, int get_x, int get_y){

	cv::Mat vccImage = cv::Mat::zeros(get_y, get_x, CV_8UC3);
	unsigned char *im = vccImage.data;
	unsigned short *Tmp = vcc_image;
	for (int y = 0; y < get_y-2; y++){
		for (int x = 0; x < get_x; x += 2, Tmp += 2, im += 3){
			unsigned char t = *((unsigned char*)Tmp) & 0x0F;
			unsigned char tt = *((unsigned char*)Tmp) & 0xF0;
			//printf("%02x\n", tt);
			if (t == 1){
				*(im + 0) = 0x00;
				*(im + 1) = 0x00;
				*(im + 2) = 0xCC;
			}
			if (t == 5){
				*(im + 0) = 0x4C;
				*(im + 1) = 0xB7;
				*(im + 2) = 0xFF;
			}
			if (t == 4){
				*(im + 0) = 0x00;
				*(im + 1) = 0xFF;
				*(im + 2) = 0xFF;
			}
			if (t == 6){
				*(im + 0) = 0x7F;
				*(im + 1) = 0xFF;
				*(im + 2) = 0xAB;
			}
			if (t == 2){
				*(im + 0) = 0x41;
				*(im + 1) = 0xFF;
				*(im + 2) = 0x00;
			}
			if (t == 10){
				*(im + 0) = 0xAE;
				*(im + 1) = 0x99;
				*(im + 2) = 0x69;
			}
			if (t == 8){
				*(im + 0) = 0xcc;
				*(im + 1) = 0x00;
				*(im + 2) = 0x0c;
			}
			if (t == 9){
				*(im + 0) = 0xcc;
				*(im + 1) = 0x00;
				*(im + 2) = 0xc4;
			}
			im += 3;
			t = tt;
			if (t == 1){
				*(im + 0) = 0x00;
				*(im + 1) = 0x00;
				*(im + 2) = 0xCC;
			}
			if (t == 5){
				*(im + 0) = 0x4C;
				*(im + 1) = 0xB7;
				*(im + 2) = 0xFF;
			}
			if (t == 4){
				*(im + 0) = 0x00;
				*(im + 1) = 0xFF;
				*(im + 2) = 0xFF;
			}
			if (t == 6){
				*(im + 0) = 0x7F;
				*(im + 1) = 0xFF;
				*(im + 2) = 0xAB;
			}
			if (t == 2){
				*(im + 0) = 0x41;
				*(im + 1) = 0xFF;
				*(im + 2) = 0x00;
			}
			if (t == 10){
				*(im + 0) = 0xAE;
				*(im + 1) = 0x99;
				*(im + 2) = 0x69;
			}
			if (t == 8){
				*(im + 0) = 0xcc;
				*(im + 1) = 0x00;
				*(im + 2) = 0x0c;
			}
			if (t == 9){
				*(im + 0) = 0xcc;
				*(im + 1) = 0x00;
				*(im + 2) = 0xc4;
			}
		}
		//printf("\n");
	}
	//printf("%d\n", (int)(im - vccImage.data));
	return vccImage;
}


int VCC::VCC_matching(bool rollSeekFlag)
{

	unsigned short* McXY;
	register unsigned short* VcCd;
	unsigned short* McPs=vc_inputimage;
	short area = inptimg_size_x-temp_size_x*4+1;
	char* CrrMp=VCCCrrMp;
	short CrrMin=2048;
	short MP_margine = temp_size_x*4-4;
	short VC_margine = (inptimg_size_x-3)-temp_size_x*4;
	short MP_margine1 = inptimg_size_y-temp_size_y+1;
	short yy,xx,a,b;
	register unsigned short* Tmp;
	int CrrScr;

	float dx,dy;

	//short count = 0;
	bool local_mode = true;

//---------------------------元ソースコード------------------------------//
	if (!rollSeekFlag){
		for (yy = MP_margine1; yy; yy--, McPs += MP_margine)//temp_size_x*4-4;
		{
			for (xx = area; xx; xx--, McPs++)
			{
				Tmp = vc_template;
				CrrScr = CrrMin;
				VcCd = McPs;

				for (a = temp_size_y; a&&CrrScr > 0; a--, VcCd += VC_margine)//(inptimg_size_x-3)-temp_size_x*4;
					for (b = temp_size_x; b; b--, VcCd += 4, Tmp++)
						CrrScr += *(CrrMp + (*VcCd^*Tmp));
				if (CrrScr <= 0)continue;
				CrrMin -= CrrScr;
				McXY = McPs;
				x = (float)area - xx;
				y = (float)MP_margine1 - yy;
			}
		}
		correlationPoint = CrrMin;
	}
//--------------------------元ソースコードここまで---------------------//
//-------------------------渦巻き探索---------------------------------//
	else{
		int pos = (inptimg_size_x - 3)*(MP_margine1 / 2) + area / 2;
		int inc = 1;
		int k = -1;
		Tmp = vc_template;
		CrrScr = CrrMin;
		VcCd = McPs + pos;
		for (a = temp_size_y; a&&CrrScr > 0; a--, VcCd += VC_margine)//(inptimg_size_x-3)-temp_size_x*4;
			for (b = temp_size_x; b; b--, VcCd += 4, Tmp++)
				CrrScr += *(CrrMp + (*VcCd^*Tmp));
		CrrMin -= CrrScr;
		McXY = McPs + pos;
		x = pos % (inptimg_size_x - 3);
		y = (int)(pos / (inptimg_size_x - 3));
		while (inc < area-4){
			for (int xx = inc; xx; xx--){
				pos += k;
				Tmp = vc_template;
				CrrScr = CrrMin;
				VcCd = McPs + pos;
				for (a = temp_size_y; a&&CrrScr > 0; a--, VcCd += VC_margine)//(inptimg_size_x-3)-temp_size_x*4;
					for (b = temp_size_x; b; b--, VcCd += 4, Tmp++)
						CrrScr += *(CrrMp + (*VcCd^*Tmp));
				if (CrrScr <= 0)continue;
				CrrMin -= CrrScr;
				McXY = McPs + pos;
				x = pos % (inptimg_size_x - 3);
				y = (int)(pos / (inptimg_size_x - 3));
			}
			for (int yy = inc; yy; yy--){
				pos += k * (inptimg_size_x - 3);
				Tmp = vc_template;
				CrrScr = CrrMin;
				VcCd = McPs + pos;
				for (a = temp_size_y; a&&CrrScr > 0; a--, VcCd += VC_margine)//(inptimg_size_x-3)-temp_size_x*4;
					for (b = temp_size_x; b; b--, VcCd += 4, Tmp++)
						CrrScr += *(CrrMp + (*VcCd^*Tmp));
				if (CrrScr <= 0)continue;
				CrrMin -= CrrScr;
				McXY = McPs + pos;
				x = pos % (inptimg_size_x - 3);
				y = (int)(pos / (inptimg_size_x - 3));
			}
			k *= -1;
			inc++;
		}
		for (int xx = inc - 1; xx; xx--){
			pos += k;
			Tmp = vc_template;
			CrrScr = CrrMin;
			VcCd = McPs + pos;
			for (a = temp_size_y; a&&CrrScr > 0; a--, VcCd += VC_margine)//(inptimg_size_x-3)-temp_size_x*4;
				for (b = temp_size_x; b; b--, VcCd += 4, Tmp++)
					CrrScr += *(CrrMp + (*VcCd^*Tmp));
			if (CrrScr <= 0)continue;
			CrrMin -= CrrScr;
			McXY = McPs + pos;
			x = pos % (inptimg_size_x - 3);
			y = (int)(pos / (inptimg_size_x - 3));
		}
		correlationPoint = CrrMin;
	}

//-------------------------渦巻き探索ここまで--------------------------//



//-------------------------最小二乗近似-----------------------------
	this->subpixel_x = 0;
	this->subpixel_y = 0;
	int surroundPixel = 2;
	if (this->subpixelFlag && y < MP_margine1 - surroundPixel - 1 && y > surroundPixel +1){
		surroundCorrelationData->clear();

		for (int yyy = -1 * surroundPixel; yyy <= surroundPixel; yyy++){
			for (int xxx = -1 * surroundPixel; xxx <= surroundPixel; xxx++)
			{
				//X+1の相違度
				Tmp = vc_template;
				int CrrScr = 2048;
				VcCd = McXY + yyy*(inptimg_size_x - 3) + xxx; 			

				for (a = temp_size_y; a&&CrrScr > 0; a--, VcCd += VC_margine)
					for (b = temp_size_x; b; b--, VcCd += 4, Tmp++)
						CrrScr += *(CrrMp + (*VcCd^*Tmp));
				surroundCorrelationData->push_back(2048 - CrrScr);
			}
		}
		
		double next_x, next_xy, next_y, next_yx;

		LeastSquares lsx(4);
		for (int i = 0; i < surroundPixel * 2 + 1; i++){
			lsx.addElements(i - surroundPixel, (*surroundCorrelationData)[2 * (surroundPixel * 2 + 1) + i]);
			//std::cout << i - surroundPixel << "\t" << surroundCorrelationData[2 * (surroundPixel * 2 + 1) + i] << std::endl;
		}
		least_x = lsx.calucation();

		LeastSquares lsy(4);
		for (int i = 0; i < surroundPixel * 2 + 1; i++){
			lsy.addElements(i - surroundPixel, (*surroundCorrelationData)[i*(surroundPixel * 2 + 1) + surroundPixel]);
			//std::cout << i - surroundPixel << "\t" << surroundCorrelationData[i*(surroundPixel * 2 + 1) + surroundPixel] << std::endl;
		}
		least_y = lsy.calucation();


		LeastSquares lsx_next(4);
		if (least_y < 0){
			next_xy = -1;
		}
		else {
			next_xy = 1;
		}
		for (int i = 0; i < surroundPixel * 2 + 1; i++){
			lsx_next.addElements(i - surroundPixel, (*surroundCorrelationData)[(2 + next_xy) * (surroundPixel * 2 + 1) + i]);
		}
		next_x = lsx_next.calucation();

		LeastSquares lsy_next(4);
		if (least_x < 0){
			next_yx = -1;
		}
		else {
			next_yx = 1;
		}
		for (int i = 0; i < surroundPixel * 2 + 1; i++){
			lsy_next.addElements(i - surroundPixel, (*surroundCorrelationData)[i*(surroundPixel * 2 + 1) + surroundPixel + next_yx]);
		}
		next_y = lsy_next.calucation();

		double s1 = ((next_x - least_x)*(least_y - 0) - (next_xy - 0)*(0 - least_x)) / 2;
		double s2 = ((next_x - least_x)*(0 - next_y) - (next_xy - 0)*(least_x - next_yx)) / 2;

		this->subpixel_x = 0 + (next_yx - 0)*s1 / (s1 + s2);
		this->subpixel_y = least_y + (next_yx - 0)*s1 / (s1 + s2);
	}
	else{
	}

//-------------------------最小二乗近似-----------------------------

	return 0;
}




union shortlong{
	unsigned short s[2];
	unsigned long l;
};

//int VCC::VCC_create_inputimage_assembly(unsigned char *image, const int x, const int y)
//{
//
//	register unsigned char* Inpt;
//	register unsigned short* VcCd;
//	short VCX, VCY;
//	register short a, b;
//	unsigned short VC;
//
//	if (vc_inputimage_assembly == NULL)
//	{
//		inptimg_size_x = (x - 2);
//		inptimg_size_y = (y - 2);
//		vc_inputimage_assembly = new unsigned short[(inptimg_size_x - 3)*inptimg_size_y];
//	}
//	else
//	{
//		delete vc_inputimage;
//		inptimg_size_x = (x - 2);
//		inptimg_size_y = (y - 2);
//		vc_inputimage_assembly = new unsigned short[(inptimg_size_x - 3)*inptimg_size_y];
//	}
//	//ここに，入力画像のサイズを変更する様なものが必要だよね？
//
//	/*---------------------ベクトル符号化-------------------------------------------*/
//	Inpt = image + x + 1;
//	VcCd = vc_inputimage_assembly;
//	for (a = inptimg_size_y; a; a--, Inpt += 2)
//	{
//		VC = 0;
//		for (b = 3; b; b--, Inpt++)
//		{
//			VC <<= 4;
//			VCX = *(Inpt + x + 1) + *(Inpt + 1) + *(Inpt - x + 1) - *(Inpt + x - 1) - *(Inpt - 1) - *(Inpt - x - 1);
//			VCY = *(Inpt + x + 1) + *(Inpt + x) + *(Inpt + x - 1) - *(Inpt - x + 1) - *(Inpt - x) - *(Inpt - x - 1);
//			if (VCX>this->threshold)VC += 1; else if (VCX<-this->threshold)VC += 2;
//			if (VCY>this->threshold)VC += 4; else if (VCY<-this->threshold)VC += 8;
//		}
//		for (b = inptimg_size_x - 3; b; b--, Inpt++, VcCd++)
//		{
//			VC <<= 4;
//			VCX = *(Inpt + x + 1) + *(Inpt + 1) + *(Inpt - x + 1) - *(Inpt + x - 1) - *(Inpt - 1) - *(Inpt - x - 1);
//			VCY = *(Inpt + x + 1) + *(Inpt + x) + *(Inpt + x - 1) - *(Inpt - x + 1) - *(Inpt - x) - *(Inpt - x - 1);
//			if (VCX>this->threshold)VC += 1; else if (VCX<-this->threshold)VC += 2;
//			if (VCY>this->threshold)VC += 4; else if (VCY<-this->threshold)VC += 8;
//			*VcCd = VC;
//		}
//	}
//
//	return 0;
//}
//
//int VCC::VCC_create_inputimage_assembly2(unsigned char *image, const int x, const int y)
//{
//
//	register unsigned char* Inpt;
//	register unsigned char* VcCd;
//	short VCX, VCY;
//	register short a, b;
//	unsigned char VC;
//
//	if (vc_inputimage_assembly2 == NULL)
//	{
//		inptimg_size_x = (x - 2);
//		inptimg_size_y = (y - 2);
//		vc_inputimage_assembly2 = new unsigned char[(inptimg_size_x / 2)*inptimg_size_y];
//	}
//	else
//	{
//		delete vc_inputimage_assembly2;
//		inptimg_size_x = (x - 2);
//		inptimg_size_y = (y - 2);
//		vc_inputimage_assembly2 = new unsigned char[(inptimg_size_x / 2)*inptimg_size_y];
//	}
//	if (vc_inputimage_assembly2_2 == NULL)
//	{
//		inptimg_size_x = (x - 2);
//		inptimg_size_y = (y - 2);
//		vc_inputimage_assembly2_2 = new unsigned char[(inptimg_size_x / 2 - 1)*inptimg_size_y];
//	}
//	else
//	{
//		delete vc_inputimage_assembly2_2;
//		inptimg_size_x = (x - 2);
//		inptimg_size_y = (y - 2);
//		vc_inputimage_assembly2_2 = new unsigned char[(inptimg_size_x / 2 - 1)*inptimg_size_y];
//	}
//	//ここに，入力画像のサイズを変更する様なものが必要だよね？
//
//	/*---------------------ベクトル符号化-------------------------------------------*/
//	Inpt = image + x + 1;
//	VcCd = vc_inputimage_assembly2;
//	for (a = inptimg_size_y; a; a--, Inpt += 2)
//	{
//		VC = 0;
//		for (b = inptimg_size_x / 2; b; b--)
//		{
//			VC <<= 4;
//			VCX = *(Inpt + x + 1) + *(Inpt + 1) + *(Inpt - x + 1) - *(Inpt + x - 1) - *(Inpt - 1) - *(Inpt - x - 1);
//			VCY = *(Inpt + x + 1) + *(Inpt + x) + *(Inpt + x - 1) - *(Inpt - x + 1) - *(Inpt - x) - *(Inpt - x - 1);
//			if (VCX>this->threshold)VC += 1; else if (VCX<-this->threshold)VC += 2;
//			if (VCY>this->threshold)VC += 4; else if (VCY<-this->threshold)VC += 8;
//
//			Inpt++;
//			VC <<= 4;
//			VCX = *(Inpt + x + 1) + *(Inpt + 1) + *(Inpt - x + 1) - *(Inpt + x - 1) - *(Inpt - 1) - *(Inpt - x - 1);
//			VCY = *(Inpt + x + 1) + *(Inpt + x) + *(Inpt + x - 1) - *(Inpt - x + 1) - *(Inpt - x) - *(Inpt - x - 1);
//			if (VCX>this->threshold)VC += 1; else if (VCX<-this->threshold)VC += 2;
//			if (VCY>this->threshold)VC += 4; else if (VCY<-this->threshold)VC += 8;
//			*VcCd = VC;
//			Inpt++;
//			VcCd++;
//		}
//	}
//	/*---------------------ベクトル符号化2-------------------------------------------*/
//	Inpt = image + x + 2;
//	VcCd = vc_inputimage_assembly2_2;
//	for (a = inptimg_size_y; a; a--, Inpt += 4)
//	{
//		VC = 0;
//		for (b = inptimg_size_x / 2 - 1; b; b--)
//		{
//			VC <<= 4;
//			VCX = *(Inpt + x + 1) + *(Inpt + 1) + *(Inpt - x + 1) - *(Inpt + x - 1) - *(Inpt - 1) - *(Inpt - x - 1);
//			VCY = *(Inpt + x + 1) + *(Inpt + x) + *(Inpt + x - 1) - *(Inpt - x + 1) - *(Inpt - x) - *(Inpt - x - 1);
//			if (VCX>this->threshold)VC += 1; else if (VCX<-this->threshold)VC += 2;
//			if (VCY>this->threshold)VC += 4; else if (VCY<-this->threshold)VC += 8;
//
//			Inpt++;
//			VC <<= 4;
//			VCX = *(Inpt + x + 1) + *(Inpt + 1) + *(Inpt - x + 1) - *(Inpt + x - 1) - *(Inpt - 1) - *(Inpt - x - 1);
//			VCY = *(Inpt + x + 1) + *(Inpt + x) + *(Inpt + x - 1) - *(Inpt - x + 1) - *(Inpt - x) - *(Inpt - x - 1);
//			if (VCX>this->threshold)VC += 1; else if (VCX<-this->threshold)VC += 2;
//			if (VCY>this->threshold)VC += 4; else if (VCY<-this->threshold)VC += 8;
//			*VcCd = VC;
//			Inpt++;
//			VcCd++;
//		}
//	}
//
//	return 0;
//}
//int VCC::VCC_matching_assembly(float *x, float *y, int *crr)
//{
//
//	unsigned short* McXY;
//	register unsigned short* VcCd;
//	unsigned short* McPs = vc_inputimage;
//	short area = inptimg_size_x - temp_size_x * 4 + 1;
//	char* CrrMp = VCCCrrMp;
//	short CrrMin = 2048;
//	short MP_margine = temp_size_x * 4 - 4;
//	short VC_margine = (inptimg_size_x - 3) - temp_size_x * 4;
//	short MP_margine1 = inptimg_size_y - temp_size_y + 1;
//	short yy, xx, a, b;
//	register unsigned short* Tmp;
//	int CrrScr;
//
//	float dx, dy;
//
//
//	unsigned long result[4];
//	result[0] = 0;
//	result[1] = 0;
//	result[2] = 0;
//	result[3] = 0;
//
//	unsigned long mask1[4];
//	mask1[0] = 0x11111111;
//	mask1[1] = 0x11111111;
//	mask1[2] = 0x11111111;
//	mask1[3] = 0x11111111;
//
//	unsigned long mask2[4];
//	mask2[0] = 0x22222222;
//	mask2[1] = 0x22222222;
//	mask2[2] = 0x22222222;
//	mask2[3] = 0x22222222;
//
//	unsigned long mask3[4];
//	mask3[0] = 0x44444444;
//	mask3[1] = 0x44444444;
//	mask3[2] = 0x44444444;
//	mask3[3] = 0x44444444;
//
//	unsigned long mask4[4];
//	mask4[0] = 0x88888888;
//	mask4[1] = 0x88888888;
//	mask4[2] = 0x88888888;
//	mask4[3] = 0x88888888;
//
//	unsigned long mask5[4];
//	mask5[0] = 0x0000000F;
//	mask5[1] = 0x0000000F;
//	mask5[2] = 0x0000000F;
//	mask5[3] = 0x0000000F;
//
//
//	unsigned short sum;
//	int  tmp;
//	unsigned long buff[200];
//	unsigned long *albuff = buff;
//	int offset = 0;
//
//	while ((unsigned int)albuff % 16 != 0){
//		albuff++;
//		offset++;
//	}
//
//	for (int n = 0; n < 132; n++)
//	{
//		buff[n + offset] = 0;
//	}
//
//	sum = 4 * 32 * 32;
//
//	unsigned long * p1, *p2, *pb, i;
//	register unsigned short * head1, *head2;
//	head1 = (unsigned short *)vc_inputimage_assembly;
//	head2 = (unsigned short *)vc_template;
//
//	for (unsigned short v = 0; v < MP_margine1; v += 1, head1 += MP_margine)
//	{
//		for (unsigned short u = 0; u < area; u += 1, head1 += 1)
//		{
//			p1 = (unsigned long *)head1;
//			p2 = (unsigned long *)head2;
//			unsigned short * p1_s = head1;
//			register unsigned short sa[8];
//
//			pb = albuff;
//
//			for (int y = 0; y < 32; p1_s += inptimg_size_x - 3, p1 += VC_margine / 2, p2 += 4, pb += 4, y++)
//			{
//				for (int i = 0; i < 8; i++)
//					sa[i] = *(p1_s + i * 4);
//				p1 = (unsigned long *)sa;
//				//shortlong sl;
//				//sl.s[0] = *p1_s;
//				//sl.s[1] = *(p1_s + 4);
//				//p1 = &sl.l;
//				//p1 = (unsigned long *)p1_s;
//				__asm{
//					mov       eax, p1
//						movdqu    xmm0, [eax]
//						mov       eax, p2
//						movdqu    xmm1, [eax]
//						pxor      xmm0, xmm1
//						mov       eax, pb
//						movdqa[eax], xmm0
//				}
//			}
//
//			tmp = sseCount(albuff, result, mask1, mask2, mask3, mask4, mask5);
//
//			if (tmp < sum)
//			{
//				*x = u;
//				*y = v;
//				sum = tmp;
//			}
//		}
//	}
//
//	//short count = 0;
//
//	//for (yy = MP_margine1; yy; yy--, McPs += MP_margine)
//	//	for (xx = area; xx; xx--, McPs++)
//	//	{
//	//	Tmp = vc_template;
//	//	CrrScr = CrrMin;
//	//	VcCd = McPs;
//
//	//	for (a = temp_size_y; a&&CrrScr>0; a--, VcCd += VC_margine)
//	//		for (b = temp_size_x; b; b--, VcCd += 4, Tmp++)
//	//			CrrScr += *(CrrMp + (*VcCd^*Tmp));
//	//	if (CrrScr <= 0)continue;
//	//	CrrMin -= CrrScr;
//	//	McXY = McPs;
//	//	*x = (float)area - xx;
//	//	*y = (float)MP_margine1 - yy;
//	//	}
//	*crr = sum;
//	return 0;
//}
//
//int VCC::VCC_matching_assembly2(float *x, float *y, int *crr)
//{
//
//	unsigned short* McXY;
//	register unsigned short* VcCd;
//	unsigned char* McPs = vc_inputimage_assembly2;
//	short area = inptimg_size_x - temp_size_x * 4 + 1;
//	char* CrrMp = VCCCrrMp;
//	short CrrMin = 2048;
//	short MP_margine = (temp_size_x * 4 - 4)/4;
//	short VC_margine = (inptimg_size_x/2) - temp_size_x * 4/2;
//	short MP_margine1 = inptimg_size_y - temp_size_y + 1;
//	short yy, xx, a, b;
//	register unsigned short* Tmp;
//	int CrrScr;
//
//	float dx, dy;
//
//
//	unsigned long result[4];
//	result[0] = 0;
//	result[1] = 0;
//	result[2] = 0;
//	result[3] = 0;
//
//	unsigned long mask1[4];
//	mask1[0] = 0x11111111;
//	mask1[1] = 0x11111111;
//	mask1[2] = 0x11111111;
//	mask1[3] = 0x11111111;
//
//	unsigned long mask2[4];
//	mask2[0] = 0x22222222;
//	mask2[1] = 0x22222222;
//	mask2[2] = 0x22222222;
//	mask2[3] = 0x22222222;
//
//	unsigned long mask3[4];
//	mask3[0] = 0x44444444;
//	mask3[1] = 0x44444444;
//	mask3[2] = 0x44444444;
//	mask3[3] = 0x44444444;
//
//	unsigned long mask4[4];
//	mask4[0] = 0x88888888;
//	mask4[1] = 0x88888888;
//	mask4[2] = 0x88888888;
//	mask4[3] = 0x88888888;
//
//	unsigned long mask5[4];
//	mask5[0] = 0x0000000F;
//	mask5[1] = 0x0000000F;
//	mask5[2] = 0x0000000F;
//	mask5[3] = 0x0000000F;
//
//
//	unsigned short sum;
//	int  tmp;
//	unsigned long buff[200];
//	unsigned long *albuff = buff;
//	int offset = 0;
//
//	while ((unsigned int)albuff % 16 != 0){
//		albuff++;
//		offset++;
//	}
//
//	for (int n = 0; n < 132; n++)
//	{
//		buff[n + offset] = 0;
//	}
//
//	sum = 4 * 32 * 32;
//
//	unsigned long * p1, *p2, *pb, i;
//	unsigned char * p3;
//	register unsigned char *head1;
//	register unsigned short *head2;
//	head1 = vc_inputimage_assembly2;
//	head2 = vc_template;
//
//	for (unsigned short v = 0; v < MP_margine1; v += 1, head1 += 15)
//	{
//		for (unsigned short u = 0; u < area; u += 2, head1 += 1)
//		{
//			//p1 = (unsigned long *)head1;
//			p2 = (unsigned long *)head2;
//			p3 = head1;
//
//			pb = albuff;
//
//			for (int y = 0; y < 32; p3 += inptimg_size_x / 2, p2 += 4, pb += 4, y++)
//			{
//				p1 = (unsigned long *)p3;
//				__asm{
//					mov       eax, p3
//						movdqu    xmm0, [eax]
//						mov       eax, p2
//						movdqu    xmm1, [eax]
//						pxor      xmm0, xmm1
//						mov       eax, pb
//						movdqa[eax], xmm0
//				}
//			}
//
//			tmp = sseCount(albuff, result, mask1, mask2, mask3, mask4, mask5);
//
//			if (tmp < sum)
//			{
//				*x = u;
//				*y = v;
//				sum = tmp;
//			}
//		}
//		//printf("%d\n", v);
//	}
//
//	//short count = 0;
//
//	//for (yy = MP_margine1; yy; yy--, McPs += MP_margine)
//	//	for (xx = area; xx; xx--, McPs++)
//	//	{
//	//	Tmp = vc_template;
//	//	CrrScr = CrrMin;
//	//	VcCd = McPs;
//
//	//	for (a = temp_size_y; a&&CrrScr>0; a--, VcCd += VC_margine)
//	//		for (b = temp_size_x; b; b--, VcCd += 4, Tmp++)
//	//			CrrScr += *(CrrMp + (*VcCd^*Tmp));
//	//	if (CrrScr <= 0)continue;
//	//	CrrMin -= CrrScr;
//	//	McXY = McPs;
//	//	*x = (float)area - xx;
//	//	*y = (float)MP_margine1 - yy;
//	//	}
//	*crr = sum;
//	return 0;
//}
//
//unsigned long VCC::sseCount(unsigned long * buff, unsigned long * pr, unsigned long * pm1, unsigned long * pm2, unsigned long * pm3, unsigned long * pm4, unsigned long * pm5){
//
//	__asm{
//		mov     eax, buff        // p = buff
//
//		// #1 --------------------------------
//
//		mov     ebx, pm1
//		movdqu  xmm7, [ebx]      // mask1
//
//		movdqa  xmm1, [eax]      // a =  p[0] & mask1
//		pand    xmm1, xmm7
//
//		movdqa  xmm2, [eax + 16]   // a += p[1] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 32]   // a += p[2] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 48]   // a += p[3] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 64]   // a += p[4] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 80]   // a += p[5] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 96]   // a += p[6] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 112]  // a += p[7] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 128]   // a += p[8] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 144]   // a += p[9] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 160]   // a += p[10] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 176]   // a += p[11] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 192]   // a += p[12] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 208]   // a += p[13] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 224]   // a += p[14] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		// #2 --------------------------------
//
//		mov     ebx, pm2
//		movdqu  xmm7, [ebx]      // mask2
//
//		movdqa  xmm2, [eax]      // b =  (p[0] & mask2) >> 1
//		pand    xmm2, xmm7
//		psrld   xmm2, 1
//
//		movdqa  xmm3, [eax + 16]   // b +=  (p[1] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 32]   // b +=  (p[2] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld  xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 48]   // b +=  (p[3] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 64]   // b +=  (p[4] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 80]   // b +=  (p[5] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 96]   // b +=  (p[6] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 112]  // b +=  (p[7] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 128]   // b +=  (p[8] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 144]   // b +=  (p[9] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 160]   // b +=  (p[10] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 176]   // b +=  (p[11] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 192]   // b +=  (p[12] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 208]   // b +=  (p[13] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 224]   // b +=  (p[14] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		// #3 --------------------------------
//
//		mov     ebx, pm3
//		movdqu  xmm7, [ebx]      // mask3
//
//		movdqa  xmm3, [eax]      // c =  (p[0] & mask3) >> 2
//		pand    xmm3, xmm7
//		psrld   xmm3, 2
//
//
//		movdqa  xmm4, [eax + 16]   // c += (p[1] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 32]   // c += (p[2] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 48]   // c += (p[3] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 64]   // c += (p[4] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 80]   // c += (p[5] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 96]   // c += (p[6] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 112]  // c += (p[7] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 128]   // c += (p[8] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 144]   // c += (p[9] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 160]   // c += (p[10] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 176]   // c += (p[11] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 192]   // c += (p[12] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 208]   // c += (p[13] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 224]   // c += (p[14] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		// #4 --------------------------------
//
//		mov     ebx, pm4
//		movdqu  xmm7, [ebx]      // mask4
//
//		movdqa  xmm4, [eax]      // d =  (p[0] & mask4) >> 3
//		pand    xmm4, xmm7
//		psrld   xmm4, 3
//
//
//		movdqa  xmm5, [eax + 16]   // d += (p[1] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 32]   // d += (p[2] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 48]   // d += (p[3] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 64]   // d += (p[4] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 80]   // d += (p[5] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 96]   // d += (p[6] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 112]  // d += (p[7] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 128]   // d += (p[8] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 144]   // d += (p[9] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 160]   // d += (p[10] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 176]   // d += (p[11] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 192]   // d += (p[12] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 208]   // d += (p[13] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 224]   // d += (p[14] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//
//		// 集計 --------------------------------
//		mov     ebx, pm5
//		movdqu  xmm7, [ebx]      // mask5
//
//
//		// #a ----------------------------
//
//		movdqa  xmm0, xmm7       // tmp = a & mask5
//		pand    xmm0, xmm1
//
//
//
//		psrld   xmm1, 4          // tmp += (a >> 4) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 8) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 12) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 16) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 20) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 24) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 28) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		// #b ----------------------------
//
//		movdqa  xmm5, xmm7       // tmp += b & mask5
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 4) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 8) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 12) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 16) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 20) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 24) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 28) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//
//		// #c ----------------------------
//
//		movdqa  xmm5, xmm7       // tmp += c & mask5
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 4) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 8) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 12) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 16) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 20) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 24) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 28) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		// #d ----------------------------
//
//		movdqa  xmm5, xmm7       // tmp += d & mask5
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 4) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 8) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 12) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 16) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 20) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 24) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 28) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//
//		// #1 --------------------------------
//
//		mov     ebx, pm1
//		movdqu  xmm7, [ebx]      // mask1
//
//		movdqa  xmm1, [eax + 240]   // a =  p[15] & mask1
//		pand    xmm1, xmm7
//
//		movdqa  xmm2, [eax + 256]   // a += p[16] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 272]   // a += p[17] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 288]   // a += p[18] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 304]   // a += p[19] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 320]   // a += p[20] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 336]   // a += p[21] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 352]  // a += p[22] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 368]   // a += p[23] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 384]   // a += p[24] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 400]   // a += p[25] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 416]   // a += p[26] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 432]   // a += p[27] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 448]   // a += p[28] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 464]   // a += p[29] & mask1
//		pand    xmm2, xmm7
//		paddw   xmm1, xmm2
//
//		// #2 --------------------------------
//
//		mov     ebx, pm2
//		movdqu  xmm7, [ebx]      // mask2
//
//		movdqa  xmm2, [eax]      // b =  (p[15] & mask2) >> 1
//		pand    xmm2, xmm7
//		psrld   xmm2, 1
//
//		movdqa  xmm3, [eax + 256]   // b +=  (p[16] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 272]   // b +=  (p[17] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld  xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 288]   // b +=  (p[18] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 304]   // b +=  (p[19] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 320]   // b +=  (p[20] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 336]   // b +=  (p[21] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 352]  // b +=  (p[22] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 368]   // b +=  (p[23] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 384]   // b +=  (p[24] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 400]   // b +=  (p[25] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 416]   // b +=  (p[26] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 432]   // b +=  (p[27] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 448]   // b +=  (p[28] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		movdqa  xmm3, [eax + 464]   // b +=  (p[29] & mask2) >> 1
//		pand    xmm3, xmm7
//		psrld   xmm3, 1
//		paddw   xmm2, xmm3
//
//		// #3 --------------------------------
//
//		mov     ebx, pm3
//		movdqu  xmm7, [ebx]      // mask3
//
//		movdqa  xmm3, [eax]      // c =  (p[15] & mask3) >> 2
//		pand    xmm3, xmm7
//		psrld   xmm3, 2
//
//
//		movdqa  xmm4, [eax + 256]   // c += (p[16] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 272]   // c += (p[17] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 288]   // c += (p[18] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 304]   // c += (p[19] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 320]   // c += (p[20] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 336]   // c += (p[21] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 352]  // c += (p[22] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 368]   // c += (p[23] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 384]   // c += (p[24] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 400]   // c += (p[25] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 416]   // c += (p[26] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 432]   // c += (p[27] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 448]   // c += (p[28] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		movdqa  xmm4, [eax + 464]   // c += (p[29] & mask3) >> 2
//		pand    xmm4, xmm7
//		psrld   xmm4, 2
//		paddw   xmm3, xmm4
//
//		// #4 --------------------------------
//
//		mov     ebx, pm4
//		movdqu  xmm7, [ebx]      // mask4
//
//		movdqa  xmm4, [eax]      // d =  (p[15] & mask4) >> 3
//		pand    xmm4, xmm7
//		psrld   xmm4, 3
//
//
//		movdqa  xmm5, [eax + 256]   // d += (p[16] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 272]   // d += (p[17] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 288]   // d += (p[18] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 304]   // d += (p[19] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 320]   // d += (p[20] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 336]   // d += (p[21] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 352]  // d += (p[22] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 368]   // d += (p[23] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 384]   // d += (p[24] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 400]   // d += (p[25] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 416]   // d += (p[26] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 432]   // d += (p[27] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 448]   // d += (p[28] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		movdqa  xmm5, [eax + 464]   // d += (p[29] & mask4) >> 3
//		pand    xmm5, xmm7
//		psrld   xmm5, 3
//		paddw   xmm4, xmm5
//
//		// 集計 --------------------------------
//		mov     ebx, pm5
//		movdqu  xmm7, [ebx]      // mask5
//
//		// #a ----------------------------
//
//		movdqa  xmm5, xmm7       // tmp += a & mask5
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 4) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 8) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 12) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 16) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 20) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 24) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 28) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		// #b ----------------------------
//
//		movdqa  xmm5, xmm7       // tmp += b & mask5
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 4) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 8) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 12) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 16) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 20) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 24) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		psrld   xmm2, 4          // tmp += (b >> 28) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm2
//		paddw   xmm0, xmm5
//
//		// #c ----------------------------
//
//		movdqa  xmm5, xmm7       // tmp += c & mask5
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 4) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 8) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 12) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 16) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 20) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 24) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		psrld   xmm3, 4          // tmp += (c >> 28) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm3
//		paddw   xmm0, xmm5
//
//		// #d ----------------------------
//
//		movdqa  xmm5, xmm7       // tmp += d & mask5
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 4) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 8) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 12) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 16) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 20) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 24) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		psrld   xmm4, 4          // tmp += (d >> 28) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm4
//		paddw   xmm0, xmm5
//
//		// ----------------
//
//		// #1 --------------------------------
//
//		mov     ebx, pm1
//		movdqu  xmm3, [ebx]      // mask1
//		mov     ebx, pm2
//		movdqu  xmm4, [ebx]      // mask2
//		mov     ebx, pm3
//		movdqu  xmm5, [ebx]      // mask3
//		mov     ebx, pm4
//		movdqu  xmm6, [ebx]      // mask4
//		mov     ebx, pm5
//		movdqu  xmm7, [ebx]      // mask5
//
//		movdqa  xmm1, [eax + 480]   // a =  p[30] & mask1
//		pand    xmm1, xmm3
//
//		movdqa  xmm2, [eax + 480]   // a += p[30] & mask2
//		pand    xmm2, xmm4
//		psrld   xmm2, 1
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 480]   // a += p[30] & mask3
//		pand    xmm2, xmm5
//		psrld   xmm2, 2
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 480]   // a += p[30] & mask4
//		pand    xmm2, xmm6
//		psrld   xmm2, 3
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 496]   // a += p[31] & mask1
//		pand    xmm2, xmm3
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 496]   // a += p[31] & mask2
//		pand    xmm2, xmm4
//		psrld   xmm2, 1
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 496]   // a += p[31] & mask3
//		pand    xmm2, xmm5
//		psrld   xmm2, 2
//		paddw   xmm1, xmm2
//
//		movdqa  xmm2, [eax + 496]   // a += p[31] & mask4
//		pand    xmm2, xmm6
//		psrld   xmm2, 3
//		paddw   xmm1, xmm2
//
//		// 集計 --------------------------------
//
//		// #a ----------------------------
//
//		movdqa  xmm5, xmm7       // tmp += a & mask5
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 4) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 8) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 12) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 16) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 20) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 24) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		psrld   xmm1, 4          // tmp += (a >> 28) & mask5
//		movdqa  xmm5, xmm7
//		pand    xmm5, xmm1
//		paddw   xmm0, xmm5
//
//		mov     edx, pr
//		movdqu[edx], xmm0
//	}
//	return (pr[0] + pr[1] + pr[2] + pr[3]);
//}