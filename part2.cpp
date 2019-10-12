#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>

using namespace std;
typedef pair<int, int> coords;

/*
	1, 2, 8, 9
	1, 6, 7, 0
	3, 4, 5, 6

	receive: 0,2 => 8
			 1,1 => 6
			 2,0 => 3

	for these receieves you just find an element in the other rows that are equal to time - 1

	1, 2, 8, 9
	1, 6, 7, 0
	2, 3, 4, 5

	receive: 0,2 => 8
			 1,1 => 6
			 2,0 => 2

	lets create a mapping for each time to its location in the matrix
	then for every receive event we just find its corresponding send using the hash
	then update our grid.
*/

struct Event
{
	int row;
	int column;
	int time;

	Event(int row, int column, int time)
	{
		this->row = row;
		this->column = column;
		this->time = time;
	}

	Event() = default;
};

struct receive
{
	int row;
	int column;
	int time;

	vector<Event> sends;

	receive(int row, int column, int time)
	{
		this->row = row;
		this->column = column;
		this->time = time;
	}

	receive() = default;
};

bool verify(vector<vector<int>> test, vector<vector<string>>& result);
void print(vector<vector<string>> result);
void find_receive(vector<vector<int>> test, vector<receive>& receive_events);
bool find_send(vector<vector<int>> test, vector<receive>& receieve_events, unordered_map<int, vector<coords>> hash, vector<vector<string>>& result);
void check_send(vector<vector<int>> test, vector<receive>& receieve_events);
string next_event(string input);

int main()
{
	vector<vector<int>> test1 = { {1,2,8,9}, {1,6,7,0}, {3,4,5,6} };
	vector<vector<int>> test2 = { {1,2,8,9}, {1,6,7,0}, {2,3,4,5} };
	vector<vector<int>> test3 = { {1,2,8,9}, {1,6,7,0}, {2,4,5,6} };

	vector<vector<string>> result1;

	if (verify(test1, result1)) print(result1);
	else cout << "INCORRECT" << endl;

	vector<vector<string>> result2;

	if (verify(test2, result2)) print(result2);
	else cout << "INCORRECT" << endl;

	vector<vector<string>> result3;

	if (verify(test3, result3)) print(result3);
	else cout << "INCORRECT" << endl;

	cin.get();
	return 0;
}

bool verify(vector<vector<int>> test, vector<vector<string>>& result)
{
	// find the number of receive events
	// check if there is a potential match somewhere

	vector<receive> receive_events;
	find_receive(test, receive_events);

	check_send(test, receive_events);
	for (receive& e : receive_events)
	{
		if (e.sends.size() == 0) return false;
	}

	unordered_map<int, vector<coords>> hash;

	std::sort(receive_events.begin(), receive_events.end(), [](receive lhs, receive rhs) {
		return lhs.time < rhs.time;
	});

	for (int i = 0; i < test.size(); i++)
	{
		for (int j = 0; j < test[i].size(); j++)
		{
			hash[test[i][j]].push_back(coords(i, j));
		}
	}

	for (int i = 0; i < test.size(); i++)
	{
		vector<string> temp;
		for (int j = 0; j < test[i].size(); j++)
		{
			temp.push_back("null");
		}
		result.push_back(temp);
	}

	if (!find_send(test, receive_events, hash, result)) return false;

	char c = 'a';
	for (int i = 0; i < result.size(); i++)
	{
		for (int j = 0; j < result[i].size(); j++)
		{
			if (test[i][j] == 0) result[i][j] = "NULL";
			else if (result[i][j] == "null") result[i][j] = c++;
		}
	}

	return true;
}

void print(vector<vector<string>> result)
{
	cout << "\n\n";
	for (auto& vec : result)
	{
		for (string& str : vec)
		{
			cout << str << " ";
		}
		cout << "\n";
	}
	cout << "\n\n";
}

void find_receive(vector<vector<int>> test, vector<receive>& receive_events)
{
	for (int i = 0; i < test.size(); i++)
	{
		for (int j = 1; j < test[i].size() - 1; j++)
		{
			if (j == 1 && test[i][j - 1] > 1)
			{
				receive_events.push_back(receive(i, j - 1, test[i][j - 1]));
			}
			if (test[i][j] - 1 > test[i][j - 1])
			{
				receive_events.push_back(receive(i, j, test[i][j]));
			}
		}
	}
}

void check_send(vector<vector<int>> test, vector<receive>& receieve_events)
{
	for (int i = 0; i < test.size(); i++)
	{
		for (int j = 0; j < test[i].size(); j++)
		{
			for (auto& e : receieve_events)
			{
				if (e.time - 1 == test[i][j])
				{
					e.sends.push_back(Event(i, j, test[i][j]));
				}
			}
		}
	}
}

bool find_send(vector<vector<int>> test, vector<receive>& receieve_events, unordered_map<int, vector<coords>> hash, vector<vector<string>>& result)
{
	string curr_send = "s1";
	string curr_receive = "r1";

	for (receive& r : receieve_events)
	{
		coords temp = hash[r.time - 1].front();
		result[temp.first][temp.second] = curr_send;
		result[r.row][r.column] = curr_receive;
		curr_send = next_event(curr_send);
		curr_receive = next_event(curr_receive);
	}


	return true;
}

string next_event(string input)
{


	string result = "";
	char c = input[0];
	int temp = input[1];
	temp++;
	result += c;
	c = temp;
	result += c;
	return result;
}
