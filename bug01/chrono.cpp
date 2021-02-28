#include <iostream>
#include <string>
#include <chrono>

using namespace std;

int main() {
    //距离时间戳2两秒
    chrono::time_point<chrono::system_clock, chrono::seconds> tp(chrono::seconds(2));
    cout << "to epoch : " <<tp.time_since_epoch().count() << "s" <<endl;
    //转化为ctime，打印输出时间点
    time_t tt = chrono::system_clock::to_time_t(tp);
    cout << tt << endl;
    
    cout << "system clock          : ";
	cout << chrono::system_clock::period::num << "/" << chrono::system_clock::period::den << "s" << endl;
	cout << "steady clock          : ";
	cout << chrono::steady_clock::period::num << "/" << chrono::steady_clock::period::den << "s" << endl;
	cout << "high resolution clock : ";
	cout << chrono::high_resolution_clock::period::num << "/" << chrono::high_resolution_clock::period::den << "s" << endl;

    typedef chrono::time_point<chrono::system_clock, chrono::milliseconds> microClock_type;

	microClock_type tp1 = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now());

	tt = chrono::system_clock::to_time_t(tp1);

	cout << "to 1970-1-1,00:00  " << tp1.time_since_epoch().count() << "ms" << endl;


    return 0;
   
}