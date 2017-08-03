#pragma once

class Fractal {
	double Width, Height;

	double Scale;
	double quality;

	double CamRe;
	double CamIm;

public:
	Fractal(double S = 0.5, double q = 100, double Re = 0.0, double Im = 0.0) :
		Scale(S),
		quality(q),
		CamRe(Re),
		CamIm(Im)
	{}

	void Zoom(double dz) {
		Scale *= dz;
	}

	void Move(double dRe, double dIm) {
		CamRe += dRe;
		CamIm += dIm;
	}

	void Transform(int x, int y, double *Re, double *Im) {
		*Re = CamRe + (2 * x - Width) / (Scale*Width);
		*Im = CamIm + (2 * y - Height) / (Scale*Height);
	}

	void BackTransform(double Re, double Im, int *x, int *y) {
		*x = (Width + (Re - CamRe)*Scale*Width) / 2;
		*y = (Height + (Im - CamIm)*Scale*Height) / 2;
	}
};