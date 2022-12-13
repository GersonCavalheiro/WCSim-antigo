#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "cloud.h"
#include "node.h"
#include "host.h"
#include "baremetal.h"
#include "event.h"

vector<vector<int>> Cloud::link;

Host* Cloud::newHost( const string name, const int risingDate, const int bmFamily ) {
  // O parametro bmFamily permite escolher qual classe Host usar
  return new Host(name, risingDate);
}

Node *Cloud::getNode(string name) {
  return Node::getNode(name);
}

void Cloud::readCloudFile( string cloudFileName ) {
  string name;
  int hostRisingDate, bmFamily;
  std::ifstream infile(cloudFileName);
  cout << "Cloud file name: " << cloudFileName << endl;

  infile >> name;
  while( !infile.eof() ) {
    infile >> hostRisingDate >> bmFamily;
    Cloud::newHost(name, hostRisingDate, bmFamily);
    infile >> name;
  }
}

void Cloud::setLinkSpeeds( int host, string& speeds ) {
  stringstream sIn;
  vector<int> auxv;
  string auxs;
  int number;

  sIn << speeds;

  for( int i = host+1 ; i <= Host::getNbHosts() ; ++i ) {
    sIn >> auxs;
    if( !(stringstream(auxs) >> number) ) exit(0);
    auxv.push_back(number);
  }
  link.push_back(auxv);
}

int Cloud::getLinkSpeed( int source, vector<int>& vSpeeds ) {
  vSpeeds = link[source];
  return vSpeeds.size();
}

void Cloud::readNetworkFile( string networkFileName ) {
  int host = 0;
  string strIn, strOut;
  std::ifstream infile(networkFileName);
  cout << "Network file name: " << networkFileName << endl;

  std::getline(infile,strIn);
  while( !infile.eof() && host < Host::getNbHosts() ) {
    strOut = extractAllIntegers(strIn);
    if( strOut.size() != 0 ) setLinkSpeeds(host++,strOut);
    std::getline(infile,strIn);
  }
}

void Cloud::printAllLinks() {
  for( int i = 0; i < Host::getNbHosts() ; ++i ) {
    for( int j = 0; j < Host::getNbHosts() ; ++j ) 
      cout << Cloud::bandwidth(i,j) << "*";
    cout << endl;
  }
}

//inline void swap(int &a, int &b ){int aux=a;a=b;b=aux;}

int Cloud::bandwidth( int line, int column ) {
  if( line >= Host::getNbHosts() || column >= Host::getNbHosts() ) return 5;
  if( line == column ) return 0;
  if( line < column ) return link[line][column-1-line];
  else {std::swap(line,column); return link[line][column-1-line]; }
}

void Cloud::deploy() {
  readCloudFile();
  readNetworkFile();
  // Ler arquivo de descrição da cloud
  // Criar os nos
  // Determinar os custos de comunicação entre os nos
}

void Cloud::printAllCloud() {
  cout << "****" << endl;
  Host::printAllHosts();
  cout << "****" << endl;
}

