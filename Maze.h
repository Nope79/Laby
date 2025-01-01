#include <iostream>
#include <cstdio>
#include <vector>
#include <random>
#include <algorithm>
 
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
	int difficulty; // x <= 19: 0, x <= 49: 1 <= 99: 2, x > 99 : 3
					// where x is max(height, width)
	vector <vector <char>> mz;
	
	Maze(int height, int width){
		height = (height / 2 * 2) + 1;
		width = (width / 2 * 2) + 1;
		this->height = height;
		this->width = width;
		this->mz = vector<vector<char>>(height, vector<char>(width, '.'));
		int maxi = max(height, width);
		this->difficulty = maxi <= 19 ? 0 : maxi <= 49 ? 1 : maxi <= 99 ? 2 : 3;
		pair <pair <int, int>, pair <int, int>> c = gen_coordinates(height, width);
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
	
	pair <pair<int, int>, pair <int, int>> gen_coordinates(int height, int width){
		int n = height, m = width;
		n--, m--;
		int x = dist(generator);
		uniform_int_distribution<> rows(1, n-1);
		int i = rows(generator);
		uniform_int_distribution<> columns(1, m-1);
		int j = columns(generator);
		pair <int, int> start, end;
		
		if(x == 1) start = {0, j}; // maze[0][x]
		if(x == 2) start = {i, m}; // maze[x][m-1]
		if(x == 3) start = {n, j}; // maze[n-1][x]
		if(x == 4) start = {i, 0}; // maze[x][0]
			
		end = start;
		int y;
		while(end == start){
			y = dist(generator);
			i = rows(generator);
			j = columns(generator);
			if(y == 1) end = {0, j}; // maze[0][x]
			if(y == 2) end = {i, m}; // maze[x][m-1]
			if(y == 3) end = {n, j}; // maze[n-1][x]
			if(y == 4) end = {i, 0}; // maze[x][0]
		} 
		
		// We must stabilize the components (odd)
		start = stabilize(x, start);
		end = stabilize(y, end);
		
		gen_paths(x, start, end);
		
		return {start, end};
	}
	
	pair<int, int> stabilize(int x, pair <int, int> p){
		
		if(x == 1 || x == 3) p.second = (p.second / 2 * 2) + 1;
		else if(x == 2 || x == 4) p.first = (p.first / 2 * 2) + 1;
		
		return p;
	}
	
	void gen_paths(int x, pair <int, int> start, pair <int, int> end){
		
		int consx = 2, consy = 2;
		int advance_x, advance_y;
		
		if(x == 1) start.first++;
		if(x == 2) start.second--;
		if(x == 3) start.first--;
		if(x == 4) start.second++;
		
		mz[start.first][start.second] = '#';
		
		int direction;
		
		int con = 0;
		
		if(end.first == 0) end.first++;
		if(end.first == height-1) end.first--;
		if(end.second == 0) end.second++;
		if(end.second == width-1) end.second--;
		
		while(con++ < 2000 && mz[start.first+1][start.second] != 'E' && mz[start.first-1][start.second] != 'E' && mz[start.first][start.second+1] != 'E' && mz[start.first][start.second-1] != 'E'){
			
			if(mz[end.first][end.second] != '.') break;
			
			int jump = dist(generator);
			
			direction = dist(generator);
			cout << direction << endl;
			if(direction == 1) advance_x = -consx, advance_y = 0;
			if(direction == 2) advance_x = 0, advance_y = consy;
			if(direction == 3) advance_x = consx, advance_y = 0;
			if(direction == 4) advance_x = 0, advance_y = -consy;
			
			int a = start.first + advance_x, b = start.second + advance_y;
			
			if(a >= 0 && a < height && b >= 0 && b < width){
				if(advance_x != 0){
					
					if(start.first < a){
					
						for(int i = start.first+1; i <= a; i++){
							mz[i][start.second] = 'N';
						}
					}else{
						
						for(int i = start.first-1; i >= a; i--){
							mz[i][start.second] = 'S';
						}
					}
				}else{
					if(start.second < b){
						
						for(int i = start.second+1; i <= b; i++){
							mz[start.first][i] = 'e';
						}
					}else{
						for(int i = start.second-1; i >= b; i--){
							mz[start.first][i] = 'W';
						}
					}
				}
				start.first = a;
				start.second = b;
			}
		}
		cout << "contador: " << con << endl;
	}
};