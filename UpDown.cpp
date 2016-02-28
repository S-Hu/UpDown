/*
1400015962 �⻪����ѧԺ ��˳꿣�˫ѧλ��
cpp��ҵ1�������ࣨUpDown����sort����
1.��/��/˫�ι��캯�����򵥵�ѭ��С�����������operator=��myUD = 3.14
2.�Զ�Լ��
3.UpDown�估�������������ͼ�ĳ������㣨+ - * / == != > < >= <= ++ -- [] += -= *= /=��
4.UpDown��double/int/bool֮�������ת��
5.Setter/Getter/Print������cout<<UpDown()
6.��������void sort(T arr, int n, bool order = false), ����arrΪ������(��ַ)��nΪ�����Ա������orderΪ����˳��ΪTRUE��ʾ����Ĭ������
#���Ի�����Microsoft Visual Studio 2015
*/

#include<iostream>
using namespace std;

//ѡ������
//void sort(T arr, int n, bool order = false), ����arrΪ������(��ַ)��nΪ�����Ա������orderΪ����˳��ΪTRUE��ʾ����Ĭ������
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

//������
class UpDown {
private:
	int Up, Down;

	//ȡ���Լ��
	int GCD(int a, int b) {
		if (a >= b) return a%b == 0 ? b : GCD(b, a%b);
		else return b%a == 0 ? a : GCD(a, b%a);
	}
	//��׼������Ϊ�����ڷ��ӵļ�Լ�ٷ���
	//�ڹ��캯������ֵ�������Լ��Լ�������ִ�У�ȷ�����ж����Ǳ�׼����
	void Standardize() {
		if (0 == Up) { Down = 1; return; }//����0

										  //������ţ�ʹ�����ڷ���
		if ((Up > 0 && Down < 0) || (Up < 0 && Down > 0)) { Up = Up < 0 ? Up : -Up; }
		else { Up = Up > 0 ? Up : -Up; }
		Down = Down > 0 ? Down : -Down;

		//Լ��
		int gcd = GCD(Up > 0 ? Up : -Up, Down);
		Up = Up / gcd;
		Down = Down / gcd;
	}
public:
	//��/���ι��캯��
	//ѭ��С���������С�����16λѭ������Ϊ��ѭ��С����
	UpDown(double u = 0) {
		//ȡΪ����
		bool negative = u < 0 ? 1 : 0;
		double uAbs = u < 0 ? -u : u;
		int nums[17];//nums[0]Ϊ��������,nums[1~16]ΪС�����16λ
		for (int i = 0; i <= 16; i++) {
			nums[i] = int(uAbs);
			uAbs = (uAbs - int(uAbs)) * 10;
		}
		//Ѱ��ѭ����
		bool isCirculator = 0;//�ҵ�ѭ��������1
		int length, startLocation;//ѭ���ڳ��ȡ���ʼѭ����λ��
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
		//��ֵ
		if (isCirculator) { //�ҵ�ѭ����
			Up = 0; Down = 0;
			//ѭ���ڲ���
			for (int iLoop = 0; iLoop <= length - 1; iLoop++) {
				Up = Up * 10 + nums[startLocation + iLoop];
				Down = Down * 10 + 9;
			}
			//��ѭ���ڲ���
			for (int iLoop = 1; iLoop < startLocation; iLoop++) {
				Up += int(double(nums[startLocation - iLoop])*Down);
				Down *= 10;
			}
			Up += nums[0] * Down;//��������
			Up *= negative ? -1 : 1;//����
		}
		else { //δ�ҵ�ѭ����
			for (Down = 1; u != double(int(u)) && u < 1e8 && Down < 1e8; Down *= 10) { u *= 10; }
			Up = int(u);
		}
		Standardize();
	}

	//˫�ι��캯��
	UpDown(double u, double d) {
		//cout << "UpDown���캯�������á�" << endl;
		if (d != 0) {
			if (1 == d) { *this = UpDown(u); }
			else {
				//���ӷ�ĸȥС����
				//��ֹ���
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

	//���ظ�ֵ�����������myUD = 3.14;
	UpDown &operator=(double u) {
		*this = UpDown(u);
		return *this;
	}

	/*
	UpDown(UpDown& it){
	Up = it.getUp(); Down = it.getDown();
	cout << "UpDown�������캯�������á�" << endl; }
	~UpDown(){ cout << "UpDown�������������á�" << endl; }
	*/

	//void Print(){ cout << Up << '/' << Down; }
	UpDown &Print() { cout << Up << '/' << Down; return *this; }

	//����<<�����������cout << UpDown();
	friend ostream &operator<<(ostream &os, UpDown &it) {
		os << it.Up << '/' << it.Down;
		return os;
	}

	//setter��getter����
	//�Ը�ֵ���кϷ��Լ��
	//�Զ�Լ��
	//�������ķ��ӷ�ĸ��setUp/setDown�����¸�ֵ��setVal
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

	//�������������[ż��]ȡ���ӣ�[����]ȡ��ĸ
	int operator[](int index) const {
		if (0 == index % 2) { return Up; }
		else { return Down; }
	}

	explicit operator double() const { return double(Up) / Down; }//����double����ת����explicit��ֹ�Զ�ת����������Զ�����UpDown��double�Ķ�Ӧ��ϵ����ɳ�ͻ
	explicit operator int() const { return Up / Down; }//����int����ת����explicit��ֹ�Զ�ת��
	explicit operator bool() const { return 0 != Up; }

	//�����Է����ļӼ��˳�����
	UpDown operator+(const UpDown &rightVal) const {
		return(UpDown(this->Up*rightVal.Down + rightVal.Up*this->Down,
			this->Down*rightVal.Down));
	}
	UpDown operator-() const { return(UpDown(-Up, Down)); }//����
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

	//�������Լӡ��Լ�����
	UpDown &operator++() {//ǰ���Լ�
		Up += Down;
		Standardize();
		return *this;
	}
	UpDown operator++(int) {//�����Լ�
		UpDown temp = *this;
		Up += Down;
		Standardize();
		return temp;
	}
	UpDown &operator--() {//ǰ���Լ�
		Up -= Down;
		Standardize();
		return *this;
	}
	UpDown operator--(int) {//�����Լ�
		UpDown temp = *this;
		Up -= Down;
		Standardize();
		return temp;
	}

	//�������+= -= *= /=
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

	//������Ĺ�ϵ����
	bool operator==(const UpDown &it) const { return (this->Up == it.Up) && (this->Down == it.Down); }
	bool operator!=(const UpDown &it) const { return !(*this == it); }
	bool operator<(const UpDown &it) const { return (*this - it).Up < 0; }
	bool operator>(const UpDown &it) const { return (*this - it).Up > 0; }
	bool operator<=(const UpDown &it) const { return (*this - it).Up <= 0; }
	bool operator>=(const UpDown &it) const { return (*this - it).Up >= 0; }

	//�������������͵�����
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

	//�������ͶԷ���������
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
	system("color F2&title class UpDown (by ��˳� 1400015962)");

	cout << "1.���캯��ʾ����" << endl;
	cout << "UpDown() = " << UpDown() << endl;
	cout << "UpDown(-3.14) = " << UpDown(-3.14) << endl;
	cout << "UpDown(2.0/3) = " << UpDown(2.0 / 3) << endl;
	cout << "UpDown(2, -3) = " << UpDown(2, -3) << endl;
	cout << "Division by zero: UpDown(2, 0)" << endl; UpDown(2, 0);

	cout << endl << "2.�����ʾ����" << endl;
	cout << "UpDown(3, -4) + UpDown(2, 3) = " << (UpDown(3, -4) + UpDown(2, 3)) << endl;
	cout << "++UpDown(-3, 4) = " << ++UpDown(-3, 4) << endl;
	cout << "UpDown(3, 5) - 3 = " << (UpDown(3, 5) - 3) << endl;
	cout << "2.4 * -UpDown(3, 5) = " << (2.4 * -UpDown(3, 5)) << endl;
	cout << "UpDown(3, 5) / UpDown(0, 1)" << endl; UpDown(3, 5) / UpDown(0, 1);
	cout << "UpDown(2, 3)[0] = " << UpDown(2, 3)[0] << endl;
	cout << "UpDown(3, 4) > UpDown(2, 3) = " << (UpDown(3, 4) > UpDown(2, 3)) << endl;
	UpDown myUD;
	myUD = 2.5;//��ֵ
	cout << myUD << '\t';
	myUD -= 1.0 / 3;
	myUD.Print();

	cout << endl << endl << "3.����ת��ʾ����" << endl;
	cout << "double(UpDown(3, 2)) = " << double(UpDown(3, 2)) << endl;
	cout << "int(UpDown(3, 2)) = " << int(UpDown(3, 2)) << endl;
	if (UpDown(2, 5) - 0.4) cout << 1; else cout << 0;

	cout << endl << endl << "4.setter/getter/print����ʾ����UpDown(2, 3).setDown(-7).Print()" << endl;
	UpDown(2, 3).setDown(-7).Print();

	cout << endl << endl << "5.sort����ʾ����" << "UpDown arr[]{1.0/3, -2, 2.4, UpDown(4,3), 0}" << endl << "����ǰ��";
	UpDown arr[]{ 1.0 / 3, -2, 2.4, UpDown(4, 3), 0 };
	unsigned int dataCount = sizeof(arr) / sizeof(arr[0]);
	for (unsigned int iLoop = 0; iLoop < dataCount; ++iLoop) { cout << arr[iLoop] << '\t'; }
	cout << endl << "����  ��";
	sort(arr, dataCount);
	for (unsigned int iLoop = 0; iLoop < dataCount; ++iLoop) { cout << arr[iLoop] << '\t'; }
	cout << endl << "����  ��";
	sort(arr, dataCount, true);
	for (unsigned int iLoop = 0; iLoop < dataCount; ++iLoop) { cout << arr[iLoop] << '\t'; }

	system("echo.&pause>nul");
	return 0;
}