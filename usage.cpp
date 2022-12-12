#include <vector>
#include <map>
#include "usage.h"

//map<int,int*> Usage::usage;
map<int,vector<int>> Usage::usage;

void Usage::print() {
    cout << "------------\n";
    for( auto it = usage.begin() ; it != usage.end() ; ++it ) {
    cout << "Host: " << it->first << endl;
    cout << "\t" << "Idle:\t\t" << it->second[0] << endl;
    for( int i = 1 ; i < 8 ; ++i )
       cout << "\t" << (int)(i-1)*0.25*100 << "-" << (int)i*0.25*100 << "%:"
	    << ((i<4) ? "\t\t" : "\t")
            << it->second[i] << endl;
    cout << "\t" << ">= 200%:\t" << it->second[8] << endl;
    cout << "------------\n";
  }
}
