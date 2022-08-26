#include <string>
#include <sstream>
#include "utils.h"

using namespace std;
 
string extractAllIntegers(string strIn) {
  stringstream sIn;
  string strOut, aux;
  int number;

  // Skips comments
  if( strIn[0] != '#' ) {
    sIn << strIn;
    while( !sIn.eof() ) {
      sIn >> aux;
      if( stringstream(aux) >> number )
        strOut += " " + to_string(number);
    }
  }

  return strOut;
}
