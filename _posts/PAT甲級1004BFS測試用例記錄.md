第三個（編號爲2）測試用例爲爲 1 0.
```c++
#include<iostream>
#include<queue>
#include<vector>
using namespace std;
int nonleave, nodenum;
int book[999];//level have
int level[999];//node belong
vector<int> v[999];
int main(){
  cin>>nodenum>>nonleave;
  for(int i = 0; i < nonleave; i++){
    int node, cir;
    cin>>node>>cir;
    for(int j = 0; j < cir; j++){
      int ttt;
      cin>>ttt;
      v[node].push_back(ttt);
    }
  }
  queue<int> act;
  act.push(1);
  int maxlevel = 0;
  level[1] = 1;
  if(nodenum == 1) {
    cout<<1;
    return 0;
  }
  while(act.size() != 0){
    int tm = act.front();
    act.pop();
    if(maxlevel < level[tm]){
      maxlevel = level[tm];
    }
    for(int j = 0; j < v[tm].size(); j++){
      level[v[tm][j]] = level[tm] + 1;
      if(v[v[tm][j]].size() == 0){
        book[level[v[tm][j]]]++;
      }
      if(v[v[tm][j]].size() > 0){
        act.push(v[tm][j]);
      }
    }
  }
  int maxi = 1;
  for(int i = 1; i < 999; i++){
    if(maxi < i && book[i] > 0){
      maxi = i;
    }
  }
  for(int i = 1; i < maxi + 1; i++ ){
    if(i != 1) cout<<" ";
    cout<<book[i];
      // cout<<book[i];
      // if(i != maxi){
      //   cout<<" ";
      // }
  }
return 0;
}
```
