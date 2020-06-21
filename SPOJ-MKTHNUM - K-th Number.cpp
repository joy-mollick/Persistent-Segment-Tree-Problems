/// Persistent Segment tree - Nice Idea
/// This will take Qlogn memory and time complexity,which is much much faster 

/// Time-300ms
/// It can be solved with merge sort tree ,sqrt decomposition and even with persistent segment tree with binary search 
/// But this approach takes only O(logn) time for each query ,faster than those approaches. 

#include<bits/stdc++.h>

using namespace std;

#define mx 100005

int arr[mx];

vector<int>v;

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
    ///change nwnode value to 1
    ///no need to use node pointer of previous
    /// Because it is new one value ,as all values are distinct
        nwone->on=1;
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
/// likelihood this difference will work like a one segment tree ,awesome !!
int query(Node* nw,Node* prev, int l,int r,int k )
{
    if(l==r) return l;
   /// Find sum of left subtree. But this time we don't actually have a
  /// Segment Tree Node for that range. But we have 2 segment tree.
  /// and we know that the segemnt tree we want has each node value =
  /// difference of other two segment tree nodes for same range.
    int cnt = nw->left->on - prev->left->on;
    int mid = (l+r)>>1;
    if(cnt>= k) return query(nw->left, prev->left,l, mid,k);
    else return query(nw->right, prev->right,mid+1,r,k-cnt);
}


int main()

{
    int n,q;
    scanf("%d%d",&n,&q);
    /// taking input array
    for(int i=1; i<=n; i++){
        scanf("%d",&arr[i]);
        v.push_back(arr[i]);
    }
    sort(v.begin(),v.end());
    /// data compression
    /// we will give new id to the given numbers
    /// we will arrange the numbers in such way that all numbers are within 1 to n
    for(int i = 1;i<= n; i++)
    {
        arr[i] = lower_bound(v.begin(),v.end(),arr[i]) - v.begin() + 1;
    }
    ///this is the first initialized segment tree with all zeros
    root[0] = new Node();
    /// construction of segment tree ,takes  time complexity O(2*n-1) ~ O(n)
    build(root[0],1,n);
    for(int i=1;i<=n;i++)
    {
    ///new root of new segment tree which hold the property for range [1 to i] .
    ///every one takes log(n) time to update as well as same memory.
    ///it will link to previous segment tree root[i-1]
        root[i]=new Node();
        update(root[i-1],root[i],1,n,arr[i]);
    }
  /// now all prefix segment tree [1,i] ,1<=i<=n has been created

  while(q--) {
        int l,r,k;
        scanf("%d %d %d",&l,&r,&k);
        int indx = query(root[r], root[l-1], 1,n,k);
        printf("%d\n",v[indx-1]);
    }
    return 0;
}
