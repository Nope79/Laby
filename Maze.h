#include <iostream>
#include <cstdio> // Using for fast testing
#include <vector> 
#include <random> // Used in almost all Maze class
 
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
	pair <int, int> start; // Coordinate of Maze start
	pair <int, int> end; // Coordinate of Maze end
	int difficulty; // x <= 19: 0, x <= 49: 1 <= 99: 2, x > 99 : 3
					// where x is max(height, width)
	vector <vector <char>> mz; // Maze
	bool solved;
	
	Maze(int height, int width){
		
		height = (height / 2 * 2) + 1;
		width = (width / 2 * 2) + 1;
		// For convenience, I'll work with odd measuremets
		
		this->height = height;
		this->width = width;
		this->mz = vector<vector<char>>(height, vector<char>(width, '.')); // All Maze is inicially fulled with '.' character
		int maxi = max(height, width); // I'll determinate the difficulty with the maximum measure
		this->difficulty = maxi <= 19 ? 0 : maxi <= 49 ? 1 : maxi <= 99 ? 2 : 3; // For convenience, difficulty is an integer
		pair <pair <int, int>, pair <int, int>> c = gen_coordinates(height, width); // This function generate start and end coordinates
		this->start = c.first;
		this->end = c.second;
		this->mz[start.first][start.second] = 'S'; // Assing start character
		this->mz[end.first][end.second] = 'E'; // Assing end character
	}
	
	// This function shows the matrix / vector of vectors
	void show(){
		for(vector <char> x : mz){
			for(char c : x){
				cout << c;
			}
			cout << endl;
		}
	}
	
	// This function shows attributs, I used it for testing
	void attributes(){
		printf("Height: %d\nWidth: %d\nDifficulty: %d\nStart: (%d, %d)\nEnd: (%d, %d)\nSolved: %d", 
		this->height, this->width, this->difficulty, this->start.first, this->start.second, this->end.first, this->end.second, this->solved);
	}
	
	// This function generates start and end points in maze
	pair <pair<int, int>, pair <int, int>> gen_coordinates(int height, int width){
		
		int n = height-1, m = width-1;
		int x = dist(generator); // Remember that this value will vary between 1 and 4
		
		// Also, I'll use two distributions, one for rowns, the second one for columns
		uniform_int_distribution<> rows(1, n-1);
		uniform_int_distribution<> columns(1, m-1);
		// These distributions must be between 1 and n-1|m-1 because it can't be in the corners
		
		// Generation of i and j random indices
		int i = rows(generator);
		int j = columns(generator);
		
		// Preparating start and end coordinates
		pair <int, int> start, end;
		
		if(x == 1) start = {0, j}; // maze[0][x]
		if(x == 2) start = {i, m}; // maze[x][m-1]
		if(x == 3) start = {n, j}; // maze[n-1][x]
		if(x == 4) start = {i, 0}; // maze[x][0]
		// Where x is a random number between 0 and n or m, depending on whether x is a column or a row
			
		// We need different start and end coordinates 
		end = start;
		int y; // Same that x, I'll use x later, so, I need this aux variable
		while(end == start){
			y = dist(generator);
			i = rows(generator);
			j = columns(generator);
			if(y == 1) end = {0, j}; // maze[0][y]
			if(y == 2) end = {i, m}; // maze[y][m-1]
			if(y == 3) end = {n, j}; // maze[n-1][y]
			if(y == 4) end = {i, 0}; // maze[y][0]
			// Where y is a random number between 0 and n or m, depending on whether y is a column or a row
		} 
		
		// We must stabilize the components (odd)
		start = stabilize(x, start);
		end = stabilize(y, end);
		
		// this function is a boolean, if maze is it solved is true, else, false
		this->solved = gen_paths(x, start, end); // I'll generate main path, I use x variable for comfort
		
		return {start, end};
	}
	
	pair<int, int> stabilize(int x, pair <int, int> p){
		
		// The coordinates components at least one must be odd, depending of its position
		if(x == 1 || x == 3) p.second = (p.second / 2 * 2) + 1; // North or South, j must be odd
		else if(x == 2 || x == 4) p.first = (p.first / 2 * 2) + 1; // East or West, i must be odd

		return p;
	}
	
	bool gen_paths(int x, pair <int, int> start, pair <int, int> end){
		
		// For now, we will move on two steps
		int consx = 2, consy = 2;
		
		// Variable that it will be changing depending of move direction
		int advance_x, advance_y;
		
		// Reposition for convenience
		if(x == 1) start.first++;
		if(x == 2) start.second--;
		if(x == 3) start.first--;
		if(x == 4) start.second++;
		
		// Inicialization of path
		mz[start.first][start.second] = '#';
		
		// This varibale is used like an random direction, 1 N, 2 E, 3 S, 4 W.
		int direction;
		
		int con = 0;
		
		// Used later for find out position start and end matching, 
		// is repositioned like start position.
		if(end.first == 0) end.first++;
		if(end.first == height-1) end.first--;
		if(end.second == 0) end.second++;
		if(end.second == width-1) end.second--;
		
		// if con >= 2000, We can't create the main path...
		// if start have end like adjacent position, its time to break!
		while(con++ < 2000 && mz[start.first+1][start.second] != 'E' && mz[start.first-1][start.second] != 'E' && mz[start.first][start.second+1] != 'E' && mz[start.first][start.second-1] != 'E'){
			
			// If an adjacent position changes, its time to break!
			if(mz[end.first][end.second] != '.') break;
			
			direction = dist(generator); // valor between 1 and 4
			if(direction == 1) advance_x = -consx, advance_y = 0; // We need up, i = 0 is upper than i = 5. Screen coordinates principle, same for 2, 3 and 4
			if(direction == 2) advance_x = 0, advance_y = consy; // Go to right
			if(direction == 3) advance_x = consx, advance_y = 0; // Go to down
			if(direction == 4) advance_x = 0, advance_y = -consy; // Go to left
			
			// I was supposed to do this to modularize the code, but I can't do it XD, using variables intelligently
			int a = start.first + advance_x, b = start.second + advance_y;
			
			// This is a piece of shit that I need to improve
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
				
				// Actualizate start position
				start.first = a;
				start.second = b;
			}
		}
		
		return con < 2000; // maze is solved or not
	}
};