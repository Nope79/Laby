#include <iostream>
#include <vector>
#include <random>

using namespace std;


class Solve{
public:
	
	Maze &m;
	vector <vector <char>> main_path;
	
	Solve(Maze &maze) : m(maze){
		this->m = m;
		this->main_path = vector <vector <char>>(m.height, vector <char>(m.width, '#'));
		main_path[m.start.first][m.start.second] = 'S';
		main_path[m.end.first][m.end.second] = 'E';
	}
	
	void print(){
		for(vector <char> v : main_path){
			for(char c : v){
				cout << c;
			}
			cout << endl;
		}
	}
};