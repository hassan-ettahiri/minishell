#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int main() {
	// your code goes here
    long long t;
    cin>>t;
    while(t--)
    {
        long long n;
        cin>>n;
        vector<long long> v(n+1);
        set<pair<long long,long long>> s;
        for(long long i = 1 ; i <= n ; i++)
        {
            cin>>v[i];
            if(i < n)
                s.insert({v[i],i});
        }
        long long S = 0;
        for(long long i = n; i >= 1 ; i--)
        {
            S += v[i];
            if(!s.empty() && (--s.end())->first > v[i])
            {
                cout<<S-v[i]+(--s.end())->first<<" ";
            }
            else
                cout<<S<<" ";
            s.erase({v[i],i});
        }cout<<endl;
    }
}
//didi dodo lili

