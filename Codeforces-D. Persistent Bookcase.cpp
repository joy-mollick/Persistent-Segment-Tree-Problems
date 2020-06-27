/// Time-249ms
/// persistent segment tree
/// Very Nice One 

#include<bits/stdc++.h>
using namespace std;
#define mx 100001

struct Node
{
    Node *left;
    Node *right;
    bitset<1001>everyshelf;
/// on parameter holds that how many numbers are one in that range
    int sum;
    Node(int v=0,Node*l=NULL,Node*r=NULL,bitset<1001>b=0)
    {
        sum=v;
        left=l;
        right=r;
        everyshelf=b;
    }
}*root[mx];

/// it will be used to invert the integers in bitset per shelf of the tree
bitset<1001>basic;

/// It will be used for only for the first time to build seg-tree.
void build(Node* curr,int l,int r)
{
    if(l==r)
    {
        curr->sum=0;
        return;
    }
    int mid=(l+r)>>1;/// dividing by 2
    curr->left=new Node();///new node is created automatically and initialized by constructor
    curr->right=new Node();///new node is created automatically and initialized by constructor
    build(curr->left,l,mid);
    build(curr->right,mid+1,r);
    curr->sum=(curr->left->sum+curr->right->sum);
}

void update(Node* prev,Node* nwone,int l,int r,int typ,int i,int j)
{
    /// out of range
    if(i>r || i<l || l>r) return;
    if(l==r)
    {
        /// taking previous one's parameters and we have to update it
        nwone->everyshelf=prev->everyshelf;
        if(typ==1)
        {
            nwone->everyshelf.set(j);
        }
        else if(typ==2)
        {
            nwone->everyshelf.reset(j);
        }
        else if(typ==3)
        {
            nwone->everyshelf^=basic;
        }
      ///after doing all type operations,assign value newly
        nwone->sum=nwone->everyshelf.count();
        return;
    }
    int mid=(l+r)>>1;
    if(i<=mid)
    {
        nwone->right=prev->right;
        nwone->left=new Node();
        update(prev->left,nwone->left,l,mid,typ,i,j);
    }
    else
    {
        nwone->left=prev->left;
        nwone->right=new Node();
        update(prev->right,nwone->right,mid+1,r,typ,i,j);
    }
    /// node's value is recursively added
    nwone->sum=(nwone->left->sum+nwone->right->sum);
}


int main()

{

    int n,q,u,vv,m;
    scanf("%d%d%d",&n,&m,&q);

    for(int l=1;l<=m;l++) basic.set(l);
    int type,op,x,y;
    root[0] = new Node();
    build(root[0],1,n);
    for(int i=1;i<=q;i++)
    {
         scanf("%d%d",&op,&x);
        if(op==1||op==2) scanf("%d",&y);
        if(op==4){
            root[i] = root[x];
        }
        else{
            root[i] = new Node();
            update(root[i-1],root[i],1,n,op,x,y);
        }
        /// root node hold the sum of all (1-N) range ,so no need to do query to get answer ,direct call sum property of root node 
        printf("%d\n", root[i]->sum);
    }

    return 0;
}
