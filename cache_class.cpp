/*
 * cache_class.cpp
 *
 *  Created on: Apr 2, 2022
 *      Author: Andres Correa
 */
#include<iostream>
#include<fstream>
using namespace std;

class Entry {
public:

  //void display(ofstream& outfile);
 Entry(){
     valid = false;
     tag = 0;
	 ref = -1;
	  }
  void set_tag(int _tag) {
	  tag = _tag;
  }
  int get_tag() {
	  return tag;
  }

  void set_valid(bool _valid) {
	  valid = _valid;
  }
  bool get_valid() {
	  return valid;
  }

  void set_ref(int _ref) {
	  ref = _ref;
  }
  int get_ref() {
	  return ref;
  }

  ~Entry();

private:
  bool valid;
  unsigned tag;
  int ref;
};

class Cache {
public:


  //void display(ofstream& outfile);



// set up the cache  and variable that keeps the least recently used.
  Cache(int num_entries , int assoc ) {
	  this->num_entries = num_entries;
	  this->assoc = assoc;
	  this->num_sets = num_entries / assoc ;
	  counter = 0;
	  // variable that keeps least recently used.
	  leastUsed = new int*[num_sets] ;
	  	  for (int i = 0; i < num_sets; i++ ){
	  		leastUsed[i]= new int[assoc];
	  	for (int j = 0; j < assoc; j++){
	  		leastUsed[i][j] = 0;
	  	}
	  	  }

	  entries = new Entry *[num_sets];
	  for (int i = 0; i < num_sets; i++ ){
		  entries[i]= new Entry[assoc];
	  }

  }


  int get_index(unsigned long addr){

	return (addr) % num_sets;
  }

  int get_tag (unsigned long addr){
	  return (addr) / num_sets;
  }

  unsigned long retrieve_addr(int way, int index){

	  unsigned long address = 0;
	  address = entries[way][index].get_ref();

	  return address;
  }
  void update(ofstream& outfile, unsigned long addr){
	  //case empty cache
	  for(int i = 0 ; i < num_sets; i++ ){
	  if(entries[i][get_index(addr)].get_ref() == -1  ){
		  entries[i][get_index(addr)].set_ref(addr);
		  entries[i][get_index(addr)].set_valid(true);
		  counter++;
		  leastUsed[i][get_index(addr)] += counter;
		  outfile << "MISS Compulsory" << endl;
		  return ;
	  }
	 }
	  int min = leastUsed[0][get_index(addr)];
	  // case to discard least used.
	  // first find area with min counter ,
	  for(int i = 1; i < num_sets; i++){
		  if (min >= leastUsed[i][get_index(addr)] ){
			  min = leastUsed[i][get_index(addr)];
		  }
	  }
	  // min hold the spot with least used from the index ( ways)( rows)
	  for(int i = 0; i < num_sets; i ++){
		  if(leastUsed[i][get_index(addr)] == min){
			 entries[i][get_index(addr)].set_ref(addr);
			 entries[i][get_index(addr)].set_valid(true);
			 counter++;
			 leastUsed[i][get_index(addr)] += counter;
		  }
	  }
	  outfile << "MISS" << endl;
  }


  bool hit(ofstream& outfile, unsigned long addr)
  {
	  bool found = false;
	  for (int i = 0; i < num_sets;i++ ){
		  if(entries[i][get_index(addr)].get_valid() && entries[i][get_index(addr)].get_ref() == addr){
			  found = true;
			  outfile << "HIT" << endl;
			  break;
		  }
	  }

	  return found;
  }

  ~Cache(){
 delete leastUsed;
 delete entries;

  }
private:
  int assoc;
  unsigned num_entries;
  int num_sets;
  int **leastUsed;
  int counter;
  Entry **entries;
};

