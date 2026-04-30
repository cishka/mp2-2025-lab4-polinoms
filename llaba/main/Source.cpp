#include "polinom.h"
#include<limits>
#include<iostream>

using namespace std;


void printMenu() {
	cout << "\n========== MENU ==========\n";
	cout << "1. Enter polynomial A\n";
	cout << "2. Enter polynomial B\n";
	cout << "3. Print polynomial A\n";
	cout << "4. Print polynomial B\n";
	cout << "5. Add A and B (A + B)\n";
	cout << "6. Subtract B from A (A - B)\n";
	cout << "7. Multiply A by constant\n";
	cout << "8. Multiply A by B (A * B)\n";
	cout << "9. Compare A and B\n";
	cout << "10. Clear polynomial A\n";
	cout << "11. Clear polynomial B\n";
	cout << "0. Exit\n";
	cout << "Your choice: ";
}

void entPol(Polinom& p, const string& name) {
	p.clear();
	int n;
	cout << "Ent count monom for " << name << ": ";
	cin >> n;

	if (n < 0 || cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invflid number\n";
		return;
	}

	for (int i = 0; i < n; i++) {
		double coef;
		int x, y, z;
		cout << "Monom " << i + 1 << ":\n";
		cout << "  Coeffic:  ";
		cin >> coef;
		cout << "  Degree x (0-9):  ";
		cin >> x;
		cout << "  Degree y (0-9):  ";
		cin >> y;
		cout << "  Degree z (0-9):  ";
		cin >> z;

		if (cin.fail() || x < 0 || x>9 || y < 0 || y>9 || z < 0 || z>9) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid degree. Must be 0-9\n";
			continue;
		}

		int deg = x * 100 + y * 10 + z;
		p.insertPol(Monom(deg, coef));
	}
	cout << "Polinom " << name << " enter\n";
}

int main() {
	Polinom A, B;
	int choice;
	cout << "Polinom calc (varib x, y, z, degree(0..9)\n";

	do {
		printMenu();
		cin >> choice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Must be number enter\n";
			continue;
		}

		switch (choice) {
		case 1:
			entPol(A, "A");
			break;
		case 2:
			entPol(B, "B");
			break;
		case 3:
			cout << "A =";
			A.print();
			cout << endl;
			break;
		case 4:
			cout << "B =";
			B.print();
			cout << endl;
			break;
		case 5: {
			Polinom C = A.add(B);

			cout << "A+B =";
			C.print();
			cout << endl;
			break;
		}
		case 6: {
			Polinom C = A.subtract(B);
			cout << "A - B = ";
			C.print();
			cout << endl;
			break;
		}
		case 7: {
			double k;
			cout << "enter const: ";
			cin >> k;
			Polinom C = A.mulonconst(k);
			cout << "A * " << k << " = ";
			C.print();
			cout << endl;
			break;
		}
		case 8: {
			try {
				Polinom C = A.mul(B);
				cout << "A * B = ";
				C.print();
				cout << endl;
			}
			catch (const runtime_error& e) {
				cout << "Error: " << e.what() << endl;
			}
			break;
		}
		case 9: {
			if (A == B)
				cout << "A and B are equal.\n";
			else
				cout << "A and B are not equal.\n";
			break;
		}
		case 10:
			A.clear();
			cout << "Polinom A clear.\n";
			break;
		case 11:
			B.clear();
			cout << "Polinom B clear.\n";
			break;
		case 0:
			cout << " Exit\n";
		default:
			cout << " Invalid choice. try again\n";
		}
	} while (choice != 0);

	return 0;
}
