/*
 * C++ Programming, Assignment 1
 * UpDown class & sort functions
 * by S.Hu
 *
 * 1.无/单/双参构造函数（简单的循环小数变分数）及operator=
 * 2.自动约分
 * 3.UpDown间及与其他数据类型间的常用运算（+ - * / == != > < >= <= ++ -- [] += -= *= /=）
 * 4.UpDown与double/int/bool之间的类型转换
 * 5.Setor/Getor/Printer函数、流插入（<<）operaotor
 * 6.排序函数
 * IDE：Microsoft Visual Studio 2015
 */
#pragma warning(disable:4290)//忽略C++异常规范，但指示函数不是__declspec(nothrow)
#include <iostream>
#include <stdexcept>
using namespace std;

/* sort functions */
template<typename Iterator, typename Comparator>
void sort(Iterator begin, Iterator end, Comparator comp) {
	for (Iterator iter = begin; iter < end; ++iter)
		for (Iterator finder = iter; finder < end; ++finder) {
			if (comp(*finder, *iter)) {
				auto temp = *iter;
				*iter = *finder;
				*finder = temp;
			}
		}
}

template<typename Iterator>
void sort(Iterator begin, Iterator end) {
	for (Iterator iter = begin; iter < end; ++iter)
		for (Iterator finder = iter; finder < end; ++finder) {
			if (*finder < *iter) {
				auto temp = *iter;
				*iter = *finder;
				*finder = temp;
			}
		}
}

//分数类
class UpDown {
private:
	int Up, Down;

	//取最大公约数
	int GCD(int a, int b) {
		if (a >= b) return a % b == 0 ? b : GCD(b, a % b);
		else return b % a == 0 ? a : GCD(a, b % a);
	}
	//标准化，化为符号在分子的既约假分数
	//在构造函数、赋值函数、自加自减函数中执行，确保所有对象都是标准化的
	void Standardize() {
		if (0 == Up) {
			Down = 1;    //处理0
			return;
		}

		//处理符号，使负号在分子
		if ((Up > 0 && Down < 0) || (Up < 0 && Down > 0))
			Up = Up < 0 ? Up : -Up;
		else
			Up = Up > 0 ? Up : -Up;

		Down = Down > 0 ? Down : -Down;
		//约分
		int gcd = GCD(Up > 0 ? Up : -Up, Down);
		Up = Up / gcd;
		Down = Down / gcd;
	}
public:
	/* constructors & assignment operator */
	//无/单参构造函数
	//循环小数变分数（小数点后16位循环则认为是循环小数）
	UpDown(double u = 0.0) {
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
						isCirculator = 0;
						break;
					}
				}

				if (isCirculator)
					break;
			}

			if (isCirculator)
				break;
		}

		//赋值
		if (isCirculator) { //找到循环节
			Up = 0;
			Down = 0;

			//循环节部分
			for (int iLoop = 0; iLoop <= length - 1; iLoop++) {
				Up = Up * 10 + nums[startLocation + iLoop];
				Down = Down * 10 + 9;
			}

			//非循环节部分
			for (int iLoop = 1; iLoop < startLocation; iLoop++) {
				Up += int(double(nums[startLocation - iLoop]) * Down);
				Down *= 10;
			}

			Up += nums[0] * Down;//整数部分
			Up *= negative ? -1 : 1;//符号
		} else { //未找到循环节
			for (Down = 1; u != double(int(u)) && u < 1e8 && Down < 1e8; Down *= 10)
				u *= 10;

			Up = int(u);
		}

		Standardize();
	}

	//双参构造函数
	UpDown(int u, int d)throw(logic_error) {
		if (d == 0) throw logic_error("Divide By Zero");
		else {
			Up = u;
			Down = d;
			Standardize();
		}
	}

	//assignment operator
	UpDown &operator=(double u) {
		*this = UpDown(u);
		return *this;
	}

	//流插入运算符
	friend ostream &operator<<(ostream &os, UpDown &it) {
		os << it.Up << '/' << it.Down;
		return os;
	}

	//setor, getor, printer
	UpDown &setUp(int u) {
		Up = u;
		Standardize();
		return *this;
	}
	UpDown &setDown(int d)throw(logic_error) {
		if (d == 0) throw logic_error("Divide By Zero");
		else {
			Down = d;
			Standardize();
		}

		return *this;
	}
	UpDown &setVal(double u) {
		*this = UpDown(u);
		return *this;
	}
	UpDown &setVal(int u, int d)throw(logic_error) {
		*this = UpDown(u, d);
		return *this;
	}
	int getUp() const {
		return Up;
	}
	int getDown() const {
		return Down;
	}
	UpDown &Print() {
		cout << Up << '/' << Down;
		return *this;
	}

	//[偶数]取分子，[奇数]取分母
	int operator[](int index) const {
		if (0 == index % 2)
			return Up;
		else
			return Down;
	}

	//(explicit) casters
	explicit operator double() const {
		return double(Up) / Down;
	}
	explicit operator int() const {
		return Up / Down;
	}
	explicit operator bool() const {
		return 0 != Up;
	}

	//+ - * / UpDown to UpDown
	UpDown operator+(const UpDown &rightVal) const {
		return (UpDown(this->Up * rightVal.Down + rightVal.Up * this->Down,
		               this->Down * rightVal.Down));
	}
	UpDown operator-() const {
		return (UpDown(-Up, Down));    //negative
	}
	UpDown operator-(const UpDown &rightVal) const {
		return (UpDown(this->Up * rightVal.Down - rightVal.Up * this->Down,
		               this->Down * rightVal.Down));
	}
	UpDown operator*(const UpDown &rightVal) const {
		return UpDown(this->Up * rightVal.Up, this->Down * rightVal.Down);
	}
	UpDown operator/(const UpDown &rightVal) const {
		return UpDown(this->Up * rightVal.Down, this->Down * rightVal.Up);
	}

	//++ --
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

	//+= -= *= /= between UpDowns
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

	//logical operators
	bool operator==(const UpDown &it) const {
		return (this->Up == it.Up) && (this->Down == it.Down);
	}
	bool operator!=(const UpDown &it) const {
		return !(*this == it);
	}
	bool operator<(const UpDown &it) const {
		return (*this - it).Up < 0;
	}
	bool operator>(const UpDown &it) const {
		return (*this - it).Up > 0;
	}
	bool operator<=(const UpDown &it) const {
		return (*this - it).Up <= 0;
	}
	bool operator>=(const UpDown &it) const {
		return (*this - it).Up >= 0;
	}

	//UpDown to double
	UpDown operator+(double rightVal) const {
		return *this + UpDown(rightVal);
	}
	UpDown operator-(double rightVal) const {
		return *this - UpDown(rightVal);
	}
	UpDown operator*(double rightVal) const {
		return *this * UpDown(rightVal);
	}
	UpDown operator/(double rightVal) const {
		return *this / UpDown(rightVal);
	}
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
	bool operator==(double rightVal) const {
		return double(*this) == rightVal;
	}
	bool operator!=(double rightVal) const {
		return double(*this) != rightVal;
	}
	bool operator<(double rightVal) const {
		return double(*this) < rightVal;
	}
	bool operator>(double rightVal) const {
		return double(*this) > rightVal;
	}
	bool operator<=(double rightVal) const {
		return double(*this) <= rightVal;
	}
	bool operator>=(double rightVal) const {
		return double(*this) >= rightVal;
	}

	//double to UpDown
	friend UpDown operator+(double leftVal, const UpDown &rightVal) {
		return UpDown(leftVal) + rightVal;
	}
	friend UpDown operator-(double leftVal, const UpDown &rightVal) {
		return UpDown(leftVal) - rightVal;
	}
	friend UpDown operator*(double leftVal, const UpDown &rightVal) {
		return UpDown(leftVal) * rightVal;
	}
	friend UpDown operator/(double leftVal, const UpDown &rightVal) {
		return UpDown(leftVal) / rightVal;
	}
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
	friend bool operator==(double leftVal, const UpDown &rightVal) {
		return leftVal == double(rightVal);
	}
	friend bool operator!=(double leftVal, const UpDown &rightVal) {
		return leftVal != double(rightVal);
	}
	friend bool operator>(double leftVal, const UpDown &rightVal) {
		return leftVal > double(rightVal);
	}
	friend bool operator<(double leftVal, const UpDown &rightVal) {
		return leftVal < double(rightVal);
	}
	friend bool operator>=(double leftVal, const UpDown &rightVal) {
		return leftVal >= double(rightVal);
	}
	friend bool operator<=(double leftVal, const UpDown &rightVal) {
		return leftVal <= double(rightVal);
	}
};

int main() {
	system("color F2&title class UpDown (by S.Hu)");
	cout << "1.构造函数示例：" << endl;
	cout << "UpDown() = " << UpDown() << endl;
	cout << "UpDown(-3.14) = " << UpDown(-3.14) << endl;
	cout << "UpDown(2.0/3) = " << UpDown(2.0 / 3) << endl;
	cout << "UpDown(2, -3) = " << UpDown(2, -3) << endl;
	cout << "测试异常: UpDown(2, 0)" << endl;
	try {
		UpDown(2, 0);
	} catch (logic_error e) {
		cerr << e.what() << endl;
	}

	cout << endl << "2.运算符示例：" << endl;
	cout << "UpDown(3, -4) + UpDown(2, 3) = " << (UpDown(3, -4) + UpDown(2, 3)) << endl;
	cout << "++UpDown(-3, 4) = " << ++UpDown(-3, 4) << endl;
	cout << "UpDown(3, 5) - 3 = " << (UpDown(3, 5) - 3) << endl;
	cout << "2.4 * -UpDown(3, 5) = " << (2.4 * -UpDown(3, 5)) << endl;
	cout << "测试异常: UpDown(3, 5) / UpDown(0, 1)" << endl;
	try {
		UpDown(3, 5) / UpDown(0, 1);
	} catch (logic_error e) {
		cerr << e.what() << endl;
	}
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
	if (UpDown(2, 5)) cout << 1;
	else cout << 0;

	cout << endl << endl << "4.setor/getor/printer示例：UpDown(2, 3).setDown(-7).Print()" << endl;
	UpDown(2, 3).setDown(-7).Print();

	cout << endl << endl << "5.sort函数示例：" << "UpDown arr[]{1.0/3, -2, 2.4, UpDown(4,3), 0}" << endl << "排序前：";
	UpDown arr[] { 1.0 / 3, -2, 2.4, UpDown(4, 3), 0 };
	unsigned int dataCount = sizeof(arr) / sizeof(arr[0]);
	for (unsigned int iLoop = 0; iLoop < dataCount; ++iLoop)
		cout << arr[iLoop] << '\t';

	cout << endl << "升序  ：";
	sort(arr, arr + dataCount);
	for (unsigned int iLoop = 0; iLoop < dataCount; ++iLoop)
		cout << arr[iLoop] << '\t';

	cout << endl << "降序  ：";
	sort(arr, arr + dataCount, [](UpDown x, UpDown y) ->bool {return x > y;});
	for (unsigned int iLoop = 0; iLoop < dataCount; ++iLoop)
		cout << arr[iLoop] << '\t';

	system("echo.&pause>nul");
	return 0;
}
