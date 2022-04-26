#include "v_point.h"

int main()
{
	string img_name; //이미지명
	double width, height; //이미지 너비, 높이

	Mat img, canny_img, line_img, result; //이미지를 저장할 매트릭스

	while (1)
	{
		cout << endl << "소실점을 찾을 이미지명을 입력하시오(파일 형식 포함)" << endl << "(종료하려면 \"종료\"라고 입력)" << endl << endl << ">> ";
		cin >> img_name; //이미지명 입력 받기

		if (img_name == "종료") //"종료" 입력 시 종료
			return 0;

		img = imread(img_name); //이미지 읽기
		width = img.rows;
		height = img.cols;

		if (height > 800 && width > 800) //이미지의 크기가 일정 크기 이상이면 크기 재설정
			resize(img, img, Size(0.5 * height, 0.5 * width));

		if (img.empty()) { //이미지가 존재하지 않으면 실행
			cout << endl << "이미지가 존재하지 않습니다." << endl << endl;
		}

		canny_img = canny(img); //이미지 canny edge 검출
		line_img = vanishing_point(canny_img); //소실점을 찾는 함수 실행 및 결과 반환
		addWeighted(img, 0.8, line_img, 1, 0, result); //이미지 합하기
		imshow("result", result); //결과 출력

		if (waitKey(0) == 27) //ESC키 입력 시 종료
			break;

	}
	img.release(); //객체 해제
	destroyAllWindows(); //윈도우 종료

}