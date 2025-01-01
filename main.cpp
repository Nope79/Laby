#include <iostream>
#include <vector>
#include "Maze.h"
#include "Solve.h"

using namespace std;  
 
int main() {
	
	Maze m(3, 3);    
	m.show();
	m.attributes(); 
	
	cout << endl << endl;
	
	Solve s(m);
	s.print();
	
	return 0;
}