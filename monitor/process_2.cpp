// Created Time:    2017-12-10 14:05:24
// Modified Time:   2017-12-12 09:14:47

#include <iostream>
#include <map>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
    map<int,string> Map;
    Map.insert(pair<int,string>(15,"aaa"));
    Map.insert(pair<int,string>(35,"bbb"));
    Map.insert(pair<int,string>(25,"ccc"));

    map<int,string>::iterator it;
    it = Map.find(15);
    if(it != Map.end())
    {
       cout<<it->first<<' '<<it->second<<endl;;
    }
    else
    {
        cout << "can not find" << endl;
    }

    while(1)
    {
        cout << "hello world" << endl;
        sleep(1);
    }

    return 0;
}

