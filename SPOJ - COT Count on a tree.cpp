/// Time-2800ms
/// Very Nice One 
/// persistent and sparse table (LCA)
#include<bits/stdc++.h>
using namespace std;
#define mx 100005
int arr[mx];
const int N = 100001, L = 21;
int sparse[L][N], Lev[N], two[L];
int sz;
int vertex;
int Maxlog;
vector<int>v;
vector<int>vec[mx];
struct Node
{
    Node *left;
    Node *right;
/// on parameter holds that how many numbers are one in that range
    int on;
    Node(int v=0,Node*l=NULL,Node*r=NULL)
    {
        on=v;
        left=l;
        right=r;
    }
}*root[mx];
///root array will hold the pointer of heads of the every segment-tree after update

/// building sparse-table for finding lca
void sparseTable() {

    for (int i = 1; two[i] <= vertex; i++) {
        for (int j = 1; j <= vertex; j++) {
            if (sparse[i - 1][j] != -1) { /// if 2^(i-1) th ancestor of jth node is available then proceed
                sparse[i][j] = sparse[i - 1][sparse[i - 1][j]];
            /// sparse[i][j] states for 2^ith node from j node
            }}}
}

/// finding lca of a and b
int lca(int a, int b)
    {
        if(Lev[a]>Lev[b]) {swap(a,b);}

        int dif_between_lev=Lev[b]-Lev[a];
        /// we have to find out the same level of a which is also ancestor of b
        while(dif_between_lev>0) /// if difference between b and a is above zero , we can reduce it by binary lifting
        {
            int max_mum_power_of_i=log2(dif_between_lev);
            ///b=sparse[b][max_mum_power_of_i];
            b=sparse[max_mum_power_of_i][b];
            dif_between_lev-=(1<<max_mum_power_of_i);
        }
        if(b==a) return b;/// if a is itself ancestor of a and b

 ///Now,two are on same level,so trying to reduce the level  just before that ancestor node
        for(int i=Maxlog;i>=0;i--)
        {
            if(sparse[i][a]!=-1 && sparse[i][a]!=sparse[i][b])
            {
                a=sparse[i][a];
                b=sparse[i][b];
            }
        }
        return sparse[0][a];/// sparse[a][0] , now print the just parent of this node
    }
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

void dfs(int u , int par , int level )
{
    sparse[0][u] = par;
    Lev[u] = level;
    root[u]=new Node();
    ///so, building segment tree from root to the u node
    ///extend from [root-par] to [root-u]
    update(root[par],root[u],1,sz,arr[u]);
    for (int i = 0; i < (int) vec[u].size(); i++)
    {
        int v = vec[u][i];
        if (v != par) {
            dfs(v, u, level + 1);
        }
    }
}

int query(Node* u,Node* v,Node* lc,Node*par,int l,int r,int k )
{
    if(l==r) return l;
   /// Find sum of left subtree. But this time we don't actually have a
    int cnt = u->left->on + v->left->on - lc->left->on - par->left->on;
    int mid = (l+r)>>1;
    if(cnt>= k) return query(u->left, v->left,lc->left,par->left,l, mid,k);
    else return query(u->right, v->right,lc->right,par->right,mid+1,r,k-cnt);
}


int main()

{
     two[0] = 1;
    for (int i = 1; i <= 20; i++)
        two[i] = two[i - 1] << 1;

    int n,q,u,vv;
    scanf("%d%d",&n,&q);
    vertex=n;
    Maxlog=log2(vertex+1);

    for (int i = 0; two[i] <= vertex; i++) {
        for (int j = 1; j <= vertex; j++)
            sparse[i][j] = -1;
    }
    for(int i=1; i<=n; i++)
    {
        scanf("%d",&arr[i]);
        v.push_back(arr[i]);
    }
    for(int i=1;i<n;i++)
    {
        scanf("%d%d",&u,&vv);
        vec[u].push_back(vv);
        vec[vv].push_back(u);
    }
    sort(v.begin(),v.end());
    v.erase(unique(v.begin(),v.end()),v.end());///erase duplicates
     /// data compression
    /// we will give new id to the given numbers
    for(int i=1;i<=n;i++)
    {
    arr[i]=lower_bound(v.begin(),v.end(),arr[i])-v.begin()+1;///keeping id
    }
    sz=v.size();
    ///this is the first initialized segment tree with all zeros
    root[0] = new Node();
    build(root[0],1,sz);
    dfs(1,0,0);
    sparseTable();
  while(q--) {
        int l,r,k;
        scanf("%d %d %d",&l,&r,&k);
        int LCA=lca(l,r);
        int parent_of_lca=sparse[0][LCA];
        int indx = query(root[l], root[r],root[LCA],root[parent_of_lca],1,sz,k);
        printf("%d\n",v[indx-1]);
    }
    return 0;
}
