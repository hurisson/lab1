//           _____                    _____                   _____          
//          /\    \                  /\    \                 /\    \         
//         /::\    \                /::\    \               /::\    \        
//         \:::\    \              /::::\    \             /::::\    \       
//          \:::\    \            /::::::\    \           /::::::\    \      
//           \:::\    \          /:::/\:::\    \         /:::/\:::\    \     
//            \:::\    \        /:::/__\:::\    \       /:::/__\:::\    \    
//            /::::\    \      /::::\   \:::\    \      \:::\   \:::\    \   
//   ____    /::::::\    \    /::::::\   \:::\    \   ___\:::\   \:::\    \  
//  /\   \  /:::/\:::\    \  /:::/\:::\   \:::\____\ /\   \:::\   \:::\    \ 
// /::\   \/:::/  \:::\____\/:::/  \:::\   \:::|    /::\   \:::\   \:::\____\
// \:::\  /:::/    \::/    /\::/    \:::\  /:::|____\:::\   \:::\   \::/    /
//  \:::\/:::/    / \/____/  \/_____/\:::\/:::/    / \:::\   \:::\   \/____/ 
//   \::::::/    /                    \::::::/    /   \:::\   \:::\    \     
//    \::::/____/                      \::::/    /     \:::\   \:::\____\    
//     \:::\    \                       \::/____/       \:::\  /:::/    /    
//      \:::\    \                       ~~              \:::\/:::/    /     
//       \:::\    \                                       \::::::/    /      
//        \:::\____\                                       \::::/    /       
//         \::/    /                                        \::/    /        
//          \/____/                                          \/____/         

//#define OPT
#define POTOK
#define _USE_MATH_DEFINES 
#define PROGRAM_CYCLE 100


#include <iostream>
#include <cmath>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <string> 

using namespace std;
using namespace std::chrono;

double f(double x) { return (8 / (1 + pow(x, 2))); }
double I(double a, double b, int n, double y) { return ((b - a) / (2 * n) * y); }


int run_integral(int n) {




	//int p_cycle = 0;
	//while (p_cycle < PROGRAM_CYCLE) {


	double a, b, dy, In;
	double y = 0;
	a = 0;
	b = 1;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	if (n > 1) {
		dy = (b - a) / n;
		y += f(a) + f(b);


		 #ifdef OPT
					#pragma loop(ivdep)
		 			#pragma loop(hint_parallel(8))
		 #else
		 			#pragma loop(no_vector)
		 #endif
		for (int i = 1; i < n; i++) { y += 2 * (f(a + dy * i)); }
		In = I(a, b, n, y);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> duration = (t2 - t1);
		string ex = to_string(n);
		string filename = "diff_" + ex + ".txt";
		std::ofstream diff(filename, std::ios::app);
		if (diff.is_open())
		{
			diff << scientific;
			diff << setprecision(20);
			//diff << abs(2 * M_PI - In) << std::endl;
			diff << "The numerical value of the integral =  " << In << endl << endl;
			diff << "Duration is: " << duration.count() << " seconds" << endl << endl;
			diff << "Diff is: " << abs(2 * M_PI - In) << endl << endl;
		}
		else { cout << "Exit with err ==diff=="; }
		diff.close();

		//std::ofstream times("times.txt", std::ios::app);
		//if (times.is_open())
		//{
		//	times << scientific;
		//	times << setprecision(20);
		//	times << duration.count() << std::endl;
		//}
		//else { cout << "Exit with err ==times=="; }
		//times.close();


	}
	else { cout << "Exit with 0xFADE0001"; }
	//p_cycle++;

//}

	return 0;
}


int main() {

	remove("diff_100.txt");
	remove("diff_1000.txt");
	remove("diff_10000.txt");
	remove("diff_100000.txt");
	remove("diff_1000000.txt");

	int n;

	int splitting[] = { 100, 1000, 10000, 100000, 1000000 };

#ifndef POTOK
	high_resolution_clock::time_point boba = high_resolution_clock::now();
#ifdef OPT
#pragma loop(ivdep)
#pragma loop(hint_parallel(8))
#else
#pragma loop(no_vector)
#endif
	for (int i = 0; i < sizeof(splitting) / sizeof(int); i++) {
		n = splitting[i];


		run_integral(n);
	}
	high_resolution_clock::time_point biba = high_resolution_clock::now();
	duration<double> duration = (biba - boba);
	cout << "Duration for five loops is: " << duration.count() << " seconds" << endl << endl;
#else

	high_resolution_clock::time_point potok1 = high_resolution_clock::now();
	thread th1(run_integral, 100);
	thread th2(run_integral, 1000);
	thread th3(run_integral, 10000);
	thread th4(run_integral, 100000);
	thread th5(run_integral, 1000000);
	th1.join();
	th2.join();
	th3.join();
	th4.join();
	th5.join();
	high_resolution_clock::time_point potok5 = high_resolution_clock::now();
	duration<double> duration = (potok5 - potok1);
	cout << "Duration for five threads is: " << duration.count() << " seconds" << endl << endl;

#endif	

}