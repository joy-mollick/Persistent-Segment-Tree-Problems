
/// Problenk Link- https://www.hackerrank.com/contests/morgan-stanley-2015/challenges/wet-shark-and-kth-largest-number/problem
/// Persistent Segment tree

/// Time-0.69s
/// Very Classical One ,nice one !

#include<bits/stdc++.h>

using namespace std;

const int mx =100005;

int arr[mx];

vector <int> indices[100005];

struct Node
{
    Node *left;
    Node *right;
/// on parameter holds that how many numbers are one in that range
    int on;
/// A constructor is a special type of member function that initialises an object automatically when it is created.
/// It is constructor .
/// Constructor has the same name as that of the class and it does not have any return type.
    Node(int v=0,Node*l=NULL,Node*r=NULL)
    {
        on=v;
        left=l;
        right=r;
    }
}*root[mx];
///root array will hold the pointer of heads of the every segment-tree after update


/// It is for construction of the main root initially
/// It will be used for only for the first time to build seg-tree.
void build(Node* curr,int l,int r)
{
    if(l==r)
    {
        curr->on=0;
        return;
    }
    int mid=(l+r)>>1;/// dividing by 2
    curr->left=new Node();///new node is created automatically and initialized by constructor
    curr->right=new Node();///new node is created automatically and initialized by constructor
    build(curr->left,l,mid);
    build(curr->right,mid+1,r);
    curr->on=(curr->left->on+curr->right->on);
}


void update(Node* prev,Node* nwone,int l,int r,int idx)
{
    /// out of range
    if(idx>r || idx<l || l>r) return;

    if(l==r)
    {
    ///change nwnode value to be increased from the previous one
        nwone->on=prev->on+1;
        return;
    }
    int mid=(l+r)>>1;
/// if update index goes to be laid in left range
/// Then,there is no change of right side
/// So assign this right side to the new_node
/// and update left side according to previous node's left side
    if(idx<=mid)
    {
        nwone->right=prev->right;
        nwone->left=new Node();
        update(prev->left,nwone->left,l,mid,idx);
    }
    ///if update index doesn't go to be laid in left range
    /// Do the opposite of previous function
    else
    {
        nwone->left=prev->left;
        nwone->right=new Node();
        update(prev->right,nwone->right,mid+1,r,idx);
    }

    /// recursively add left and right side value of the newnode like construction of segment tree.
    nwone->on=(nwone->left->on+nwone->right->on);
}


/// query for answering range (i,j)
/// Node *prev for prefix segment tree 1 to i-1
/// Node *nw for prefix segment tree 1 to j
/// So , all time subtracting node range 1 to i-1 (prev) from 1 to j (nw)
/// likelihood this difference will work like a one segment tree with range [i,j] ,awesome !!

int query(Node* nw, int l,int r,int k )
{
    if(l==r) return l;
   /// Find sum of left subtree. But this time we don't actually have a
  /// Segment Tree Node for that range. But we have 2 segment tree.
  /// and we know that the segemnt tree we want has each node value =
  /// difference of other two segment tree nodes for same range.
    int cnt = (nw->left==NULL)?0:nw->left->on;
    int mid = (l+r)>>1;
    if(cnt>= k) return query(nw->left,l, mid,k);
    else return query(nw->right,mid+1,r,k-cnt);
}


int main()

{
    int n,q;
    scanf("%d%d",&n,&q);
    /// taking input array
    for(int i=1; i<=n; i++){
        scanf("%d",&arr[i]);
        indices[arr[i]].push_back(i);
        /// getting positions
    }

    ///this is the first initialized segment tree with all zeros
    ///we will make segment tree from reverse
    root[100001] = new Node();
    /// construction of segment tree ,takes  time complexity ~ O(n)
    /// initially there is no numbers at positions 
    build(root[100001],1,100000);
    /// at most number and size of the array 10^5 ,so position we have to cover 100000 
    for(int j=100000;j>=1;j--)
    {
    /// This number has no positions in the array,so take previous one's root
        if(indices[j].size()==0)
        {
        root[j]=root[j+1];
        }
        else
        {
            Node *temp,*prev;
            root[j]=new Node();
        update(root[j+1],root[j],1,n,indices[j][0]);
        prev=root[j];
        for(int i=1;i<(int)indices[j].size();i++){
            temp=new Node();
        update(prev,temp,1,n,indices[j][i]);
        prev=temp;
        }
///after making all positions on of the number j in segment tree. 
/// we assigned it's root to root[j].
/// root[j] holds the segment tree of positions of numbers equal or greater than j 
        root[j]=prev;
        }
    }
  /// now all prefix segment tree [1,i] ,1<=i<=n has been created
  /// we will build prefix segment tree,thinking about all query values also as we have to add those values later at the end.
  for(int i=0;i<q;i++)
  {
      int indx,k;
      scanf("%d%d",&indx,&k);
      printf("%d\n",arr[query(root[indx],1,n,k)]);
  }
    return 0;
}
