/*
by S-Hu
cpp作业1：分数类（UpDown）与sort函数
1.无/单/双参构造函数（简单的循环小数变分数）及operator=：myUD = 3.14
2.自动约分
3.UpDown间及与其他数据类型间的常用运算（+ - * / == != > < >= <= ++ -- [] += -= *= /=）
4.UpDown与double/int/bool之间的类型转换
5.Setter/Getter/Print函数、cout<<UpDown()
6.排序函数：void sort(T arr, int n, bool order = false), 其中arr为数组名(地址)，n为数组成员个数，order为排序顺序（为TRUE表示降序，默认升序）
#调试环境：Microsoft Visual Studio 2015
*/

#include<iostream>
using namespace std;

//选择排序
//void sort(T arr, int n, bool order = false), 其中arr为数组名(地址)，n为数组成员个数，order为排序顺序（为TRUE表示降序，默认升序）
template<class T>
void sort(T *arr, unsigned int n, bool order = false) {
	for (unsigned int iLoop = 0; iLoop < n; iLoop++) {
		for (unsigned int findBigger = iLoop; findBigger < n; findBigger++) {
			if ((arr[findBigger] < arr[iLoop]) ^ order) {//xor
				T temp = arr[iLoop];
				arr[iLoop] = arr[findBigger];
				arr[findBigger] = temp;
			}
		}
	}
}

//分数类
class UpDown {
private:
	int Up, Down;

	//取最大公约数
	int GCD(int a, int b) {
		if (a >= b) return a%b == 0 ? b : GCD(b, a%b);
		else return b%a == 0 ? a : GCD(a, b%a);
	}
	//标准化，化为符号在分子的既约假分数
	//在构造函数、赋值函数、自加自减函数中执行，确保所有对象都是标准化的
	void Standardize() {
		if (0 == Up) { Down = 1; return; }//处理0

										  //处理符号，使负号在分子
		if ((Up > 0 && Down < 0) || (Up < 0 && Down > 0)) { Up = Up < 0 ? Up : -Up; }
		else { Up = Up > 0 ? Up : -Up; }
		Down = Down > 0 ? Down : -Down;

		//约分
		int gcd = GCD(Up > 0 ? Up : -Up, Down);
		Up = Up / gcd;
		Down = Down / gcd;
	}
public:
	//无/单参构造函数
	//循环小数变分数（小数点后16位循环则认为是循环小数）
	UpDown(double u = 0) {
		//取为数组
		bool negative = u < 0 ? 1 : 0;
		double uAbs = u < 0 ? -u : u;
		int nums[17];//nums[0]为整数部分,nums[1~16]为小数点后16位
		for (int i = 0; i <= 16; i++) {
			nums[i] = int(uAbs);
			uAbs = (uAbs - int(uAbs)) * 10;
		}
		//寻找循环节
		bool isCirculator = 0;//找到循环节则置1
		int length, startLocation;//循环节长度、开始循环的位置
		for (length = 1; length <= 8; length++) {
			for (startLocation = 1; startLocation <= 18 - 2 * length; startLocation++) {
				isCirculator = 1;
				for (int getLoop = 0; getLoop <= length - 1; getLoop++) {
					if (nums[startLocation + getLoop] != nums[startLocation + length + getLoop]) {
						isCirculator = 0; break;
					}
				}
				if (isCirculator) { break; }
			}
			if (isCirculator) { break; }
		}
		//赋值
		if (isCirculator) { //找到循环节
			Up = 0; Down = 0;
			//循环节部分
			for (int iLoop = 0; iLoop <= length - 1; iLoop++) {
				Up = Up * 10 + nums[startLocation + iLoop];
				Down = Down * 10 + 9;
			}
			//非循环节部分
			for (int iLoop = 1; iLoop < startLocation; iLoop++) {
				Up += int(double(nums[startLocation - iLoop])*Down);
				Down *= 10;
			}
			Up += nums[0] * Down;//整数部分
			Up *= negative ? -1 : 1;//符号
		}
		else { //未找到循环节
			for (Down = 1; u != double(int(u)) && u < 1e8 && Down < 1e8; Down *= 10) { u *= 10; }
			Up = int(u);
		}
		Standardize();
	}

	//双参构造函数
	UpDown(double u, double d) {
		//cout << "UpDown构造函数被调用。" << endl;
		if (d != 0) {
			if (1 == d) { *this = UpDown(u); }
			else {
				//分子分母去小数点
				//防止溢出
				for (; ((u != double(int(u))) || (d != double(int(d)))) && u < 1e8 && d < 1e8;) {
					u *= 10; d *= 10;
				}
				Up = int(u); Down = int(d);
				Standardize();
			}
		}
		else {
			Up = 0; Down = 1;
			cerr << "Warning: The result is not valid (Divide By Zero)" << endl;
		}
	}

	//重载赋值运算符，可以myUD = 3.14;
	UpDown &operator=(double u) {
		*this = UpDown(u);
		return *this;
	}

	/*
	UpDown(UpDown& it){
	Up = it.getUp(); Down = it.getDown();
	cout << "UpDown拷贝构造函数被调用。" << endl; }
	~UpDown(){ cout << "UpDown析构函数被调用。" << endl; }
	*/

	//void Print(){ cout << Up << '/' << Down; }
	UpDown &Print() { cout << Up << '/' << Down; return *this; }

	//重载<<运算符，可以cout << UpDown();
	friend ostream &operator<<(ostream &os, UpDown &it) {
		os << it.Up << '/' << it.Down;
		return os;
	}

	//setter、getter函数
	//对赋值进行合法性检查
	//自动约分
	//单独更改分子分母用setUp/setDown，重新赋值用setVal
	UpDown &setUp(int u) { Up = u; Standardize(); return *this; }
	UpDown &setDown(int d) {
		if (d != 0) { Down = d; Standardize(); }
		else { cerr << "Warning: The result is not valid (Divide By Zero)" << endl; }
		return *this;
	}
	UpDown &setVal(double u) { *this = UpDown(u); return *this; }
	UpDown &setVal(double u, double d) { *this = UpDown(u, d); return *this; }
	int getUp() const { return Up; }
	int getDown() const { return Down; }

	//方括号运算符，[偶数]取分子，[奇数]取分母
	int operator[](int index) const {
		if (0 == index % 2) { return Up; }
		else { return Down; }
	}

	explicit operator double() const { return double(Up) / Down; }//重载double类型转换，explicit禁止自动转换，否则会自动建立UpDown与double的对应关系，造成冲突
	explicit operator int() const { return Up / Down; }//重载int类型转换，explicit禁止自动转换
	explicit operator bool() const { return 0 != Up; }

	//分数对分数的加减乘除运算
	UpDown operator+(const UpDown &rightVal) const {
		return(UpDown(this->Up*rightVal.Down + rightVal.Up*this->Down,
			this->Down*rightVal.Down));
	}
	UpDown operator-() const { return(UpDown(-Up, Down)); }//负号
	UpDown operator-(const UpDown &rightVal) const {
		return(UpDown(this->Up*rightVal.Down - rightVal.Up*this->Down,
			this->Down*rightVal.Down));
	}
	UpDown operator*(const UpDown &rightVal) const {
		return UpDown(this->Up*rightVal.Up, this->Down*rightVal.Down);
	}
	UpDown operator/(const UpDown &rightVal) const {
		return UpDown(this->Up*rightVal.Down, this->Down*rightVal.Up);
	}

	//分数的自加、自减运算
	UpDown &operator++() {//前置自加
		Up += Down;
		Standardize();
		return *this;
	}
	UpDown operator++(int) {//后置自加
		UpDown temp = *this;
		Up += Down;
		Standardize();
		return temp;
	}
	UpDown &operator--() {//前置自减
		Up -= Down;
		Standardize();
		return *this;
	}
	UpDown operator--(int) {//后置自减
		UpDown temp = *this;
		Up -= Down;
		Standardize();
		return temp;
	}

	//分数间的+= -= *= /=
	UpDown &operator+=(const UpDown &rightVal) {
		*this = *this + rightVal;
		return *this;
	}
	UpDown &operator-=(const UpDown &rightVal) {
		*this = *this - rightVal;
		return *this;
	}
	UpDown &operator*=(const UpDown &rightVal) {
		*this = *this * rightVal;
		return *this;
	}
	UpDown &operator/=(const UpDown &rightVal) {
		*this = *this / rightVal;
		return *this;
	}

	//分数间的关系运算
	bool operator==(const UpDown &it) const { return (this->Up == it.Up) && (this->Down == it.Down); }
	bool operator!=(const UpDown &it) const { return !(*this == it); }
	bool operator<(const UpDown &it) const { return (*this - it).Up < 0; }
	bool operator>(const UpDown &it) const { return (*this - it).Up > 0; }
	bool operator<=(const UpDown &it) const { return (*this - it).Up <= 0; }
	bool operator>=(const UpDown &it) const { return (*this - it).Up >= 0; }

	//分数对其他类型的运算
	UpDown operator+(double rightVal) const { return *this + UpDown(rightVal); }
	UpDown operator-(double rightVal) const { return *this - UpDown(rightVal); }
	UpDown operator*(double rightVal) const { return *this * UpDown(rightVal); }
	UpDown operator/(double rightVal) const { return *this / UpDown(rightVal); }
	UpDown &operator+=(double rightVal) {
		*this = *this + rightVal;
		return *this;
	}
	UpDown &operator-=(double rightVal) {
		*this = *this - rightVal;
		return *this;
	}
	UpDown &operator*=(double rightVal) {
		*this = *this * rightVal;
		return *this;
	}
	UpDown &operator/=(double rightVal) {
		*this = *this / rightVal;
		return *this;
	}
	bool operator==(double rightVal) const { return double(*this) == rightVal; }
	bool operator!=(double rightVal) const { return double(*this) != rightVal; }
	bool operator<(double rightVal) const { return double(*this) < rightVal; }
	bool operator>(double rightVal) const { return double(*this) > rightVal; }
	bool operator<=(double rightVal) const { return double(*this) <= rightVal; }
	bool operator>=(double rightVal) const { return double(*this) >= rightVal; }

	//其他类型对分数的运算
	friend UpDown operator+(double leftVal, const UpDown &rightVal) { return UpDown(leftVal) + rightVal; }
	friend UpDown operator-(double leftVal, const UpDown &rightVal) { return UpDown(leftVal) - rightVal; }
	friend UpDown operator*(double leftVal, const UpDown &rightVal) { return UpDown(leftVal) * rightVal; }
	friend UpDown operator/(double leftVal, const UpDown &rightVal) { return UpDown(leftVal) / rightVal; }
	friend double &operator+=(double &leftVal, const UpDown &rightVal) {
		leftVal = leftVal + double(rightVal);
		return leftVal;
	}
	friend double &operator-=(double &leftVal, const UpDown &rightVal) {
		leftVal = leftVal - double(rightVal);
		return leftVal;
	}
	friend double &operator*=(double &leftVal, const UpDown &rightVal) {
		leftVal = leftVal * double(rightVal);
		return leftVal;
	}
	friend double &operator/=(double &leftVal, const UpDown &rightVal) {
		leftVal = leftVal / double(rightVal);
		return leftVal;
	}
	friend bool operator==(double leftVal, const UpDown &rightVal) { return leftVal == double(rightVal); }
	friend bool operator!=(double leftVal, const UpDown &rightVal) { return leftVal != double(rightVal); }
	friend bool operator>(double leftVal, const UpDown &rightVal) { return leftVal > double(rightVal); }
	friend bool operator<(double leftVal, const UpDown &rightVal) { return leftVal < double(rightVal); }
	friend bool operator>=(double leftVal, const UpDown &rightVal) { return leftVal >= double(rightVal); }
	friend bool operator<=(double leftVal, const UpDown &rightVal) { return leftVal <= double(rightVal); }
};

int main() {
	system("color F2&title class UpDown (by 胡顺昕 1400015962)");

	cout << "1.构造函数示例：" << endl;
	cout << "UpDown() = " << UpDown() << endl;
	cout << "UpDown(-3.14) = " << UpDown(-3.14) << endl;
	cout << "UpDown(2.0/3) = " << UpDown(2.0 / 3) << endl;
	cout << "UpDown(2, -3) = " << UpDown(2, -3) << endl;
	cout << "Division by zero: UpDown(2, 0)" << endl; UpDown(2, 0);

	cout << endl << "2.运算符示例：" << endl;
	cout << "UpDown(3, -4) + UpDown(2, 3) = " << (UpDown(3, -4) + UpDown(2, 3)) << endl;
	cout << "++UpDown(-3, 4) = " << ++UpDown(-3, 4) << endl;
	cout << "UpDown(3, 5) - 3 = " << (UpDown(3, 5) - 3) << endl;
	cout << "2.4 * -UpDown(3, 5) = " << (2.4 * -UpDown(3, 5)) << endl;
	cout << "UpDown(3, 5) / UpDown(0, 1)" << endl; UpDown(3, 5) / UpDown(0, 1);
	cout << "UpDown(2, 3)[0] = " << UpDown(2, 3)[0] << endl;
	cout << "UpDown(3, 4) > UpDown(2, 3) = " << (UpDown(3, 4) > UpDown(2, 3)) << endl;
	UpDown myUD;
	myUD = 2.5;//赋值
	cout << myUD << '\t';
	myUD -= 1.0 / 3;
	myUD.Print();

	cout << endl << endl << "3.类型转换示例：" << endl;
	cout << "double(UpDown(3, 2)) = " << double(UpDown(3, 2)) << endl;
	cout << "int(UpDown(3, 2)) = " << int(UpDown(3, 2)) << endl;
	if (UpDown(2, 5) - 0.4) cout << 1; else cout << 0;

	cout << endl << endl << "4.setter/getter/print函数示例：UpDown(2, 3).setDown(-7).Print()" << endl;
	UpDown(2, 3).setDown(-7).Print();

	cout << endl << endl << "5.sort函数示例：" << "UpDown arr[]{1.0/3, -2, 2.4, UpDown(4,3), 0}" << endl << "排序前：";
	UpDown arr[]{ 1.0 / 3, -2, 2.4, UpDown(4, 3), 0 };
	unsigned int dataCount = sizeof(arr) / sizeof(arr[0]);
	for (unsigned int iLoop = 0; iLoop < dataCount; ++iLoop) { cout << arr[iLoop] << '\t'; }
	cout << endl << "升序  ：";
	sort(arr, dataCount);
	for (unsigned int iLoop = 0; iLoop < dataCount; ++iLoop) { cout << arr[iLoop] << '\t'; }
	cout << endl << "降序  ：";
	sort(arr, dataCount, true);
	for (unsigned int iLoop = 0; iLoop < dataCount; ++iLoop) { cout << arr[iLoop] << '\t'; }

	system("echo.&pause>nul");
	return 0;
}
