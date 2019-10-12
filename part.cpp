/******************************************************************************
							  Online C++ Compiler.
			   Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.
*******************************************************************************/
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

vector<vector<int>> compute(vector<vector<string>> grid, int N, int M);

/*
p0 : a    s1    r3    b
p1 : c    r2    s3   ""
p2 : r1   d    s2    e
*/

struct Event
{
	int local_time;
	int lamport_time;
	string type;
	int process;

	Event(int local_time, string type, int process)
	{
		this->local_time = local_time;
		lamport_time = 0;
		this->type = type;
		this->process = process;
	}
	Event() = default;
};

struct Network
{
	vector<vector<Event>> events;
	unordered_set<int> blocked_processes;
	unordered_map<string, Event> receive_events;
	unordered_map<string, Event> send_events;
	unordered_map<string, int> send_process;
	unordered_map<string, int> receive_process;
	unordered_set<string> messages;

	Network(vector<vector<string>> grid, int N, int M)
	{
		for (int i = 0; i < N; i++)
		{
			vector<Event> temp;
			for (int j = 0; j < M; j++)
			{
				string s = grid[i][j];
				temp.push_back(Event(j + 1, grid[i][j], i));
				if (grid[i][j].length() == 2)
				{
					if (grid[i][j][0] == 'r')
					{
						s[0] = 's';
						receive_events[s] = temp[j];
						receive_process[grid[i][j]] = i;
					}
					if (grid[i][j][0] == 's')
					{
						s[0] = 'r';
						send_events[s] = temp[j];
						send_process[grid[i][j]] = i;
					}
				}
			}
			events.push_back(temp);
		}
	}

	void send(int send_time, string type, int process)
	{
		cout << "sent " << type << " to " << process << " with time of " << send_time << endl;
		for (int i = process; i < process + 1; i++)
		{

			for (int j = 0; j < (int)events[process].size(); j++)
			{
				if (messages.find(events[i][j].type) != messages.end()) continue;
				cout << "checking event " << events[i][j].type << "\n";
				Event* current_event = &events[i][j];

				if (j > 0 && current_event->type.length() == 1)
				{
					current_event->lamport_time = events[i][j - 1].lamport_time + 1;
					messages.insert(current_event->type);
				}

				if (j > 0 && current_event->type.length() == 2 && current_event->type[0] == 's')
				{
					string s = current_event->type;
					s[0] = 'r';
					blocked_processes.insert(i);
					current_event->lamport_time = events[i][j - 1].lamport_time + 1;
					this->send(current_event->lamport_time, current_event->type, receive_process[s]);
					messages.insert(current_event->type);
					blocked_processes.erase(i);
				}

				if (j == 0 && current_event->type.length() == 1 || current_event->type.length() == 2 && current_event->type[0] == 's')
				{
					current_event->lamport_time = 1;
					messages.insert(current_event->type);
					if (current_event->type.length() == 2 && current_event->type[0] == 's')
					{
						string s = current_event->type;
						s[0] = 'r';
						if (j > 0)
						{
							current_event->lamport_time = events[i][j - 1].lamport_time + 1;
							messages.insert(current_event->type);
						}
						else
						{
							blocked_processes.insert(i);
							current_event->lamport_time = events[i][j - 1].lamport_time + 1;
							this->send(current_event->lamport_time, current_event->type, receive_process[s]);
							messages.insert(current_event->type);
							blocked_processes.erase(i);
						}
					}
				}
				if (current_event->type.length() == 2 && current_event->type[0] == 'r')
				{
					if (j == 0)
					{
						current_event->lamport_time = send_time + 1;
						messages.insert(current_event->type);
					}
					else if (current_event->type[1] == type[1])
					{
						cout << "updateing " << current_event->type << " " << std::max(events[i][j - 1].lamport_time, send_time) + 1 << endl;
						messages.insert(current_event->type);
						current_event->lamport_time = std::max(events[i][j - 1].lamport_time, send_time) + 1;
					}
				}
			}
		}
	}

	void calculate_lamport()
	{
		for (int i = 0; i < (int)events.size(); i++)
		{
			for (int j = 0; j < (int)events[i].size(); j++)
			{
				if (messages.find(events[i][j].type) != messages.end()) continue;
				cout << "checking event " << events[i][j].type << "\n";
				Event* current_event = &events[i][j];

				if (j > 0 && current_event->type.length() == 1)
				{
					current_event->lamport_time = events[i][j - 1].lamport_time + 1;
					messages.insert(current_event->type);
				}

				if (j == 0 && current_event->type.length() == 1 || current_event->type.length() == 2 && current_event->type[0] == 's')
				{
					current_event->lamport_time = 1;
					messages.insert(current_event->type);
					if (current_event->type.length() == 2 && current_event->type[0] == 's')
					{
						string s = current_event->type;
						s[0] = 'r';
						if (j > 0)
						{
							current_event->lamport_time = events[i][j - 1].lamport_time + 1;
							messages.insert(current_event->type);
						}
						blocked_processes.insert(i);
						messages.insert(current_event->type);
						current_event->lamport_time = events[i][j - 1].lamport_time + 1;
						this->send(current_event->lamport_time, current_event->type, receive_process[s]);
						blocked_processes.erase(i);
					}
				}
				if (current_event->type.length() == 2 && current_event->type[0] == 'r')
				{
					if (j == 0)
					{
						current_event->lamport_time = send_events[current_event->type].lamport_time;
						messages.insert(current_event->type);
					}
					else
					{
						messages.insert(current_event->type);
						current_event->lamport_time = std::max(events[i][j - 1].lamport_time, send_events[events[i][j].type].lamport_time) + 1;
					}
				}

			}
		}
	}

	void create_result(vector<vector<int>>& result)
	{
		for (int i = 0; i < (int)events.size(); i++)
		{
			vector<int> temp;
			for (int j = 0; j < (int)events[i].size(); j++)
			{
				temp.push_back(events[i][j].lamport_time);
			}
			result.push_back(temp);
		}
	}

};

int main()
{
	vector<vector<string>> test = { {"a", "s1", "r3", "b"}, { "c", "r2", "s3", "" }, { "r1", "d", "s2", "e" } };
	vector<vector<int>> result = compute(test, 3, 4);

	cout << "\n\n\n outputing result of algorithm... \n\n\n";

	for (int i = 0; i < (int)result.size(); i++)
	{
		for (int j = 0; j < (int)result[i].size(); j++)
		{
			cout << result[i][j] << " ";
		}
		std::cout << "\n";
	}
	cin.get();
	return 0;
}

vector<vector<int>> compute(vector<vector<string>> grid, int N, int M)
{
	Network network(grid, N, M);

	network.calculate_lamport();

	vector<vector<int>> result;
	network.create_result(result);

	return result;
}
