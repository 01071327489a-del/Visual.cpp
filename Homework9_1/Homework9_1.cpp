/*
 * 기능: GenericArray 클래스를 활용한 파일 기반 정렬 프로그램 테스트
 * 이름: 조성윤
 * 학번: 22312083
 * 날짜: 2026-05-13
 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <ctime>   // time()을 쓰기 위해 추가
#include <cstdlib> // srand()를 쓰기 위해 추가
#include "GenericArray.h"

using namespace std;

int main() {
    // 프로그램 실행 시 딱 한 번 실행되어, 매번 셔플이 다르게 작동하도록 난수 시드 초기화
    srand((unsigned int)time(NULL));

    fstream fin_int, fin_dbl, fin_str;
    int per_line = 10;       // 한 줄에 출력할 데이터 개수
    int sample_lines = 2;    // 앞뒤로 몇 줄씩 샘플링해서 보여줄지 결정하는 변수

    
    // 1. Integer 배열 정렬 테스트
    fin_int.open("intArray_data.txt");
    if (fin_int.fail()) { // 파일 열기 실패 시 에러 띄우고 종료
        cout << "Error in reading int_array data file (intArray_data.txt) !!!" << endl;
        exit(1);
    }
    GenericArray<int> ga_Int("GA_Int");
    fin_int >> ga_Int; // 파일에서 데이터를 통째로 읽어옴
    fin_int.close();   // 사용이 끝난 파일 스트림 닫기

    cout << "ga_int before sorting : " << endl;
    ga_Int.printArray(per_line, sample_lines); // 정렬 전 출력
    cout << endl;

    ga_Int.insertSort(); // 삽입 정렬 시작

    cout << "ga_int after sorting : " << endl;
    ga_Int.printArray(per_line, sample_lines); // 정렬 후 결과 출력
    cout << endl;


    // 2. Double 배열 정렬 테스트
    fin_dbl.open("dblArray_data.txt");
    if (fin_dbl.fail()) {
        cout << "Error in reading double_array data file (dblArray_data.txt) !!!" << endl;
        exit(1);
    }
    GenericArray<double> ga_Dbl("GA_Double");
    fin_dbl >> ga_Dbl;
    fin_dbl.close();

    cout << "ga_Dbl before sorting : " << endl;
    ga_Dbl.printArray(per_line, sample_lines);
    cout << endl;

    ga_Dbl.insertSort();

    cout << "ga_Dbl after sorting : " << endl;
    ga_Dbl.printArray(per_line, sample_lines);
    cout << endl;


    // 3. String 배열 정렬 테스트 (문자열 사전순 정렬)
    fin_str.open("strArray_data.txt");
    if (fin_str.fail()) {
        cout << "Error in reading string_array data file (strArray_data.txt) !!!" << endl;
        exit(1);
    }
    GenericArray<string> ga_str("GA_String");
    fin_str >> ga_str;
    fin_str.close();

    cout << "ga_str before sorting : " << endl;
    ga_str.printArray(per_line, sample_lines);
    cout << endl;

    ga_str.insertSort();

    cout << "ga_str after sorting : " << endl;
    ga_str.printArray(per_line, sample_lines);
    cout << endl;

    return 0; // 프로그램 정상 종료
}