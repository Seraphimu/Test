#include "Start.hpp"

//静态变量和全局变量都放在数据段，或者叫静态区中，要放在函数外面初始化。
unsigned int Student::count = 0;

void test(void) {
    //函数重载
    Student * newStudent = new Student("佐佐木淳平", 1919810);
    Student * newStudentNullParamater = new Student();

    //初始化

    //打印学生一的信息
    cout << "带参数的: \n";
    newStudent->showName();
    newStudent->showId();

    //打印学生二的信息
    cout << "不带参数的: \n";
    newStudentNullParamater->showName();
    newStudentNullParamater->showId();
    
    // 显示当前对象数量
    cout << "现在创建了几个对象: " << Student::returnCount() << "\n";



    //C++内存映像不同，数据放在不同的区域，所以未用指针创建的对象不能用delete
    delete newStudent;
    delete newStudentNullParamater;
    
    //删除后的对象个数
    cout << "现在创建了几个对象: " << Student::returnCount() << "\n";
    
}

//新测试例
void test_1(void) {
    PostGraduate * a = new PostGraduate("佐佐木淳平", 1919810, "本部", 24);

    cout << "姓名: "; a->showName();
    cout << "账号: "; a->showId();
    cout << "区域: "; a->showArea();
    cout << "年龄: "; a->showLevel();


    delete a;
}







//多态与虚函数
/*
    virtual关键字，虚函数，让父类可以访问子类的方法
    覆盖
    一个父类指针，可以动态指向不同的类
    重载是编译时决定的，多态是运行时决定的（内存
    虚函数表（表！！）（查表）
*/

//抽象类，纯虚函数
/*
至少有一个纯虚函数，依靠子类多态来实现
不能创建抽象类的对象了，但指针可以
virtual void study() = 0;   //设置之后变成抽象类。纯虚函数
学生类：抽象类
*/


//多态
//加法 vs 拼接
//重载（参数格式必须不一样）
//隐藏和覆盖（参数格式可以一样）
void testPoly() {
    PostGraduate * aPostGraduate = new PostGraduate("佐佐木淳平", 1919810, "本部", 2);
    PostGraduate * otherPostGraduate = new PostGraduate("田所浩二", 114514, "本部", 5);
    Student * aStudent = new Student("24岁事学生", 24);

    //定义一个指向父类Student的指针，并让其指向aStudent
    Student * ptrToStudent = aStudent;
    //定义一个指向子类PostGraduate的指针，并让其指向aPostGraduate
    PostGraduate * ptrToPostGraduate = aPostGraduate;

    //使用指针输出各个的信息
    cout << "父类的";
    ptrToStudent->showName(); ptrToStudent->showId();
    cout << "子类的";
    ptrToPostGraduate->showName(); ptrToPostGraduate->showId();
    ptrToPostGraduate->showArea(); ptrToPostGraduate->showLevel();


    //隐藏
    //子类能用父类的，父类不能用子类的……
    //继承：父类指针可指向子类对象，反之不行
    //即使父类指针可指向子类对象，但是不可调用子类方法和属性   
    //把指向父类的一个一个一个指针修改为一个一个一个指向子类的指针



    // 子类指针不可以指向父类
    // pPG = cStu;


}


int main(void) {
    // test();
    // test_1();
    testPoly();

    return 0;
}





