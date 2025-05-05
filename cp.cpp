#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    string s;
    cin>>s;
    map<char,int> mp;
    for(int i = 0;s[i] ; i++)
    {
        mp[s[i]]++;
    }
    for(int i = 'a' ; i <= 'z' ; i++)
    {
        if(mp[i] == 0)
        {
            cout<<(char)i<<endl;
            break;
        }
    }
}