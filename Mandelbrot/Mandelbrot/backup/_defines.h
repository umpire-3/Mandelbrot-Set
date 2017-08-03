#ifndef DEFINES_H
#define DEFINES_H

#define Width 800
#define Height 700
#define Xpos 240
#define Ypos 0

#define ABS( X ) ( X < 0 ? -X : X )

double Scale = 131072;
int quality = 12800;

double CamRe = -1.95370435714721657;
double CamIm = -2.73323059082562053e-06;

bool Julia = true;
double julRe = -1.95370173263549773;
double julIm = -7.68470764160686985e-06;

bool Animated = false;
int Animation = 1;
int impulse = INT_MAX;

/*----------------------------------Gradient-------------------------------------*/

int color = 2;

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
	greenbrown(0.84f, 0.9f, 0.0f);

float3 RenderTarget[Width][Height];
int PointDepth[Width][Height];

float3 Rainbow[210],
		Colors_4[250],
		Liquid[50];
float3 Impulses[210][50];

#endif //DEFINES_H