/*
 * 기능: 다양한 데이터 타입을 처리하는 템플릿 클래스 GenericArray 구현 및 정렬/출력
 * 이름: 조성윤
 * 학번: 22312083
 * 날짜: 2026-05-13
 * * [프로그램 ]
 * - 템플릿(Template)을 사용해 int, double, string 등 다양한 타입을 다룰 수 있도록 구현
 * - 생성자와 소멸자를 이용해 동적 배열의 메모리를 안전하게 관리 (메모리 누수 방지)
 * - rand() 함수의 한계(최대 32767)를 넘는 대용량 배열도 골고루 섞이도록 셔플 함수 개선
 * - 삽입 정렬(Insertion Sort) 및 화면 크기를 고려한 데이터 샘플링 출력 기능 구현
 */

#ifndef GENERIC_ARRAY_H
#define GENERIC_ARRAY_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip> // setw() 등 출력 포맷 설정을 위해 포함
#include <cmath>   // abs() 함수 사용을 위해 포함

using namespace std;

// 템플릿 클래스 내부에서 프렌드 함수를 쓰기 위한 전방 선언
template <typename T> class GenericArray;
template <typename T> istream& operator>>(istream& fin, GenericArray<T>& ga);
template <typename T> ostream& operator<<(ostream& fout, const GenericArray<T>& ga);

template <typename T>
class GenericArray {
    // private 멤버인 내부 배열에 파일 스트림이 접근할 수 있도록 프렌드 선언
    friend istream& operator>> <T>(istream& fin, GenericArray<T>& ga);
    friend ostream& operator<< <T>(ostream& fout, const GenericArray<T>& ga);

public:
    // 생성자: 배열 이름과 최대 용량을 받아 동적 배열 할당
    GenericArray(string nm, int capacity = 0) {
        this->name = nm;
        this->capacity = capacity;
        this->num_elements = 0;
        if (capacity > 0) {
            GA = new T[capacity]; // 힙(Heap) 메모리에 동적 할당
        }
        else {
            GA = nullptr;
        }
    }

    // 소멸자: 객체가 사라질 때 할당했던 동적 메모리를 해제 (메모리 누수 방지)
    ~GenericArray() {
        if (GA != nullptr) {
            delete[] GA; // 배열 형태로 할당했으므로 delete[] 사용
        }
    }

    // 순차적인 테스트 데이터를 생성하고 무작위로 섞어주는 함수
    void genBigRandArray(int size, T offset) {
        if (GA != nullptr) delete[] GA; // 기존에 쓰던 배열이 있다면 먼저 해제
        this->capacity = size;
        this->num_elements = size;
        GA = new T[capacity];

        for (int i = 0; i < size; i++) {
            GA[i] = (T)i + offset; // 지정된 오프셋부터 순서대로 값 채우기
        }
        shuffle(); // 생성 후 무작위로 섞기
    }

    void setNumElements(int num_elements) {
        this->num_elements = num_elements;
    }

    /*
     * 함수명: shuffle
     * 중요 요구사항: RAND_MAX(32,767)보다 큰 대용량 배열도 안전하게 섞이도록 처리
     * 원리: 기본 rand()는 0~32767만 나오므로, rand() 두 개를 비트 연산으로 이어 붙여서
     * 32767보다 훨씬 큰 인덱스 위치의 원소들도 골고루 선택되어 섞이도록 만듦.
     */
    void shuffle() {
        if (num_elements <= 1) return;

        for (int i = 0; i < num_elements; i++) {
            // rand() 값을 15칸 밀고 또 다른 rand()를 더해 큰 난수 생성
            int big_rand = (rand() << 15) | rand();

            // 배열 크기 안으로 안전하게 인덱스가 들어오도록 나머지(%) 연산 수행
            int rn = abs(big_rand) % num_elements;

            // 두 원소의 위치를 서로 맞바꿈 (Swap)
            T temp = GA[i];
            GA[i] = GA[rn];
            GA[rn] = temp;
        }
    }

    /*
     * 함수명: insertSort
     * 기능: 삽입 정렬(Insertion Sort) 알고리즘을 사용해 데이터를 오름차순 정렬
     */
    void insertSort() {
        for (int i = 1; i < num_elements; i++) {
            T key = GA[i]; // 현재 정렬할 기준 원소 선택
            int j = i - 1;

            // 앞쪽에 있는 원소들 중 key보다 큰 값들을 뒤로 한 칸씩 이동
            while (j >= 0 && GA[j] > key) {
                GA[j + 1] = GA[j];
                j--;
            }
            GA[j + 1] = key; // 알맞은 위치에 찾아서 삽입
        }
    }

    /*
     * 함수명: printArray
     * 기능: 출력 포맷(setw)을 일정하게 맞추고, 데이터가 많으면 앞/뒤만 보여주는 생략 출력 기능
     */
    void printArray(int per_line, int sample_lines) {
        int max_display = per_line * sample_lines; // 한 영역당 최대로 보여줄 원소 개수

        // 데이터 개수가 적은 경우 생략 없이 전부 출력
        if (num_elements <= max_display * 2) {
            for (int i = 0; i < num_elements; i++) {
                cout << left << setw(10) << GA[i]; // 10칸 공간 확보 후 왼쪽 정렬 출력
                if ((i + 1) % per_line == 0) cout << "\n";
            }
            if (num_elements % per_line != 0) cout << "\n";
        }
        // 대용량 데이터의 경우 화면을 아끼기 위해 중간을 생략하고 앞/뒤만 분할 출력
        else {
            // 1. 앞부분(Head) 샘플 출력
            for (int i = 0; i < max_display; i++) {
                cout << left << setw(10) << GA[i];
                if ((i + 1) % per_line == 0) cout << "\n";
            }

            // 2. 중간 생략 표시 (...)
            cout << left << setw(10) << "..." << "\n";

            // 3. 뒷부분(Tail) 샘플 출력
            int start_idx = num_elements - max_display;
            for (int i = start_idx; i < num_elements; i++) {
                cout << left << setw(10) << GA[i];
                if ((i - start_idx + 1) % per_line == 0) cout << "\n";
            }
        }
    }

    T* getGA() { return GA; }

protected:
    string name;       // 배열 이름
    T* GA;             // 데이터 동적 할당용 포인터
    int num_elements;  // 현재 채워진 데이터 개수
    int capacity;      // 배열의 최대 용량
};

// 파일에서 데이터를 읽어오는 >> 연산자 오버로딩
template <typename T>
istream& operator>>(istream& fin, GenericArray<T>& ga) {
    string name;
    int cap;
    // 파일의 첫 부분에서 [배열이름]과 [데이터개수]를 먼저 읽어옴
    if (fin >> name >> cap) {
        ga.name = name;
        if (ga.GA != nullptr) delete[] ga.GA; // 기존 배열 정리
        ga.capacity = cap;
        ga.num_elements = cap;
        ga.GA = new T[cap]; // 파일에 적힌 개수만큼 새로 동적 할당

        // 데이터 개수만큼 파일에서 차례대로 읽어 배열에 채움
        for (int i = 0; i < cap; i++) {
            fin >> ga.GA[i];
        }
    }
    return fin;
}

// 화면이나 파일로 데이터를 내보내는 << 연산자 오버로딩
template <typename T>
ostream& operator<<(ostream& fout, const GenericArray<T>& ga) {
    fout << ga.name << " (Size: " << ga.num_elements << ")" << endl;
    for (int i = 0; i < ga.num_elements; i++) {
        fout << ga.GA[i] << " ";
    }
    return fout;
}

#endif // GENERIC_ARRAY_H