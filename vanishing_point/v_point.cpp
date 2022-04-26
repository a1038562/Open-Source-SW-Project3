#include "v_point.h"

Mat vanishing_point(Mat frame)
{
	//HoughLinesP() 함수에 사용할 변수
	double width = frame.cols;
	double height = frame.rows;
	double rho = 1;
	double theta = CV_PI / 180;
	double hough_thresh = 100;
	double min_length = 10;
	double max_gap = 10;

	Mat line_img = Mat::zeros(height, width, CV_8UC3); //선분을 표시할 매트릭스

	//선분 정보를 저장할 배열 및 인덱스
	vector<Vec4d>lines, lineL, lineR;
	Vec4d  lineP, lineLP, lineRP;
	double* lineLX, * lineLY, * lineRX, * lineRY;
	int indexL = 0, indexR = 0;

	//좌표를 저장할 Point 변수
	Point2d p1, p2, pl1, pl2, pr1, pr2, p1L, p2L, p1R, p2R, vp;

	//gsl_fit_linear() 함수에 사용할 변수 및 계수를 나타내는 변수
	double c0, c1, cov00, cov01, cov11, sumsq;
	double c0L = 0, c1L = 0, c0R = 0, c1R = 0;

	HoughLinesP(frame, lines, rho, theta, hough_thresh, min_length, max_gap); //허프변환 함수


	for (int i = 0; i < lines.size(); i++)
	{
		lineP = lines[i];
		p1 = { lineP[0], lineP[1] };
		p2 = { lineP[2], lineP[3] };
		double dx = (p2.x - p1.x);
		double dy = (p2.y - p1.y);
		double slope; //기울기

		if (dx != 0) //x의 변화량이 0이 아닐 때
			slope = dy / dx;
		else //x이 변화량이 0일 때
			slope = 999.0; 

		if (abs(slope) > 0.5 && abs(slope) < 5) { //기울기가 조건에 맞는다면

			//기울기가 양수인 선분
			if (slope < 0) {
				lineL.push_back(lineP);
			}

			//기울기가 음수인 선분
			if (slope > 0) {
				lineR.push_back(lineP);
			}

		}
	}

	//선분 정보를 저장할 배열 동적할당
	lineLX = new double[2 * lineL.size() + 1];
	lineLY = new double[2 * lineL.size() + 1];
	lineRX = new double[2 * lineR.size() + 1];
	lineRY = new double[2 * lineR.size() + 1];

	//기울기가 양수인 선분 정보
	for (int i = 0; i < lineL.size(); i++)
	{
		lineLP = lineL[i];
		pl1 = { lineLP[0], lineLP[1] };
		pl2 = { lineLP[2], lineLP[3] };
		lineLX[indexL] = pl1.x;
		lineLY[indexL] = pl1.y;
		indexL++;
		lineLX[indexL] = pl2.x;
		lineLY[indexL] = pl2.y;

	}
	gsl_fit_linear(lineLX, 1, lineLY, 1, indexL, &c0, &c1, &cov00, &cov01, &cov11, &sumsq); //적절한 함수 찾기

	//계수
	c0L = c0; 
	c1L = c1;

	//계수를 사용하여 좌표 찾기
	p1L.x = (height - c0L) / c1L;
	p1L.y = height;
	p2L.x = (0 - c0L) / c1L;
	p2L.y = 0;

	line(line_img, p1L, p2L, Scalar(0, 255, 255), 10); //선분 표시

	//기울기가 음수인 선분 정보
	for (int i = 0; i < lineR.size(); i++)
	{
		lineRP = lineR[i];
		pr1 = { lineRP[0], lineRP[1] };
		pr2 = { lineRP[2], lineRP[3] };
		lineRX[indexR] = pr1.x;
		lineRY[indexR] = pr1.y;
		indexR++;
		lineRX[indexR] = pr2.x;
		lineRY[indexR] = pr2.y;

	}
	gsl_fit_linear(lineRX, 1, lineRY, 1, indexR, &c0, &c1, &cov00, &cov01, &cov11, &sumsq); //적절한 함수 찾기

	//계수
	c0R = c0;
	c1R = c1;

	//계수를 사용하여 좌표 찾기
	p1R.x = (height - c0R) / c1R; 
	p1R.y = height;
	p2R.x = (0 - c0R) / c1R;
	p2R.y = 0;

	line(line_img, p1R, p2R, Scalar(0, 0, 255), 10); //선분 표시

	//소실점을 원으로 표시
	vp.x = abs((c0R - c0L) / (c1R - c1L)); 
	vp.y = c1R * vp.x + c0R;
	circle(line_img, vp, 30, (0, 0, 255), 10);

	//동적할당 해제
	delete[] lineLX;
	delete[] lineLY;
	delete[] lineRX;
	delete[] lineRY;

	return line_img; //결과 반환
}

Mat canny(Mat frame) //canny edge 검출 함수
{
	Mat canny_img;
	Canny(frame, canny_img, 70, 210);
	return canny_img;
}