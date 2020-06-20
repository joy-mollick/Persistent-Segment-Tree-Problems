
/// Persistent Segment tree - Nice Idea
/// Sometimes it is very necessary to remember all changing states of segment tree after every update
/// But ,in normal segment tree ,we only consider about the present state of tree and totally forgot about past state elements
/// If it is asked to find say range sum of array after kth update then is it not so necessary to keep every state's segment tree separately
/// so that we can answer our questions for any state of segment tree say after kth update
/// That is why ,it is necessary to use persistent segment tree .
/// Now say to keep states of all segment trees created after each update
/// We can  make a new whole a single segment tree after each update ,then if initially segment tree has N nodes
/// then after Q updates we have to make Q segment trees ,so think about memory complexity Q*N and Time complexity N*Q also as every time making new segment tree will cost N time complexity.
/// To avoid this much (N*Q) time complexity and memory complexity , we will use another approach called persistent segment tree

/// This will take Qlogn memory and time complexity,which is much much faster than the previous one.



/// Time-300ms

#include<bits/stdc++.h>

using namespace std;

#define mx 100005

int arr[mx];

struct Node
{
    Node *left;
    Node *right;
    int val;
/// A constructor is a special type of member function that initialises an object automatically when it is created.
/// It is constructor .
/// Constructor has the same name as that of the class and it does not have any return type.
    Node(int v=0,Node*l=NULL,Node*r=NULL)
    {
        val=v;
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
        curr->val=arr[l];
        return;
    }
    int mid=(l+r)>>1;/// dividing by 2
    curr->left=new Node();///new node is created automatically and initialized by constructor
    curr->right=new Node();///new node is created automatically and initialized by constructor
    build(curr->left,l,mid);
    build(curr->right,mid+1,r);
    curr->val=(curr->left->val+curr->right->val);
}


void update(Node* prev,Node* nwone,int l,int r,int idx,int value)
{
    /// out of range
    if(idx>r || idx<l || l>r) return;

    if(l==r)
    {
    ///change nwnode value according to previous value
        nwone->val=(prev->val+value);
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
        update(prev->left,nwone->left,l,mid,idx,value);
    }
    ///if update index doesn't go to be laid in left range
    /// Do the opposite of previous function
    else
    {
        nwone->left=prev->left;
        nwone->right=new Node();
        update(prev->right,nwone->right,mid+1,r,idx,value);
    }

    /// recursively add left and right side value of the newnode like construction of segment tree.
    nwone->val=(nwone->left->val+nwone->right->val);
}


/// query for answering range (i,j)
int query(Node *curr,int l,int r,int i,int j)
{
    /// outside of range so return 0
    /// (i,j)..(l,r) or (l,r)..(i,j)
   if( j<l || i>r || l>r) return 0;

   /// Inside the range
   if(l>=i && r<=j)
   {
       return curr->val;
   }

   int mid=(l+r)>>1;
   int left_sum=query(curr->left,l,mid,i,j);
   int right_sum=query(curr->right,mid+1,r,i,j);
   return (left_sum+right_sum);
}


int main()

{
    int n;
    scanf("%d",&n);
    /// taking input array
    for(int i=1; i<=n; i++)
        scanf("%d",&arr[i]);

    ///this is the first initialized segment tree
    root[0] = new Node();
    /// construction of segment tree ,takes  time complexity O(2*n-1) ~ O(n) 
    build(root[0],1,n);
    int q,idx,pos,v,l,r,id=0,type;
    /// query
    scanf("%d",&q);
    /// per update takes O(logn) time-complexity 
    /// Q query takes O(Q*logn) time complexity.
    while(q--)
    {
        scanf("%d",&type);
        /// update the value of pos position of the idxth array
        /// idxth array means which has been obtained after idx th operation
        if(type==1)
        {
            scanf("%d%d%d",&idx,&pos,&v);
            id++;/// it points the number of update
            root[id] = new Node();/// creating new node's root
            update(root[idx], root[id],1,n,pos,v);
        }
        else
        {
            scanf("%d%d%d",&idx,&l,&r);
            int ans = query(root[idx],1,n,l,r);
            printf("%d\n",ans);
        }
    }
    return 0;
}
