#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "cloud.h"
#include "node.h"
#include "baremetal.h"
#include "event.h"

vector<vector<int>> Cloud::link;

Node* Cloud::newNode( const string name, const int id, const int risingDate, const int bmFamily ) {
  return new Node(name, id, risingDate, bmFamily);
}

void Cloud::readCloudFile( string cloudFileName ) {
  string name;
  int nodeId, nodeRisingDate, bmFamily;
  std::ifstream infile(cloudFileName);
  cout << "Cloud file name: " << cloudFileName << endl;

  infile >> name;
  while( !infile.eof() ) {
    infile >> nodeId >> nodeRisingDate >> bmFamily;
    Cloud::newNode(name, nodeId, nodeRisingDate, bmFamily);
    infile >> name;
  }
}

void Cloud::setLinkSpeeds( int node, string& speeds ) {
  stringstream sIn;
  vector<int> auxv;
  string auxs;
  int number;

  sIn << speeds;

  for( int i = node+1 ; i <= Node::getNbNodes() ; ++i ) {
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
  int node = 0;
  string strIn, strOut;
  std::ifstream infile(networkFileName);
  cout << "Network file name: " << networkFileName << endl;

  std::getline(infile,strIn);
  while( !infile.eof() && node < Node::getNbNodes() ) {
    strOut = extractAllIntegers(strIn);
    if( strOut.size() != 0 ) setLinkSpeeds(node++,strOut);
    std::getline(infile,strIn);
  }
}

void Cloud::printAllLinks() {
  for( int i = 0; i < Node::getNbNodes() ; ++i ) {
    for( int j = 0; j < Node::getNbNodes() ; ++j ) 
      cout << Cloud::bandwidth(i,j) << "*";
    cout << endl;
  }
}

//inline void swap(int &a, int &b ){int aux=a;a=b;b=aux;}

int Cloud::bandwidth( int line, int column ) {
  if( line >= Node::getNbNodes() || column >= Node::getNbNodes() ) return 5;
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
  Node::printAllNodes();
  cout << "****" << endl;
}

