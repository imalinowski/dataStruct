#include <iostream>
#include <algorithm>
using namespace std;

template<typename T>
class TwoThreeTree {
	TwoThreeTree* parent;
	T data[3];
	TwoThreeTree* child[4];
	size_t ds;//счетчик даты
	size_t cs;//счетчик детей

	void setParent(TwoThreeTree* p) {
		parent = p;
	}
	void setChild(TwoThreeTree* c, size_t i) {
		child[i] = c;
		cs++;
	}

	void set(T a, size_t i) {
		data[i] = a;
		ds++;
	}

	void push(TwoThreeTree *n) {
		(*n).child[0]->setParent(this);
		(*n).child[1]->setParent(this);

		int i = 0;
		for (i; i < ds; i++) {
			if (n->data[0] < data[i] && i == 0) break;
			if (data[i - 1] < n->data[0] && n->data[0] < data[i]) break;
			//если d больше всех i = ds
		}
		if (ds < 3) {
			for (int j = ds; j > i; j--)
				data[j] = data[j - 1];
			data[i] = n->data[0];
			child[i] = n->child[0];
			for (int j = cs; j > i; j--)
				child[j] = child[j - 1];
			child[i + 1] = n->child[1];
			ds++;
			cs++;
		}
		else {
			TwoThreeTree* left = new TwoThreeTree(this);
			TwoThreeTree* right = new TwoThreeTree(this);
			
			if (i==3) {
				for (int j = 0; j < 2; j++){
					left->set(data[j], j);
					child[j]->setParent(left);
					left->setChild(child[j], j);
					data[j] = 0;
					child[j] = nullptr;
				}
				child[2]->setParent(left);
				left->setChild(child[2], 2);
				right->set(n->data[0], 0);
				(*n).child[0]->setParent(right);
				(*n).child[1]->setParent(right);
				right->setChild(n->child[0], 0);
				right->setChild(n->child[1], 1);
				data[0] = data[2];
				data[2] = 0;
				child[2] = nullptr;
				child[3] = nullptr;
			}
			////////////////////////////////////////////////////
			if (i == 0) {
				left->set(n->data[0], 0);
				(*n).child[0]->setParent(left);
				(*n).child[1]->setParent(left);
				left->setChild(n->child[0], 0);
				left->setChild(n->child[1], 1);
				//нулевой элемент изменять не нужно
				for (int j = 1; j < 3; j++) {
					right->set(data[j], j - 1);
					right->setChild(child[j], j - 1);
					data[j] = 0;
					child[j] = nullptr;
				}
				right->setChild(child[3], 2);
				child[3] = nullptr;
			}
			else {
				int j = 0;
				for (j; j < ds; j++) {
					if (j < i) {
						left->set(data[j], j);
						left->setChild(child[j], j);
						data[j] = 0;
						child[j] = nullptr;
					}
					else if (j == i) {
						left->set(n->data[0], j);
						(*n).child[0]->setParent(left);
						(*n).child[1]->setParent(left);
						left->setChild(n->child[0], j);
						left->setChild(n->child[1], j + 1);
						data[0] = data[j];
						data[j] = 0;
						child[j] = nullptr;
					}
					else if (j > i) {
						right->set(data[j], j-i-1);
						right->setChild(child[j], j-i-1);
						data[j] = 0;
						child[j] = nullptr;
					}
				}
				right->setChild(child[j], j);
				child[j] = nullptr;
			}

			child[0] = left;
			child[1] = right;

			ds = 1;
			cs = 2;

			if (parent != nullptr)
				parent->push(this);
		}
		delete n;
	}
public:
	TwoThreeTree(TwoThreeTree* p) {
		parent = p;
		ds = 0;
		cs = 0;
		for (int i = 0; i < 3; i++)
			child[i] = nullptr;
	}

	void print() {
		if (child[0] != nullptr)child[0]->print();
		if (ds >= 1)cout << data[0] << " ";
		if (child[1] != nullptr) child[1]->print();
		if (ds >= 2) cout << data[1] << " ";
		if (child[2] != nullptr) child[2]->print();
		if (ds == 3) cout << data[2] << " ";
		if (child[3] != nullptr) child[3]->print();
	}

	void splitNode(T d) {
		//поднятие дерева(деление нода)
		TwoThreeTree* left = new TwoThreeTree(this);
		TwoThreeTree* right = new TwoThreeTree(this);
		//поиск места деления
		int i = 0;
		for (i; i < ds; i++) {
			if (d < data[i] && i == 0) break;
			if (data[i - 1] < d && d < data[i]) break;
			//если d больше всех i = ds
			left->add(data[i]);
			data[i] = 0;
		}
		//обработка частных случаев
		if (i == ds) {
			data[0] = d;
		}
		else if (i == 0) {
			for (i; i < ds; i++) {
				right->add(data[i]);
				data[i] = 0;
			}
			data[0] = d;
		}
		else {
			data[0] = d;
			for (i; i < ds; i++) {
				right->add(data[i]);
				data[i] = 0;
			}
		}
		//обновление информации
		ds = 1;
		child[0] = left;
		child[1] = right;
		cs = 2;
		//в случае наличия родителя, пытаемся засунуть туда
		if (parent != nullptr)
			parent->push(this);
	}

	void add(T d) {
		if (ds < 3 && cs == 0) {//добавление в текущий нод (лист)
			data[ds] = d;
			ds++;
			sort(data, data + ds);
		}
		else if (cs > 0) {//проверка детей
			int i = 0;
			for (i; i < ds; i++) {
				if (d < data[i] && i == 0) break;
				if (data[i - 1] < d && d < data[i]) break;
				//если d больше всех i = ds
			}
			child[i]->add(d);
		}
		else if (ds == 3 && cs == 0)
			splitNode(d);
	}
};

int main() {
	TwoThreeTree<int> a(nullptr);

	a.add(90);
	a.add(83);
	a.add(44);
	a.add(84);
	a.add(64);
	a.add(72);
	a.add(88);
	a.add(16);
	a.add(21);
	a.add(5);
	a.add(18);
	a.add(3);
	a.add(99);
	a.add(71);
	a.add(82);
	a.add(64);
	a.add(42);
	a.add(82);
	a.add(40);
	a.add(7);
	a.add(6);
	a.add(88);
	a.add(97);
	a.add(86);
	a.add(29);
	a.add(15);
	a.add(12);
	a.add(60);
	/*
	a.add(68);
	a.add(42);
	a.add(75);
	a.add(12);
	a.add(58);
	a.add(92);
	a.add(22);
	a.add(93);
	a.add(0);
	a.add(73);
	a.add(65);
	a.add(9);
	a.add(97);
	a.add(43);
	a.add(30);
	a.add(70);
	a.add(3);
	a.add(70);
	a.add(48);
	a.add(59);
	a.add(6);
	a.add(38);
	a.add(47);
	a.add(71);
	a.add(82);
	a.add(44);
	a.add(83);
	a.add(53);
	a.add(59);
	a.add(49);
	a.add(93);
	a.add(32);
	a.add(89);
	a.add(78);
	a.add(18);
	a.add(53);
	a.add(26);
	a.add(84);
	a.add(7);
	a.add(97);
	a.add(1);
	a.add(67);
	a.add(15);
	a.add(40);
	a.add(5);
	a.add(87);
	a.add(25);
	a.add(13);
	a.add(36);
	a.add(35);
	a.add(99);
	a.add(52);
	a.add(26);
	a.add(70);
	a.add(5);
	a.add(35);
	a.add(100);
	a.add(2);
	a.add(21);
	a.add(48);
	a.add(92);
	a.add(21);
	a.add(35);
	a.add(10);
	a.add(90);
	a.add(56);
	a.add(97);
	a.add(95);
	a.add(22);
	a.add(38);
	a.add(30);
	a.add(65);
	*/
	a.print();
}