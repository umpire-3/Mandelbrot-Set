#include <iostream>
#include <GL\freeglut.h>
#include <thread>
#include <math.h>
#include "defines.h"

void Timer(int);
void AnimateSet();
void Render();
void Display();
void KeyPress(unsigned char, int, int);
void MouseActions(int, int, int, int);
int isMandelbrot(double&, double&, int&);//, float*);
int isJulia(double&, double&, int&);//, float*);
void Transform(int, int, double*, double*);
void BackTransform(double, double, int*, int*);
float3 Gradient(float3, float3, int, int);
float3 Gradient2(float3, float3, int, int);
float3 Gradient3(float3, float3, int, int);
void SetColorBuffer(int&, int&, int);//, float);
void ResetRenderTarget(int = 0, int = Width, int = 0, int = Height);
void InitBuffers();

int(*inSet)(double&, double&, int&) = isMandelbrot;

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(Xpos, Ypos);
	glutCreateWindow("Mandelbrot Set");
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, Width, 0, Height);

	glutKeyboardFunc(KeyPress);
	glutMouseFunc(MouseActions);
	glutTimerFunc(1, Timer, 1);

	InitBuffers();
	Render();
	
	glutMainLoop();
	
	return 0;
}


void Timer(int value) {
	if (Animated) AnimateSet();
	glutTimerFunc(1, Timer, 1);
}

void AnimateSet() {
	impulse1--;
	impulse2++;
	std::thread Animator2(ResetRenderTarget, 0, Width / 4, 0, Height);
	std::thread Animator3(ResetRenderTarget, Width / 4, Width / 2, 0, Height);
	std::thread Animator4(ResetRenderTarget, Width / 2, Width * 3/4, 0, Height);
	ResetRenderTarget(Width * 3/4, Width);

	Animator2.join();
	Animator3.join();
	Animator4.join();

	Display();
}

void RenderInTarget(int x_0, int x_1, int y_0, int y_1) {
	double Re, Im;
	for (int x = x_0; x < x_1; x++) {
		for (int y = y_0; y < y_1; y++) {
			Transform(x, y, &Re, &Im);
			PointDepth[x][y] = inSet(Re, Im, quality);//, &SmoothPart[x][y]);
			SetColorBuffer(x, y, PointDepth[x][y]);//, SmoothPart[x][y]);
		}
	}
}

void Render() {
	//memset(RenderTarget, 0, sizeof(float3) * Width * Height);

	std::thread Render2(RenderInTarget, 0, Width/2, 0, Height/2);
	std::thread Render3(RenderInTarget, 0, Width/2, Height/2, Height);
	std::thread Render4(RenderInTarget, Width/2, Width, 0, Height/2);
	RenderInTarget(Width/2, Width, Height/2, Height);

	Render2.join();
	Render3.join();
	Render4.join();

	Display();
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POINTS);
	for (int x = 0; x < Width; x++) {
		for (int y = 0; y < Height; y++) {
			glColor3f(RenderTarget[x][y].r, RenderTarget[x][y].g, RenderTarget[x][y].b);
			glVertex2i(x, y);
		}
	}
	glEnd();

	glutSwapBuffers();
}

void ConsoleLog() {
	std::cout.precision(18);
	std::cout << "Scale = " << Scale
		<< "\nQuality = "	  << quality
		<< "\n\nComplex coordinate = "
		<< CamRe << (CamIm < 0 ? " - " : " + ")
		<< abs(CamIm) << "i\n";
	if (Julia) {
		std::cout << "Julia point = " << julRe
				  << (julIm < 0 ? " - " : " + ")
				  << abs(julIm) << "i\n";
	}
	std::cout << "\n";
}

void KeyPress(unsigned char key, int tmp1, int tmp2)
{
	switch (key) {
		case 'e': Scale *= 2.0;
			Render();
			break;
		case 'q': Scale /= 2.0;
			Render();
			break;
		case 'w': Transform(Width / 2, Height / 2 + 40, &CamRe, &CamIm);
			Render();
			break;
		case 'a': Transform(Width / 2 - 40, Height / 2, &CamRe, &CamIm);
			Render();
			break;
		case 's': Transform(Width / 2, Height / 2 - 40, &CamRe, &CamIm);
			Render();
			break;
		case 'd': Transform(Width / 2 + 40, Height / 2, &CamRe, &CamIm);
			Render();
			break;
		case 'u': quality *= 2;
			Render();
			break;
		case 'j': quality /= 2;
			Render();
			break;
		case 'c': ConsoleLog();
			break;
		case 'x': Animated ^= true;
			break;
		case 'v': Animation++;
			if (Animation > 3) Animation = 0;
			ResetRenderTarget();
			break;
		case 'r':
			std::cout << "Animation type = " << Animation << "\n\n";
			break;
		case 'z': color++;
			if (color > 2) color = 0;
			ResetRenderTarget();
			Display();
			break;
		case 27: exit(0);
	}
}

void MouseActions(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		switch (state) {
		case GLUT_DOWN:
			break;
		case GLUT_UP:
			Transform(x, Height - y, &CamRe, &CamIm);
			Render();
			break;
		}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		switch (state) {
		case GLUT_DOWN:
			break;
		case GLUT_UP:
			if (Julia) {
				inSet = isMandelbrot;
			}
			else {
				inSet = isJulia;
				Transform(x, Height - y, &julRe, &julIm);
			}
			Julia ^= true;
			Render();
			break;
		}
	}
}

int isMandelbrot(double& Re, double& Im, int& epsilon) {
	double xn = Re, yn = Im, xn1;
	for (int i = 1; i <= epsilon; i++) {
		double x2 = xn*xn, y2 = yn*yn;
		xn1 = x2 - y2 + Re;
		yn = 2*xn*yn + Im;
		xn = xn1;
		if (x2 + y2 > 4) {
			/*float log_zn = log(xn*xn + yn*yn) / 2;
			*spart = i - log(log_zn / log(1 << 8));*/
			return i;
		}
	}
	return 0;
}
	
int isJulia(double& Re, double& Im, int& epsilon) {
	double xn = Re, yn = Im, xn1;
	for (int i = 1; i <= epsilon; i++) {
		double x2 = xn*xn, y2 = yn*yn;
		xn1 = x2 - y2 + julRe;
		yn = 2*xn*yn + julIm;
		xn = xn1;
		if (x2 + y2 > 4) {
			/*float log_zn = log(xn*xn + yn*yn) / 2;
			*spart = i - log(log_zn / log(2)) / log(2);*/
			return i;
		}
	}
	return 0;
}

void Transform(int x, int y, double *Re, double *Im) {
	*Re = CamRe + (2 * x - Width) / (Scale*Width);
	*Im = CamIm + (2 * y - Height) / (Scale*Height);
}

void BackTransform(double Re, double Im, int *x, int *y) {
	*x = (Width + (Re - CamRe)*Scale*Width) / 2;
	*y = (Height + (Im - CamIm)*Scale*Height) / 2;
}

float3 Gradient(float3 c1, float3 c2, int n, int d) {
	return { c1.r + (c2.r - c1.r)*n / d,
			 c1.g + (c2.g - c1.g)*n / d,
			 c1.b + (c2.b - c1.b)*n / d };
}

float3 Gradient2(float3 c1, float3 c2, int n, int d) {
	return{ c2.r - (c2.r - c1.r)*(d - n)*(d - n) / (d*d),
		c2.g - (c2.g - c1.g)*(d - n)*(d - n) / (d*d),
		c2.b - (c2.b - c1.b)*(d - n)*(d - n) / (d*d) };
}

float3 Gradient3(float3 c1, float3 c2, int n, int d) {
	return{ c2.r - (c2.r - c1.r)*(d - n)*(d - n)*(d - n) / (d*d*d),
		c2.g - (c2.g - c1.g)*(d - n)*(d - n)*(d - n) / (d*d*d),
		c2.b - (c2.b - c1.b)*(d - n)*(d - n)*(d - n) / (d*d*d) };
}

void SetColorBuffer(int& x, int& y, int n) {
	if (n == 0) {
		if (color == 2) {
			RenderTarget[x][y] = white;
		}
		else {
			memset(&RenderTarget[x][y], 0, sizeof(float3));
		}
		return;
	}

	if (Animated) {
		switch (Animation) {
		case 0:
		{
			int dn = ABS(((impulse1 & 127) - (n & 127)));
			if (dn > 50) dn = 128 - dn;
			if (dn < 50) {
				RenderTarget[x][y].r = n*n / (float)(quality*quality) + Impulses[(n - (impulse1 & 127) >> 7) % 210][dn].r;
				RenderTarget[x][y].g = n*n / (float)(quality*quality) + Impulses[(n - (impulse1 & 127) >> 7) % 210][dn].g;
				RenderTarget[x][y].b = n / (float)quality + Impulses[(n - (impulse1 & 127) >> 7) % 210][dn].b;
				return;
			}
		}
		break;
		case 1:
		{
			int dn = ABS(((impulse1 & 127) - (n & 127)));
			if (dn > 50) dn = 128 - dn;
			if (dn < 50) {
				RenderTarget[x][y].r = n*n / (float)(quality*quality) + Liquid[dn].r;
				RenderTarget[x][y].g = n*n / (float)(quality*quality) + Liquid[dn].g;
				RenderTarget[x][y].b = n / (float)quality + Liquid[dn].b;
				return;
			}
		}
		break;
		case 2:
		{
			int dn = ABS(((impulse1 & 127) - (n & 127)));
			if (dn > 50) dn = 128 - dn;
			if (dn < 50) {
				RenderTarget[x][y].r = n*n / (float)(quality*quality) + Impulses[(impulse1 - n) % 210][dn].r;
				RenderTarget[x][y].g = n*n / (float)(quality*quality) + Impulses[(impulse1 - n) % 210][dn].g;
				RenderTarget[x][y].b = n / (float)quality + Impulses[(impulse1 - n) % 210][dn].b;
				return;
			}
		}
		break;
		case 3:
		{
			int dn1 = ABS(((impulse1 & 127) - (n & 127)));
			int dn2 = ABS(((impulse2 & 127) - (n & 127)));
			int di = ABS(((impulse1 & 127) - (impulse2 & 127)));
			if (dn1 > 50) dn1 = 128 - dn1;
			if (dn2 > 50) dn2 = 128 - dn2;
			if (di > 50) di = 128 - di;
			memset(&RenderTarget[x][y], 0, sizeof(float3));
			if (dn1 < 50) {
				RenderTarget[x][y].r += OppositeImpulses[0][dn1].r;
				RenderTarget[x][y].g += OppositeImpulses[0][dn1].g;
				RenderTarget[x][y].b += OppositeImpulses[0][dn1].b;
				if (di < 50) {
					RenderTarget[x][y].r -= OppositeImpulses[0][di].r;
					RenderTarget[x][y].b -= OppositeImpulses[0][di].b;
				}
			}
			if (dn2 < 50) {
				RenderTarget[x][y].r += OppositeImpulses[1][dn2].r;
				RenderTarget[x][y].g += OppositeImpulses[1][dn2].g;
				RenderTarget[x][y].b += OppositeImpulses[1][dn2].b;
				if (di < 50) {
					RenderTarget[x][y].r -= OppositeImpulses[1][di].r;
					RenderTarget[x][y].b -= OppositeImpulses[1][di].b;
				}
			}
			return;
		}
		break;
		}
	}

	switch (color) {
	case 0:
		RenderTarget[x][y] = Rainbow[n % 210];
		break;
	case 1:
		RenderTarget[x][y] = Colors_4[n % 250];
		break;
	case 2:
		RenderTarget[x][y].r = n*n / (float)(quality*quality);
		RenderTarget[x][y].g = n*n / (float)(quality*quality);
		RenderTarget[x][y].b = n / (float)quality;
		break;
	}
}

void ResetRenderTarget(int x0, int xN, int y0, int yN) {
	for (int x = x0; x < xN; x++) {
		for (int y = y0; y < yN; y++) {
			SetColorBuffer(x, y, PointDepth[x][y]);//, SmoothPart[x][y]);
		}
	}
}

void InitBuffers() {
	memset(RenderTarget, 0, sizeof(float3)*Width*Height);
	memset(PointDepth, 0, sizeof(int)*Width*Height);
	memset(SmoothPart, 0, sizeof(float)*Width*Height);

	Binary[0] = white;//cyan;
	Binary[1] = black;//red;

	for (int i = 0; i < 210; i++) {
		int m = i % 30;
		if (i < 30) Rainbow[i] = Gradient(red, orange, m, 30);
		else if (i < 60) Rainbow[i] = Gradient(orange, yellow, m, 30);
		else if (i < 90) Rainbow[i] = Gradient(yellow, green, m, 30);
		else if (i < 120) Rainbow[i] = Gradient(green, cyan, m, 30);
		else if (i < 150) Rainbow[i] = Gradient(cyan, blue, m, 30);
		else if (i < 180) Rainbow[i] = Gradient(blue, violett, m, 30);
		else Rainbow[i] = Gradient(violett, red, m, 30);
	}

	for (int i = 0; i < 250; i++) {
		int m = i % 50;
		if (i < 50) Colors_4[i] = Gradient(greenbrown, yellow, m, 50);
		else if (i < 100) Colors_4[i] = Gradient(yellow, pink, m, 50);
		else if (i < 150) Colors_4[i] = Gradient(pink, lightblue, m, 50);
		else if (i < 200) Colors_4[i] = Gradient(lightblue, white, m, 50);
		else Colors_4[i] = Gradient(white, greenbrown, m, 50);
	}

	for (int i = 0; i < 51; i++) {
		Liquid[i] = Gradient3(cyan, black, i, 51);
	}
	for (int i = 0; i < 50; i++) {
		OppositeImpulses[0][i] = Gradient3(gordon, black, i, 50);
		OppositeImpulses[1][i] = Gradient3(sabrina, black, i, 50);
	}
	for (int i = 0; i < 210; i++) {
		for (int j = 0; j < 50; j++) {
			Impulses[i][j] = Gradient3(Rainbow[i], black, j, 50);
		}
	}
}