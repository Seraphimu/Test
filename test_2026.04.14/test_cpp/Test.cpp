// /**
//  * 复习一下CPP的语法
//  */






// /**
//  * static成员
//  */
// #include <iostream>

// class Date {
// public:
//     Date(int year = 1145, int month = 1, int day = 4) :
//     year(year), month(month), day(day)
//     {
//         cnt++;
//     }

//     static int getCount() {
//         return cnt;
//     }

//     void print() {
//         printf("y:%d m:%d d:%d\n", 
//             this->year, this->month, this->day);
//     }
//     void showPtrIn() {
//         printf("&year = %p\n", &year);
//         printf("this = %p\n", this);
//         printf("&month = %p\n", &month);
//         printf("&day = %p\n", &day);
//         printf("&cnt = %p\n", &cnt);
//     }
// private:
//     int year = 2015;
//     int month = 2;
//     int day = 1;
//     // a member with an in-class initializer must be const
//     // static int cnt = 1;
//     static int cnt;
// };

// //私有变量, 在静态下也能在外部声明, 那么C++的地址空排一定有讲究.

// /**
//  *  vivit@Xen:test_cpp$ ./a.out 
//     y:1145 m:1 d:4
//     &year = 0x7fff17e5d754
//     &month = 0x7fff17e5d758
//     &day = 0x7fff17e5d75c
//     &cnt = 0x64761bca301c
//     明显不在一块

//     两种分配方式打印出来的地址相关极远! 一个用栈, 一个用堆.
//     用堆的差不多, 用栈得差很远!
//     应该和两种分配方式有关.


//     &year = 0x7ffdcfa1d8f4
// this = 0x7ffdcfa1d8f4
// &month = 0x7ffdcfa1d8f8
// &day = 0x7ffdcfa1d8fc
// &cnt = 0x5f5ca1ca101c
// d = 0x7ffdcfa1d8f4
// &getCount = 0x5f5ca1c9e204

// 结论: this指向被实例化的对象!
//  */
// int Date::cnt = 0;

// void showPtr(const Date * d) {
//     printf("d = %p\n", d);
//     printf("&getCount = %p\n", &(d->getCount));
// }

// int main(void) {
//     Date d;
//     d.showPtrIn();
//     showPtr(&d);

//     // Date * pd = new Date;
//     // pd->showPtrIn();
//     // showPtr(pd);

//     // delete pd;

//     // printf("&showPtr = %p\n", &showPtr);

//     return 0;
// }



//  /**
//   * 基于范围的for循环, 类似java中的foreach
//   * 
//   * ivit@Xen:test_cpp$ ./a.out 
//     &e = 0x7fff9add2670
//     &e = 0x7fff9add2674
//     &e = 0x7fff9add2678
//     &e = 0x7fff9add267c
//     &e = 0x7fff9add2680
//     &e = 0x7fff9add266c
//     &e = 0x7fff9add266c
//     &e = 0x7fff9add266c
//     &e = 0x7fff9add266c
//     &e = 0x7fff9add266c
//     arr = 0x7fff9add2670

//     结论: 
//         带引用, 相当于直接对变量做操作, auto & e = arr[i]
//         不带引用, 相当于auto e = arr[i], 使用的是同一个变量来拷贝arr[i]的值
//     单进程, 单线程, 只有一份进程地址空间, 凭这一点, 足够证明了!
//   */
// #include <iostream>

// int main(void) {
//     int arr[5];

//     int i = 0;
//     //这里不带引用写不进去
//     for(auto & e: arr) {
//         e = i++;
//         printf("&e = %p\n", &e);
//     }
    
//     for (auto e: arr) {
//         printf("&e = %p\n", &e);
//         // std::cout << "e = " << e << "\n";
//     }
//     printf("arr = %p\n", arr);

//     return 0;
// }

















// /**
//  * 引用
//  * 引用从来开辟过内存空间, 它只对被引用的变量起了一个别名, 
//  * 对引用做操作, 就等于对被引用的变量做操作, 它们都在相同地址中
//  */
// #include <iostream>

// void add(int & a) {
//     a+=10;
// }

// int main(void) {
//     int a = 10;
//     printf("a = %d, &a = %p\n", a, &a);
//     int & ref_a = a;
//     printf("ref_a = %d, &ref_a = %p\n", ref_a, &ref_a);

//     add(a);
//     printf("a = %d, &a = %p\n", a, &a);
//     printf("ref_a = %d, &ref_a = %p\n", ref_a, &ref_a);

//     return 0;
// }






// /**
//  * auto关键字
//  * 自动推导类型, 定义时必须被初始化, 那么它就一定在编译之前就完成了
//  * 一定类似于template<>这样的特性, 是让编译器来帮助用户写代码.
//  * 调试器见! 调试器看不出来啊! 猜错力(悲)
//  * auto不能作为函数的参数
//     auto不能直接用了声明数组

// g++ -g Test.cpp -o a.out -std=c++11
// Test.cpp:39:11: warning: use of ‘auto’ in parameter declaration only available with ‘-std=c++20’ or ‘-fconcepts’
//    39 | auto test(auto & a) {
//       |           ^~~~
// Test.cpp:39:1: error: ‘test’ function uses ‘auto’ type specifier without trailing return type
//    39 | auto test(auto & a) {
//       | ^~~~
// Test.cpp:39:1: note: deduced return type only available with ‘-std=c++14’ or ‘-std=gnu++14’
// make: *** [makefile:2: a.out] Error 1

// Test.cpp:54:1: error: declaration of ‘auto num [10]’ has no initializer
//    54 | auto num[10];
//       | ^~~~
// make: *** [makefile:2: a.out] Error 1
//  */

// auto test(auto & a) {
//     a = a + 1145;
//     return a;
// }

// // auto num[10];
// #include <iostream>
// int main(void) {
//     //一次定义多个变量时, auto会根据第一个变量的类型来推导, 
//     //后面的变量类型不同. 编译器: 不是我喜欢的类型, 直接报错!
//     // auto a = 10, b = 1.1;
//     auto a = 10;
//     auto & ref_a = a;
//     auto * ptr_a = &a;

//     /**
//      *  vivit@Xen:test_cpp$ ./a.out 
//         类型: i, a = 10
//         类型: i, ref_a = 10
//         类型: Pi, ptr_a = 0x7fff67033f2c
//      */
//     std::cout << "类型: " << typeid(a).name() << ", a = " << a << "\n";
//     std::cout << "类型: " << typeid(ref_a).name() << ", ref_a = " << ref_a << "\n";
//     std::cout << "类型: " << typeid(ptr_a).name() << ", ptr_a = " << ptr_a << "\n";
    
//     test(a);

//     std::cout << "a = " << a << "\n";

//     return 0;
// }

