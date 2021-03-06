// 单例模式-懒汉式多线程问题.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<Windows.h>
#include "winbase.h"
#include <process.h>
using namespace std;

/*
1 "懒汉"模式虽然有优点，但是每次调用GetInstance()静态方法时，必须判断NULL == m_instance，使程序相对开销增大。
2 多线程中会导致多个实例的产生，从而导致运行代码不正确以及内存的泄露。
3 提供释放资源的函数
*/

class Singelton {
private:
	//构造函数私有化
	Singelton() {
			cout << "Singelton构造函数执行begin" << endl;
			Sleep(1000); //线程睡觉
			cout << "Singelton构造函数执行end" << endl;
	}
public:
	/*
	懒汉式 只有调用该方法时才会new Singelton::getInstance()

	会存在多线程调用的问题，解决：线程同步+加锁+解锁
	Singelton *p1 = Singelton::getInstance(); 线程1
	Singelton *p2 = Singelton::getInstance(); 线程2

	Singelton() {
	cout << "Singelton构造函数执行begin" << endl;
	sleep(1000);
	cout << "Singelton构造函数执行end" << endl;
	}
	*/
	static Singelton *getSingelton() {
		if (NULL == m_psl) {
			count++;
			m_psl = new Singelton();
		}
		return m_psl;
	}

	static void freeInstance() {
		if (NULL != m_psl) {
			delete m_psl;
			m_psl = NULL;
		}
	}
	void pirntS() //测试函数
	{
		printf("Singelton printS test count:%d \n", count);
	}

private:
	static Singelton *m_psl;
	static int count;
};

//静态变量初始化
Singelton* Singelton::m_psl = NULL;
int Singelton::count = 0;

void main11() {

	Singelton *p1 = Singelton::getSingelton();
	Singelton *p2 = Singelton::getSingelton();

	if (p1 == p2) {
		cout << "同一个对象" << endl; //同一个对象
	}
	else {
		cout << "不是同一个对象" << endl;
	}

	Singelton::freeInstance();
}


void MyThreadFunc(void *) {
	// cout << "我是线程体..." << endl;
	cout << "我是线程体...\n";

	int id = GetCurrentThreadId();
	printf("\n threadfunc%d \n", id);

	Singelton::getSingelton()->pirntS();
	return;

}

int _tmain(int argc, _TCHAR argv[])
{
	//main11();
	HANDLE hThread[10];
	int threadnum = 3;

	for (int i = 0; i < 3; i++) {
		hThread[0] = (HANDLE)_beginthread(MyThreadFunc, 0, NULL);
	}

	//等待所有的子线程都运行完毕后,才执行这个代码
	for (int i = 0; i<threadnum; i++){
		WaitForSingleObject(hThread[i], INFINITE);
	}

	printf("等待线程结束\n");

	for (int i = 0; i<threadnum; i++){
		CloseHandle( hThread[i] );
	}
	Singelton::freeInstance();

	cin.get();
	return 0;
}

