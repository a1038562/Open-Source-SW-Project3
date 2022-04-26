#include "v_point.h"

int main()
{
	string img_name; //�̹�����
	double width, height; //�̹��� �ʺ�, ����

	Mat img, canny_img, line_img, result; //�̹����� ������ ��Ʈ����

	while (1)
	{
		cout << endl << "�ҽ����� ã�� �̹������� �Է��Ͻÿ�(���� ���� ����)" << endl << "(�����Ϸ��� \"����\"��� �Է�)" << endl << endl << ">> ";
		cin >> img_name; //�̹����� �Է� �ޱ�

		if (img_name == "����") //"����" �Է� �� ����
			return 0;

		img = imread(img_name); //�̹��� �б�
		width = img.rows;
		height = img.cols;

		if (height > 800 && width > 800) //�̹����� ũ�Ⱑ ���� ũ�� �̻��̸� ũ�� �缳��
			resize(img, img, Size(0.5 * height, 0.5 * width));

		if (img.empty()) { //�̹����� �������� ������ ����
			cout << endl << "�̹����� �������� �ʽ��ϴ�." << endl << endl;
		}

		canny_img = canny(img); //�̹��� canny edge ����
		line_img = vanishing_point(canny_img); //�ҽ����� ã�� �Լ� ���� �� ��� ��ȯ
		addWeighted(img, 0.8, line_img, 1, 0, result); //�̹��� ���ϱ�
		imshow("result", result); //��� ���

		if (waitKey(0) == 27) //ESCŰ �Է� �� ����
			break;

	}
	img.release(); //��ü ����
	destroyAllWindows(); //������ ����

}