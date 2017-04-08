#include<iostream>
#include<string>
using namespace std;

class string_set{
	struct node_st{node_st *p1,*p2; string value; bool red;};
	node_st *tree_root;
	int nodes_count;
private:
	node_st *NewNode(string value);
	void DelNode(node_st*);
	void Clear(node_st*);
	node_st *Rotate21(node_st*);
	node_st *Rotate12(node_st*);
	void BalanceInsert(node_st**);
	bool BalanceRemove1(node_st**);
	bool BalanceRemove2(node_st**);
	bool Insert(string,node_st**);
	bool GetMin(node_st**,node_st**);
	bool Remove(node_st**,string);
	void Show(node_st*);
public:
	string_set();
	~string_set();
	void Clear();
	bool Find(string);
	void Insert(string);
	void Remove(string);
	int GetNodesCount();
	bool Empty();
	void Show();
};


string_set::string_set()
{
	tree_root=0;
	nodes_count=0;
}

string_set::~string_set()
{
	Clear(tree_root);
}

int string_set::GetNodesCount()
{
	return nodes_count;
}

bool string_set::Empty()
{
	if (!tree_root)
        return true;
    return false;
}

// videlenie novoi vershini
string_set::node_st *string_set::NewNode(string value)
{
	nodes_count++;
	node_st *node=new node_st;
	node->value=value;
	node->p1=node->p2=0;
	node->red=true;
	return node;
}

// ydalenie vershini
void string_set::DelNode(node_st *node)
{
	nodes_count--;
	delete node;
}

// snos dereva (rekursivnaya chast)
void string_set::Clear(node_st *node)
{
	if(!node) return;
	Clear(node->p1);
	Clear(node->p2);
	DelNode(node);
}

// vivod dereva (rekursivnaya chast)
void string_set::Show(node_st *node)
{
	int n;
	if(!node) return;
	cout<<node->value<<" ";
	Show(node->p1);
	Show(node->p2);
}

string_set::node_st *string_set::Rotate21(node_st *node)
{
	node_st *p2 =node->p2;
	node_st *p21=p2->p1;
	p2->p1=node;
	node->p2=p21;
	return p2;
}

string_set::node_st *string_set::Rotate12(node_st *node)
{
	node_st *p1 =node->p1;
	node_st *p12=p1->p2;
	p1->p2=node;
	node->p1=p12;
	return p1;
}

void string_set::BalanceInsert(node_st **root)
{
	node_st *p1,*p2,*px1,*px2;
	node_st *node=*root;
	if(node->red) return;
	p1=node->p1;
	p2=node->p2;
	if(p1 && p1->red) {
		px2=p1->p2;				// poisk dvux riadom stoiashix vershin
		if(px2 && px2->red) p1=node->p1=Rotate21(p1);
		px1=p1->p1;
		if(px1 && px1->red) {
			node->red=true;
			p1->red=false;
			if(p2 && p2->red) {
				px1->red=true;
				p2->red=false;
				return;
			}
			*root=Rotate12(node);
			return;
		}
	}
	// toge samoe v drygyu storony
	if(p2 && p2->red) {
		px1=p2->p1;
		if(px1 && px1->red) p2=node->p2=Rotate12(p2);
		px2=p2->p2;
		if(px2 && px2->red) {
			node->red=true;
			p2->red=false;
			if(p1 && p1->red) {
				px2->red=true;
				p1->red=false;
				return;
			}
			*root=Rotate21(node);
			return;
		}
	}
}


bool string_set::BalanceRemove1(node_st **root)
{
	node_st *node=*root;
	node_st *p1=node->p1;
	node_st *p2=node->p2;
	if(p1 && p1->red) {
		p1->red=false;return false;
	}
	if(p2 && p2->red) { // slychai 1
		node->red=true;
		p2->red=false;
		node=*root=Rotate21(node);
		if(BalanceRemove1(&node->p1)) node->p1->red=false;
		return false;
	}
 	unsigned int mask=0;
	node_st *p21=p2->p1;
	node_st *p22=p2->p2;
	if(p21 && p21->red) mask|=1;
	if(p22 && p22->red) mask|=2;
	switch(mask)
	{
	case 0:		// slyshai 2 - if((!p21 || !p21->red) && (!p22 || !p22->red))
		p2->red=true;
		return true;
	case 1:
	case 3:		// slyshai 3 - if(p21 && p21->red)
		p2->red=true;
		p21->red=false;
		p2=node->p2=Rotate12(p2);
		p22=p2->p2;
	case 2:		// slyshai 4 - if(p22 && p22->red)
		p2->red=node->red;
		p22->red=node->red=false;
		*root=Rotate21(node);
	}
	return false;
}

bool string_set::BalanceRemove2(node_st **root)
{
	node_st *node=*root;
	node_st *p1=node->p1;
	node_st *p2=node->p2;
	if(p2 && p2->red) {p2->red=false;return false;}
	if(p1 && p1->red) { // slyshai 1
		node->red=true;
		p1->red=false;
		node=*root=Rotate12(node);
		if(BalanceRemove2(&node->p2)) node->p2->red=false;
		return false;
	}
 	unsigned int mask=0;
	node_st *p11=p1->p1;
	node_st *p12=p1->p2;
	if(p11 && p11->red) mask|=1;
	if(p12 && p12->red) mask|=2;
	switch(mask) {
	case 0:		// slyshai 2 - if((!p12 || !p12->red) && (!p11 || !p11->red))
		p1->red=true;
		return true;
	case 2:
	case 3:		// slyshai 3 - if(p12 && p12->red)
		p1->red=true;
		p12->red=false;
		p1=node->p1=Rotate21(p1);
		p11=p1->p1;
	case 1:		// slyshai 4 - if(p11 && p11->red)
		p1->red=node->red;
		p11->red=node->red=false;
		*root=Rotate12(node);
	}
	return false;
}


bool string_set::Find(string value)
{
	node_st *node=tree_root;
	while(node) {
		if(node->value==value) return true;
		node=node->value>value?node->p1:node->p2;
	}
	return false;
}

bool string_set::Insert(string value,node_st **root)
{
	node_st *node=*root;
	if(!node) *root=NewNode(value);
	else {
		if(value==node->value) return true;
		if(Insert(value,value<node->value?&node->p1:&node->p2)) return true;
		BalanceInsert(root);
	}
	return false;
}

bool string_set::GetMin(node_st **root,node_st **res)
{
	node_st *node=*root;
	if(node->p1) {
		if(GetMin(&node->p1,res)) return BalanceRemove1(root);
	} else {
		*root=node->p2;
		*res=node;
		return !node->red;
	}
	return false;
}

bool string_set::Remove(node_st **root,string value)
{
	node_st *t,*node=*root;
	if(!node) return false;
	if(node->value<value) {
		if(Remove(&node->p2,value))	return BalanceRemove2(root);
	} else if(node->value>value) {
		if(Remove(&node->p1,value))	return BalanceRemove1(root);
	} else {
		bool res;
		if(!node->p2) {
			*root=node->p1;
			res=!node->red;
		} else {
			res=GetMin(&node->p2,root);
			t=*root;
			t->red=node->red;
			t->p1=node->p1;
			t->p2=node->p2;
			if(res) res=BalanceRemove2(root);
		}
		DelNode(node);
		return res;
	}
	return 0;
}

// vivod dereva
void string_set::Show()
{
	Show(tree_root);
}

// fynkthia vstavki
void string_set::Insert(string value)
{
	Insert(value,&tree_root);
	if(tree_root) tree_root->red=false;
}

// ydalenie yzla
void string_set::Remove(string value)
{
	Remove(&tree_root,value);
}

// snos dereva
void string_set::Clear()
{
	Clear(tree_root);
	tree_root=0;
}


int main()
{
	int n,i;
	string_set sset;
	if (sset.Empty())
        cout<<"Set is empty"<<endl;
    else
        cout<<"Set isn't empty"<<endl;

    sset.Insert("45");
    sset.Insert("sfg");
    sset.Insert("uy");
    sset.Insert("wetry");
    sset.Insert("wetry");
    sset.Show();
    cout<<endl;

    cout<<endl<<"Count: "<<sset.GetNodesCount()<<endl;
    sset.Remove("45");
    sset.Show();
    cout<<endl<<endl;

    if (sset.Find("uy"))
        cout<<"\"uy\" in set"<<endl;
    else
        cout<<"\"uy\" not in set"<<endl;

    if (sset.Find("45"))
        cout<<"\"45\" in set"<<endl;
    else
        cout<<"\"45\" not in set"<<endl;

    cout<<endl<<"Count: "<<sset.GetNodesCount()<<endl;
	if (sset.Empty())
        cout<<"Set is empty"<<endl;
    else
        cout<<"Set isn't empty"<<endl;
	return 0;
}
