#include <string>
#include <iostream>
using namespace std;

//类、类型
//类的定义、使用
class Student {
    //默认对象所有对象为private访问类型
    //public访问类型是所有对象都可以访问
    //private只能被本类的对象访问
    //protected和private在没有继承时的作用相同，但是在继承时，protected可被继承，private不行
protected:
    string name;
    int id;
    //静态成员：static
    //与class有关，与class的对象无关的成员
    //统计创建了多少个对象：可在构造函数中++一个类中的静态变量
    //定义时不写static，在类中声名时需要写
    //a.count(), b.count, Student::count()结果一样
    //静态函数      限定类::对象写法
public:
    //成员函数/方法
    //构造函数：初始化操作
    //1. 与类名相同
    //2. 没有返回类型和返回值
    static unsigned int count;
    Student(string name, int id);

    //函数重载：同一个函数名，用不同的参数来识别调用
    //本函数的参数与上面的函数同名而不同参数，调用时系统会根据参数来调用特定的函数
    Student();

    //析构函数比构造函数多个~
    //内存释放？？delete调用析构函数，用指针创建对象时可用。
    ~Student();

    //常成员函数：只读取，不修改的函数
    void showName() const;
    void showId() const;
    static unsigned int returnCount();
};





//继承
//子类构造函数
//先调用父类的构造函数，再调用子类本身的构造函数
//son::son():father

//不加public继承不了public的内容
//什么都不加的就是默认private类型继承
class PostGraduate : public Student {
// class PostGraduate : Student {
protected:
    string area;    //校区名称
    int level;      //年级
public:
    void showArea();
    void showLevel();
    // PostGraduate(string area, int age);
    PostGraduate(string name, int id, string area, int level);
};