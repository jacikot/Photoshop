// Dll1.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "slika_BMP.h"
#include "slika_PAM.h"
#include "slika_Image.h"
#include <iostream>
using namespace std;
#include "BMP.h"
#include "PAM.h"
#include "Image.h"

JNIEXPORT void JNICALL Java_slika_BMP_writeBMPHeader
(JNIEnv *env, jobject, jint w, jint h, jstring filepath) {
	jboolean isCopy;
	const char *convertedFp = (env)->GetStringUTFChars(filepath, &isCopy);
	BMP::saveHeader(w, h, convertedFp);
}

JNIEXPORT void JNICALL Java_slika_PAM_convertPAMtoBMP
(JNIEnv *env, jobject, jstring pampath, jstring bmppath) {
	cout << "usao" << endl;
	jboolean isCopy;
	Image i((env)->GetStringUTFChars(pampath, &isCopy));
	i.activateLayer(0);
	i.loadImage();
	i.saveImage((env)->GetStringUTFChars(bmppath, &isCopy));
}

JNIEXPORT void JNICALL Java_slika_PAM_convertBMPtoPAM
(JNIEnv *env, jobject, jstring bmppath, jstring pampath) {
	jboolean isCopy;
	Image i((env)->GetStringUTFChars(bmppath, &isCopy));
	i.activateLayer(0);
	i.loadImage();
	i.saveImage((env)->GetStringUTFChars(pampath, &isCopy));
}

JNIEXPORT void JNICALL Java_slika_Image_loadImageXML
(JNIEnv *env, jobject, jstring xmlpath, jstring bmppath) {
	jboolean isCopy;
	Image*i=Image::getProgramContext((env)->GetStringUTFChars(xmlpath,&isCopy));
	cout << "....................................prosao xml" << endl;
	i->loadImage();
	i->saveImage((env)->GetStringUTFChars(bmppath, &isCopy));
}