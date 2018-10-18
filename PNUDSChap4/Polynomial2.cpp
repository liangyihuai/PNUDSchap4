//program 4.25: Adding two polynomials, page 209
// circular list를 사용한 version

#include <iostream>

using namespace std;
// Tested p + q; 0 + p; p + 0; p + p; 0 + 0; p + (-p)
enum Bool { FALSE, TRUE };

template <class Type> class CircList;
template <class Type> class CircListIterator;
template <class Type> class List;

struct Term
{
	int coef;
	int exp;
	void Init(int c, int e) { coef = c; exp = e; };
};

ostream& operator<<(ostream& os, Term e)
{
	os << e.coef << " " << e.exp;
	return os;
}

template <class Type>
class ListNode {
	friend class CircList<Type>;
	friend class CircListIterator<Type>;
private:
	Type data;
	ListNode *link;
	ListNode(Type);
	ListNode() {};
};

template <class Type>
ListNode<Type>::ListNode(Type Default)
{
	data = Default;
	link = 0;
}

template <class Type>
class CircList {
	friend class CircListIterator<Type>;
public:
	CircList() { first = new ListNode<Type>; first->link = first; last = first; };
	CircList(const CircList&);
	~CircList();
	void Attach(Type);
private:
	ListNode<Type> *first;
	ListNode<Type> *last;
	static ListNode<Type> *av;
};

ListNode<Term> * CircList<Term>::av = 0;

template <class Type>
class CircListIterator {
public:
	CircListIterator(const CircList<Type>& l) : list(l) { current = l.first->link; }
	Type* Next();
	Type* First();
	Bool NotNull();
	Bool NextNotNull();
private:
	const CircList<Type> &list;
	ListNode<Type>* current;
};

template <class Type>
Type* CircListIterator<Type>::First() {
	if (current != list.first) return &current->data;
	else return 0;
}

template <class Type>
Type* CircListIterator<Type>::Next() {
	current = current->link;
	if (current != list.first) return &current->data;
	else return 0;
}

template <class Type>
Bool CircListIterator<Type>::NotNull()
{
	if (current != list.first) return TRUE;
	else return FALSE;
}

template <class Type>
Bool CircListIterator<Type>::NextNotNull()
{
	if (current->link != list.first) return TRUE;
	else return FALSE;
}


template <class Type>
CircList<Type>::CircList(const CircList<Type>& l)
{
	if (l.first == 0) { first = 0; return; }
	first = new ListNode<Type>(l.first->data);
	ListNode<Type> *newnode = first;
	for (ListNode<Type>* current = l.first->link; current != l.first; current = current->link)
	{
		newnode->link = new ListNode<Type>(current->data);
		newnode = newnode->link;
	}
	newnode->link = first;
}


template <class Type>
CircList<Type>::~CircList()
{
	if (first)
	{
		ListNode<Type> *second = first->link;
		first->link = av;
		av = second;
		first = 0;
	}
}


ostream& operator<<(ostream& os, CircList<Term>& l)
{
	CircListIterator<Term> li(l);
	if (!li.NotNull()) return os;
	os << *li.First() << endl;
	while (li.NextNotNull())
		os << *li.Next() << endl;
	return os;
}


template <class Type>
void CircList<Type>::Attach(Type k)
{
	ListNode<Type> *newnode = new ListNode<Type>(k);
	if (first == 0) first = last = newnode;
	else {
		newnode->link = last->link;
		last->link = newnode;
		last = newnode;
	}
}


//////////////Polynomial////////////////

class Polynomial
{
	friend ostream& operator<<(ostream&, Polynomial&);
	friend Polynomial operator+(const Polynomial&, const Polynomial&);
private:
	CircList<Term> poly;
public:
	void Add(Term e);
};


void Polynomial::Add(Term e)
{
	poly.Attach(e);
}

ostream& operator<<(ostream& os, Polynomial& p)
{
	os << p.poly;
	return os;
}

char compare(int a, int b)
{
	if (a == b) return '=';
	if (a < b) return '<';
	return '>';
}

Polynomial operator+(const Polynomial& a, const Polynomial& b)
{
	Term *p, *q, temp;
	CircListIterator<Term> Aiter(a.poly);
	CircListIterator<Term> Biter(b.poly);
	Polynomial c;

	p = Aiter.First();
	q = Biter.First();
	
	int x = 0;
	while (Aiter.NotNull() && Biter.NotNull()) {
		switch (compare(p->exp, q->exp)) {
		case '=':
			x = p->coef + q->coef;
			temp.Init(x, q->exp);
			if (x) c.poly.Attach(temp);
			p = Aiter.Next();
			q = Biter.Next();
			break;
		case '<':
			temp.Init(q->coef, q->exp);
			c.poly.Attach(temp);
			q = Biter.Next();
			break;
		case '>':
			temp.Init(p->coef, p->exp);
			c.poly.Attach(temp);
			p = Aiter.Next();
		}
	}
	while (Aiter.NotNull()) {
		temp.Init(p->coef, q->exp);
		c.poly.Attach(temp);
		p = Aiter.Next();
	}
	while (Biter.NotNull()) {
		temp.Init(q->coef, q->exp);
		c.poly.Attach(temp);
		q = Biter.Next();
	}
	return c;
}

int main()
{
	Polynomial p;
	Polynomial *q = new Polynomial;

	Term e;
	cout << "start" << endl;
	e.Init(3, 14); p.Add(e);
	e.Init(2, 8); p.Add(e);
	e.Init(1, 0); p.Add(e);
	cout << "output p" << endl << p << endl;

	e.Init(-3, 14); q->Add(e);
	e.Init(-2, 8); q->Add(e);
	e.Init(-1, 0); q->Add(e);

	cout << "output q" << endl << *q << endl;

	Polynomial r = p + *q;
	cout << "do: r = p + q" << endl << "output: \n"<< r << endl;



	Polynomial s;
	cout << "output s" << endl << s << endl;

	Polynomial t = s + s;
	cout << "do: t = s + s" << endl << "output: \n"<< t << endl;

	Polynomial u = p + p;
	cout << "do: u = p + p" << endl <<"output: \n"<< u << endl;
	delete q;

	system("pause");
	return 0;
}

