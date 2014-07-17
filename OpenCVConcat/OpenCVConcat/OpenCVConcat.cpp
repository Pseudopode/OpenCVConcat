//Copyright (c) 2014 Jean-Philippe Deblonde.
//Do what you want with the code. Cite me if you do anything useful
//with it.
//I'm not responsible for any problem/damaged done by using that code...

//change the following include lines to match your installation
#ifdef _DEBUG
#pragma comment(lib, "C:\\opencv2.4.9\\opencv\\build\\x64\\vc12\\lib\\opencv_highgui249d.lib")
#pragma comment(lib, "C:\\opencv2.4.9\\opencv\\build\\x64\\vc12\\lib\\opencv_imgproc249d.lib")
#pragma comment(lib, "C:\\opencv2.4.9\\opencv\\build\\x64\\vc12\\lib\\opencv_core249d.lib")
#else
#pragma comment(lib, "C:\\opencv2.4.9\\opencv\\build\\x64\\vc12\\lib\\opencv_highgui249.lib")
#pragma comment(lib, "C:\\opencv2.4.9\\opencv\\build\\x64\\vc12\\lib\\opencv_imgproc249.lib")
#pragma comment(lib, "C:\\opencv2.4.9\\opencv\\build\\x64\\vc12\\lib\\opencv_core249.lib")
#endif

#include "C:\OpenCV2.1\include\opencv\cv.h"
#include "C:\OpenCV2.1\include\opencv\cxcore.h"
#include "C:\OpenCV2.1\include\opencv\highgui.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "tinydir.h"
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace cv;
using namespace std;

Mat result_img;
vector<Mat > *img_to_concat;
int num_img_to_concat;

bool verbose = true;

void concat_img(vector<Mat > *img_to_concat, int num_of_concat_done){

	//for(int i = 0; i < num_img_to_concat; i++){
	Mat result,A,B;
	img_to_concat->at(0).copyTo(result);

	for(int i = 1; i < 5; i+=1){
		if(verbose == true)
			printf("Concatenating image...\n");
		//hconcat( img_to_concat->at(i),  img_to_concat->at(i-1), result);
		img_to_concat->at(i).copyTo(B);
		result.copyTo(A);
		/*imshow("A",A);
		imshow("B",B);
		waitKey(0);*/
		vconcat(A, B, result);
	/*	imshow("result",result);
		waitKey(0);*/
	}

	/*imshow("test",img_to_concat->at(0));
	waitKey(0);*/
	char name[7];
	name[0] = '0';
	name[1] = '0';
	name[2] = '0';
	if(num_of_concat_done>=10){
		 itoa(num_of_concat_done,&name[1],10);
	}else{
		itoa(num_of_concat_done,&name[2],10);
	}
	name[3] = '.';
	name[4] = 'j';
	name[5] = 'p';
	name[6] = 'g';
	if(verbose == true)
		printf("Writing image to disk : %s\n",name);
	imwrite(name,result);

}

bool compare_ext(char *fpath){
	char *suffix = strrchr(fpath,'.');

	if (suffix)
	{
		suffix++;
		//if (strcmp(suffix,"jpg"))
		if (strcmp(suffix,fpath))
		{
			// you got it
			return true;
		}else
		{
			return false;
		}
	}
}

void list_jpg_files(){
	
	tinydir_dir dir;
	int i;
	//tinydir_open_sorted(&dir, "/path/to/dir");
	tinydir_open_sorted(&dir, ".");
	if(verbose == true)
		printf("Open current dir\n");

	int num_of_concat_done = 0;
	int num_img_read = 0;

	if(verbose == true)
		printf("Number of Files in current directory : %i\n",dir.n_files);

	for (i = 0; i < dir.n_files; i++)
	{
		tinydir_file file;
		tinydir_readfile_n(&dir, &file, i);

		if(verbose == true)
			printf("File : %s\n", file.name);

		//check image file format by looking at the extension
		bool isJpeg = !strcmp("jpg",file.extension);

		if(isJpeg == true){

			Mat tmpImg = imread(file.name, CV_LOAD_IMAGE_COLOR);
			img_to_concat->push_back(tmpImg);

			num_img_read++;

			//here, we concatenate the images X by X, based on the command line, num_img_to_concat = X
			if(num_img_read>= num_img_to_concat){
				
				concat_img(img_to_concat,num_of_concat_done);

				if(verbose == true)
					printf("Concatenating images\n");

				num_img_read=0;
				num_of_concat_done++;
				img_to_concat->clear(); //TODO change that, very dirty :  deallocate images before clearing it !
			}
		}

	}

	tinydir_close(&dir);

}

int main( int argc, char** argv )
{

	//argv[1] = number of images to concat
	num_img_to_concat = atoi(argv[1]);
	img_to_concat = new vector<Mat >();

	//if(argc > 2 && argv[2] == "s") verbose = false;

	if(verbose == true)
		printf("Number of Files to concatenate into one image : %i\n",num_img_to_concat);

	list_jpg_files();

	return 0;
}