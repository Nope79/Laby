#include <iostream>
#include <cstdio>
#include <vector>
#include <random>
 
using namespace std;

// I'll use a random number between 1 and 4 for generate the start and end coordinates based on this number
// 1 = North, 2 = East, 3 = South, 4 = West
random_device rd;   
mt19937 generator(rd()); 
uniform_int_distribution<> dist(1, 4); 

class Maze{
public:	
	int height;
	int width;
	pair <int, int> start;
	pair <int, int> end;
	int difficulty; // x <= 20: 0, x <= 50: 1 <= 100: 2, x > 100 : 3
					// where x is max(height, width)
	vector <vector <char>> mz;
	
	Maze(int height, int width){
		this->height = height;
		this->width = width;
		this->mz = vector<vector<char>>(height, vector<char>(width, '#'));
		int maxi = max(height, width);
		this->difficulty = maxi <= 20 ? 0 : maxi <= 50 ? 1 : maxi <= 100 ? 2 : 3;
		pair <pair <int, int>, pair <int, int>> c = gen_coordinates();
		this->start = c.first;
		this->end = c.second;
		this->mz[start.first][start.second] = 'S';
		this->mz[end.first][end.second] = 'E';
	}
	
	void show(){
		for(vector <char> x : mz){
			for(char c : x){
				cout << c;
			}
			cout << endl;
		}
	}
	
	void attributes(){
		printf("Altura: %d\nAncho: %d\ndifficulty: %d\nStart: (%d, %d)\nEnd: (%d, %d)\n", 
		this->height, this->width, this->difficulty, this->start.first, this->start.second, this->end.first, this->end.second);
	}
	
	pair <pair<int, int>, pair <int, int>> gen_coordinates(){
		int n = this->height, m = this->width;
		cout << n << " " << m << endl;
		int x = dist(generator);
		uniform_int_distribution<> rows(0, n-2);
		int i = rows(generator);
		uniform_int_distribution<> columns(0, m-2);
		int j = columns(generator);
		pair <int, int> start, end;
		
		cout << "i y j: " << i << " " << j << endl;
		
		if(x == 1) start = {0, j}; // maze[0][x]
		if(x == 2) start = {i, m}; // maze[x][m-1]
		if(x == 3) start = {n, j}; // maze[n-1][x]
		if(x == 4) start = {i, 0}; // maze[x][0]
		
		cout << "X: " << x << endl;
			
		end = start;
		
		cout << start.first << " " << start.second << endl;
		
		while(end == start){
			cout << "duplicada xddd\n";
			x = dist(generator);
			i = rows(generator);
			j = columns(generator);
			if(x == 1) end = {0, j-1}; // maze[0][x]
			if(x == 2) end = {i-1, m-1}; // maze[x][m-1]
			if(x == 3) end = {n-1, j-1}; // maze[n-1][x]
			if(x == 4) end = {i-1, 0}; // maze[x][0]
		}
		
		cout << end.first << " " << end.second << endl;
		return {start, end};
	}
};