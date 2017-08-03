#ifndef DEFINES_H
#define DEFINES_H

#include <limits.h>

#define Width 800
#define Height 700
#define Xpos 240
#define Ypos 0

#define ABS( X ) ( X < 0 ? -X : X )

double Scale = 0.5;
int quality = 100;

double CamRe = 0.0;
double CamIm = 0.0;

bool Julia = false;
double julRe = 0.0;
double julIm = 0.0;

bool Animated = false;
int Animation = 0;
int impulse1 = INT_MAX;
int impulse2 = 0;

/*----------------------------------Gradient-------------------------------------*/

int color = 0;

struct float3 {
	float r, g, b;

	float3() : r(0.0f), g(0.0f), b(0.0f) {}
	float3(float _r, float _g, float _b) :
		r(_r), g(_g), b(_b) {}
};

float3 black(0.0f, 0.0f, 0.0f),
	white(1.0f, 1.0f, 1.0f),
	red(1.0f, 0.0f, 0.0f),
	orange(1.0f, 0.5f, 0.0f),
	yellow(1.0f, 1.0f, 0.0f),
	green(0.0f, 1.0f, 0.0f),
	cyan(0.0f, 1.0f, 1.0f),
	blue(0.0f, 0.0f, 1.0f),
	violett(1.0f, 0.0f, 1.0f),//(0.353f, 0.0f, 0.616f);
	salat(0.8f, 1.0f, 0.0f),
	pink(1.0f, 0.4f, 0.7f),
	lightblue(0.4f, 0.4f, 0.9f),
	greenbrown(0.84f, 0.9f, 0.0f),
	/*****************************/
	gordon(0.0f, 0.7f, 1.0f),
	sabrina(1.0f, 0.3f, 0.0f);

float3 RenderTarget[Width][Height];
int PointDepth[Width][Height];
float SmoothPart[Width][Height];

float3 Rainbow[210],
		Colors_4[250],
		Binary[2],
		Liquid[51];
float3 Impulses[210][50];

float3 OppositeImpulses[2][50];

#endif //DEFINES_H