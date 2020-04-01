#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctime>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>

#define MAX 1000000
#define CMAX 1000000

using namespace std;

int num1[MAX] = {0}; //source
int num2[MAX] = {0}; //target
int num3[MAX] = {0}; //weight
set<int> account1;
set<int> account2;

unordered_map<int, int> mp; //account with sorted number, check not real NO.
vector<int> actNo; //check real account NO.

int actNum = 0; //useful account number
int dp[MAX/100][MAX/100]; //adjMat
int vis[MAX];
int vis2[MAX];
vector<int> path;
vector<vector<int>> output;
char outbuf[CMAX]; //result

int char2num(char* buf, int len){
    int i = 0, j = 0;
    for(char *p = buf; p - buf < len; ++p) {
        //cout << (char)(*p);
        if(*p == ',') {
            ++i;
            continue;
        }
        if(*p == '\n'|| *p == '\r') {
            i = 0;
            //for cut useless data
            account1.insert(num1[j]);
            account2.insert(num2[j]);
            ++j;
            //cout << j << endl;
            if(*(p+1) == '\n')
                ++p;
            continue;
        }
        if(i == 0) {
            num1[j] = num1[j] * 10 + *p - '0';
            continue;
        }
        if(i == 1) {
            num2[j] = num2[j] * 10 + *p - '0';
            continue;
        }
        if(i == 2) {

            num3[j] = num3[j] * 10 + *p - '0';
            continue;
        }
    }
    return j;
}

void dealPath(vector<int> path){
    
    int start = *path.rbegin();
    int min = start;
    path.pop_back();
    //output.emplace_back(path);
    
    vector<int>::iterator left = find(path.begin(), path.end(), start);
    vector<int>::iterator minIter = left;
    vector<int>::iterator right = path.end();
    for(auto i = left; i <= right; ++i){
        if(*i < min) {
            min = *i;
            minIter = i;
        }
    }
    if(right - left > 7 || right - left < 3)
        return;
    vector<int> pathNew;
    pathNew.insert(pathNew.end(), minIter, right);
    pathNew.insert(pathNew.end(), left, minIter);
    output.emplace_back(pathNew);
    
    
    return;
}

void dfs(int v, int n) {
    if(vis[v] == 1 && vis2[v] == -1)
        return;
    if(vis[v] == 1) {
       dealPath(path);
       return;
    }
    vis[v] = 1;
    for(int i = 0; i < n; ++i) {
        if(dp[v][i]) {
            path.emplace_back(i);
            dfs(i, n);
            path.pop_back();
        }
    }
    vis[v] = 0;
    vis2[v] = -1;
    return;
}

bool compare(vector<int>& v1, vector<int>& v2) {
    if(v1.size() == v2.size()) {
        for(int i = 0; i < v1.size(); ++i) {
            if(v1[i] < v2[i])
                return true;
            if(v1[i] > v2[i])
                return false;
        }
    }
    else
        return v1.size() < v2.size();
}

int main(int argc,char* argv[])
{
    clock_t start = clock();
	int fd;
    if(argc >= 2)
        fd = open(argv[1], O_RDONLY);
    else
        fd = open("test_data.txt", O_RDONLY);
    int len = lseek(fd, 0, SEEK_END);
    char *buf = (char*)mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    cout << "the length of file is : " << len << endl;
    int nTemp = char2num(buf, len); //all trade number
    cout << "all trade number is :" << nTemp << endl;
    int tradeNum = 0; //useful trade number
    set<int> actReal; //record useful account NO.
    vector<int> source, target; //record the source and target of useful trades
    for(int i = 0; i < nTemp; ++i) {
        if(account1.find(num2[i]) != account1.end() && account2.find(num1[i]) != account2.end()){
            ++tradeNum;
            source.emplace_back(num1[i]);
            actReal.insert(num1[i]);
            target.emplace_back(num2[i]);
            actReal.insert(num2[i]);
        }
        else
            continue;
    }
    cout << "useful trade number is :" << tradeNum << endl;
    for(auto& i : actReal) {
        mp[i] = actNum++;
        actNo.emplace_back(i);
    }
    for(int i = 0; i < tradeNum; ++i) {
        int x = mp[source[i]];
        int y = mp[target[i]];
        if(i == tradeNum - 1){ 
            cout << "the last trade is :" << source[i] << " to " << target[i] << endl;
            cout << "the last trade source No. is : " << x << ", target NO. is : " << y << endl;
        }
        dp[x][y] = 1;    
    }
    cout << "actNum is : " << actNum << "\ntradeNum is : " << tradeNum << endl;
    clock_t end1 = clock();
    cout << "run time1 is: " << (double)((end1 - start)/(double)CLOCKS_PER_SEC) << "s" << endl;
    for(int i = 0; i < actNum; ++i) {
        if(vis2[i] == -1)
            continue;
        path.emplace_back(i);
        dfs(i, actNum);
        path.pop_back();
    }
    sort(output.begin(),output.end(), compare);
    clock_t end2 = clock();
    cout << "run time2 is: " << (double)((end2 - end1)/(double)CLOCKS_PER_SEC) << "s" << endl;
    cout << "the result is : " << endl;
    cout << output.size() << endl;
    for(auto& i : output){
        for(auto& j : i) 
            cout << actNo[j] << " ";
        cout << endl;
    } 

    close(fd);
    //int fd2 = open("result.txt", O_RDWR | O_CREAT, 0666);
    /*
    int fd3 = open("data1.txt", O_RDWR | O_CREAT, 0666);
    for(int i = 0; i < 100; ++i)
        write(fd3, buf, len);
    */
    /*
    char* outbuf = "4\r\n1,2,4\r\n1,3,4\r\n4,6,7\r\n1,2,5,4\r\n";
    int outlen = 33;
    write(fd2, outbuf, outlen);
    */

    //close(fd2);
    //close(fd3);
    /*
    cout << num1[0] << " " << num2[0] << " " << num3[0] << endl;
    cout << num1[9] << " " << num2[9] << " " << num3[9] << endl;
    cout << num1[19] << " " << num2[19] << " " << num3[19] << endl;
    cout << num1[n-1] << " " << num2[n-1] << " " << num3[n-1] << endl;
    */
    return 0;
}
