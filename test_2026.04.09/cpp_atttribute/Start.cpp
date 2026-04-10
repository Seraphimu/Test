#include "Start.hpp"

//函数实现
Student::Student(string name, int id) {
    //this关键字是指向当前类的一个指针，用this->name表示的是本类的对象name，而不是本构造函数的参数
    this->name = name;
    this->id = id;
    count++;
}

Student::Student() {
    name = "田所浩二";
    id = 114514;
    count++;
}

// 析构函数的实现
Student::~Student() {
    count--;
    cout << "对象已删除.\n";
}

//函数实现，不带const会报错，一定声明清楚
void Student::showName() const {
    cout << name << "\n";
}

void Student::showId() const {
    cout << id << "\n";
}

//和const的用法分开一些，这个static不用加上
// static void Student::showCount() {
unsigned int Student::returnCount() {
    return count;
}

//在子类中的构造函数把参数写好，包含父类构造函数的参数，然后传给父类构造函数。
//执行顺序是先执行子类的，后执行父类的。
PostGraduate::PostGraduate(string name, int id, string area, int level) : Student(name, id){
    this->area = area;
    this->level = level;
}


void PostGraduate::showArea() {
    cout << "校区在: " << area << "\n";
}

void PostGraduate::showLevel() {
    cout << "年级是: " << level << "\n";
}
