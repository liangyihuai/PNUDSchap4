// Program 4.24 ¨C Adding two polynomials, page 206
//Polynomial addition

#include <iostream>
#include<string>
#include <assert.h>

using namespace std;

template<class T> class Term {
public:
	//All members of Term are public by default
	T coef; //coefficient
	T exp; //exponent
	Term() {}
	Term(T c, T e) :coef(c), exp(e) {}
	Term Set(int c, int e) { coef = c; exp = e; return *this; };
};


template<class T> class Chain; //forward declaration
template<class T> class ChainIterator;

template <typename valType>
inline ostream& operator<< (ostream &os, const Term<valType> &term) {
	os << term.coef << "^" << term.exp;
	return os;
}

template<class T> class ChainNode {
	friend class Chain<T>;
	friend class ChainIterator<T>;

public:
	ChainNode() {}

	ChainNode(const T &element) {
		data = element;
		link = 0;
	}

private:
	T data;
	ChainNode<T> *link;
};

template<class T> class Chain {
public:
	Chain() { first = 0; };

	void Delete(void) //delete the first element after first
	{
		ChainNode<T> *current, *next;
		next = first->link;
		if (first != nullptr) //non-empty list
		{
			ChainNode<T> *temp = first;
			first = next;
			delete temp;
		}
		else cout << "Empty List: Not deleted" << endl;
	}

	int Length();
	void Add(const T& element) //add a new node after first
	{
		ChainNode<T> *newnode = new ChainNode<T>(element);
		if (!first) //insert into empty list
		{
			first = newnode;
			return;
		}
		//insert a new node after first
		newnode->link = first;
		first = newnode;
	}

	void Invert() {
		ChainNode<T> *p = first, *q = 0; //q trails p
		while (p) {
			ChainNode<T> *r = q; q = p; //r trails q
			p = p->link; //p moves to next node
			q->link = r; //link q to preceding node
		}
		first = q;
	}

	void Concatenate(Chain<T> b) {
		if (!first) { first = b.first; return; }
		if (b.first) {
			for (ChainNode<T> *p = first; p->link; p = p->link) {
				p->link = b.first;
			}
		}
	}

	void InsertBack(const T & element) {
		ChainNode<T> *newnode = new ChainNode<T>(element);
		if (!first) //insert into empty list
		{
			first = newnode;
			return;
		}
		ChainNode<T> *curr = first;
		while (curr->link != NULL) {
			curr = curr->link;
		}
		curr->link = newnode;
	}

	void displayAll() {
		if (!first) return;
		ChainNode<T> * curr = first;
		while (curr != NULL) {
			cout << curr->data << " + ";
			curr = curr->link;
		}
		cout << endl;
	}

	ChainIterator<T> begin() const { return ChainIterator<T>(first); }
	ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }

private:
	ChainNode<T> *first;

};


template<class T> class ChainIterator {
private:
	//const Chain<T>* list; //refers to an existing list
	ChainNode<T>* current; //points to a node in list

public:
	//ChainIterator<T>(const Chain<T> &l) :Chain(l), current(l.first) {};
	ChainIterator() {}

	ChainIterator(ChainNode<T> *node):current(node) {}

	//dereferencing operators
	T& operator *() const { return current->data; }
	T* operator->()const { return &current->data; }

	bool operator && (ChainIterator<T> iter)const {
		return current != NULL && iter.current != NULL;
	}

	bool isEmpty() const {
		return current == NULL;
	}

	//increment
	ChainIterator& operator ++() //preincrement
	{
		current = current->link;
		return *this;
	}
	ChainIterator operator ++(int) //post increment
	{
		ChainIterator old = *this;
		current = current->link;
		return old;
	}
	bool NotNull() { //check the current element in list is non-null
		if (current) return 1;
		else return 0;
	}
	bool NextNotNull() { //check the next element in list is non-null
		if (current && current->link) return 1;
		else return 0;
	}

	//T* First() { //return a pointer to the first element of list
	//	if (list->first) return &list->first->data;
	//	else return 0;
	//}

	T* Next() {//return a pointer to the next element of list
		if (current) {
			current = current->link;
			return &current->data;
		}
		else return 0;
	}

};


/*
class Polynomial
*/
template<class T> class Polynomial {
public:
	Polynomial() {}
	//Polynomial(Chain<Term<T> > * terms):poly(terms){}
	Polynomial operator+(const Polynomial<T>& b) const;
	void add(T coef, T exponent);

	void addAll(Polynomial *poly);
	void display();
private:
	Chain<Term<T> > poly;
};

template<class T>
void Polynomial<T>::add(T coef, T exponent) {
	Term<T> *newTerm = new Term<T>(coef, exponent);
	this->poly.Add(*newTerm);
}

template<class T> void Polynomial<T>::addAll(Polynomial<T> *b) {
	ChainIterator<Term<T>> iterB = b->poly.begin();
	
	while (iterB.NotNull()) {
		Term<T> dataB = *iterB;
		add(dataB.coef, dataB.exp);
		iterB.Next();
	}
}

template<class T> void Polynomial<T>::display() {
	poly.displayAll();
}

template<class T> Polynomial<T> Polynomial<T>::operator+(const Polynomial& b) const {
	Term temp;

	ChainIterator<T> ai = poly.begin(), bi = b.poly.begin();
	Chain<Term> cChain;
	Polynomial c(&cChain);

	while (ai && bi) {//current nodes are not null

		if (ai->exp == bi->exp) {
			int sum = ai->coef + bi->coef;
			if (sum) c.poly.InsertBack(temp.Set(sum, ai->exp));
			ai++, bi++; //advance to next term
		}
		else if (ai->exp < bi->exp) {
			c.poly.InsertBack(temp.Set(bi->coef, bi->exp));
			bi++;//next term of b
		}
	}

	while (!ai.isEmpty()) {//copy rest of a
		c.poly.InsertBack(temp.Set(ai->coef, ai->exp));
		ai++;
	}
	while (!bi.isEmpty()) {//copy rest of b
		c.poly.InsertBack(temp.Set(bi->coef, bi->exp));
		bi++;
	}
	return c;
}

//void testChain() {
//	Chain<int> chain;
//	chain.Add(1);
//	chain.Add(2);
//	chain.Add(3);
//	chain.Add(4);
//
//	chain.displayAll();
//
//	ChainIterator<int> iter = chain.begin();
//	
//	while (iter.NotNull()) {
//		cout << *(iter) << ", ";
//		iter.Next();
//	}
//	
//	cout << endl;
//	
//	chain.Delete();
//	chain.displayAll();
//}

int main(void) {

	Polynomial<int> a, b;

	char select;
	/*Term<int> *tempTerm;
	ChainNode<Term<int>> nd;
	Chain<Term<int>> a, b;
	ChainIterator<Term<int>> iter;*/
	int c, e;

	cout << endl << "Select command: a: Add_a, b: Add_b, p: Plus, d: DisplayAll, q: exit" << endl;
	cin >> select;
	while (select != 'q')
	{
		switch (select)
		{
		case 'a':
			cout << "Add a new term to a: " << endl;
			cout << "input coef: ";
			cin >> c;
			cout << "input exp: ";
			cin >> e;
			a.add(c, e);
			break;
		case 'b':
			cout << "Add a new term to b: " << endl;
			cout << "input coef: ";
			cin >> c;
			cout << "input exp: ";
			cin >> e;
			b.add(c, e);
			break;
		case 'p': //a+b
			cout << "a+b: ";
			a.addAll(&b);
			a.display();
			break;
		case 'd':
			cout << "display all: " << endl;
			a.display();
			break;
		default:
			cout << "WRONG INPUT  " << endl;
			cout << "Re-Enter" << endl;
		}
		cout << endl << "Select command: a: Add_a, b: Add_b, p: Plus, d: DisplayAll, q: exit" << endl;
		cin >> select;
	}
	system("pause");
	return 0;
}


