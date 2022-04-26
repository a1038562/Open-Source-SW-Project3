#include "v_point.h"

Mat vanishing_point(Mat frame)
{
	//HoughLinesP() �Լ��� ����� ����
	double width = frame.cols;
	double height = frame.rows;
	double rho = 1;
	double theta = CV_PI / 180;
	double hough_thresh = 100;
	double min_length = 10;
	double max_gap = 10;

	Mat line_img = Mat::zeros(height, width, CV_8UC3); //������ ǥ���� ��Ʈ����

	//���� ������ ������ �迭 �� �ε���
	vector<Vec4d>lines, lineL, lineR;
	Vec4d  lineP, lineLP, lineRP;
	double* lineLX, * lineLY, * lineRX, * lineRY;
	int indexL = 0, indexR = 0;

	//��ǥ�� ������ Point ����
	Point2d p1, p2, pl1, pl2, pr1, pr2, p1L, p2L, p1R, p2R, vp;

	//gsl_fit_linear() �Լ��� ����� ���� �� ����� ��Ÿ���� ����
	double c0, c1, cov00, cov01, cov11, sumsq;
	double c0L = 0, c1L = 0, c0R = 0, c1R = 0;

	HoughLinesP(frame, lines, rho, theta, hough_thresh, min_length, max_gap); //������ȯ �Լ�


	for (int i = 0; i < lines.size(); i++)
	{
		lineP = lines[i];
		p1 = { lineP[0], lineP[1] };
		p2 = { lineP[2], lineP[3] };
		double dx = (p2.x - p1.x);
		double dy = (p2.y - p1.y);
		double slope; //����

		if (dx != 0) //x�� ��ȭ���� 0�� �ƴ� ��
			slope = dy / dx;
		else //x�� ��ȭ���� 0�� ��
			slope = 999.0; 

		if (abs(slope) > 0.5 && abs(slope) < 5) { //���Ⱑ ���ǿ� �´´ٸ�

			//���Ⱑ ����� ����
			if (slope < 0) {
				lineL.push_back(lineP);
			}

			//���Ⱑ ������ ����
			if (slope > 0) {
				lineR.push_back(lineP);
			}

		}
	}

	//���� ������ ������ �迭 �����Ҵ�
	lineLX = new double[2 * lineL.size() + 1];
	lineLY = new double[2 * lineL.size() + 1];
	lineRX = new double[2 * lineR.size() + 1];
	lineRY = new double[2 * lineR.size() + 1];

	//���Ⱑ ����� ���� ����
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
	gsl_fit_linear(lineLX, 1, lineLY, 1, indexL, &c0, &c1, &cov00, &cov01, &cov11, &sumsq); //������ �Լ� ã��

	//���
	c0L = c0; 
	c1L = c1;

	//����� ����Ͽ� ��ǥ ã��
	p1L.x = (height - c0L) / c1L;
	p1L.y = height;
	p2L.x = (0 - c0L) / c1L;
	p2L.y = 0;

	line(line_img, p1L, p2L, Scalar(0, 255, 255), 10); //���� ǥ��

	//���Ⱑ ������ ���� ����
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
	gsl_fit_linear(lineRX, 1, lineRY, 1, indexR, &c0, &c1, &cov00, &cov01, &cov11, &sumsq); //������ �Լ� ã��

	//���
	c0R = c0;
	c1R = c1;

	//����� ����Ͽ� ��ǥ ã��
	p1R.x = (height - c0R) / c1R; 
	p1R.y = height;
	p2R.x = (0 - c0R) / c1R;
	p2R.y = 0;

	line(line_img, p1R, p2R, Scalar(0, 0, 255), 10); //���� ǥ��

	//�ҽ����� ������ ǥ��
	vp.x = abs((c0R - c0L) / (c1R - c1L)); 
	vp.y = c1R * vp.x + c0R;
	circle(line_img, vp, 30, (0, 0, 255), 10);

	//�����Ҵ� ����
	delete[] lineLX;
	delete[] lineLY;
	delete[] lineRX;
	delete[] lineRY;

	return line_img; //��� ��ȯ
}

Mat canny(Mat frame) //canny edge ���� �Լ�
{
	Mat canny_img;
	Canny(frame, canny_img, 70, 210);
	return canny_img;
}