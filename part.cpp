/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>

using namespace std;

/*
p0 : a    s1    r3    b
p1 : c    r2    s3   ""
p2 : r1   d    s2    e
*/

int main()
{
    vector<vector<string>> test = { {"a", "s1", "r3", "b"}, { "c", "r2", "s3", "" }, { "r1", "d", "s2", "e" } };
    
    vector<vector<int>> result = compute(test, 3, 4);

    for(int i = 0; i < result.size(); i++)
    {
        for(int j = 0; j < result[i].size(); j++)
        {
            cout << result[i][j] << " " << endl;
        }
    }
    return 0;
}

vector<vector<int>> compute(vector<vector<string>> grid, int N, int M)
{
    for(int row; row < N; row++)
    {
        for(int col; col < M; col++)
        {
            
        }
    }
}
