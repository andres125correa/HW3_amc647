/*
 * cmd_line.cpp
 *
 *  Created on: Apr 2, 2022
 *      Author: Andres Correa
 */
#include<iostream>
#include<fstream>  // need this for file I/O
#include<cstdlib>
#include <String>// for atoi
#include "cache_class.cpp"
using namespace std;

const int MAX_SIZE = 1000;


int main(int argc, char*argv[]) {

  /* check if the correct number of arguments have been passed; bail otherwise */
  if (argc < 4) {
    cout << "Usage: " << endl;
    cout << "   ./cache num_entries associativity filename" << endl;
    return 0;
  }

  /* get args; all args come in as strings; need to convert to int with atoi() */
  unsigned int entries = atoi(argv[1]);
  unsigned int assoc = atoi(argv[2]);

  string input_filename = argv[3];

  /* print the args */
  cout << "Number of entries: " << entries << endl;
  cout << "Associativity: " << assoc << endl;
  cout << "Input File Name: " << input_filename << endl;


  string output_filename = input_filename + "_output";

  /* create input and output files streams */
  ofstream output;
  ifstream input;


  /* open input stream for reading */
  input.open(input_filename);

  /* check if input stream has been succesfully opened; bail otherwise */
  if (!input.is_open()) {
    cerr << "Could not open input file " << input_filename << ". Exiting ..." << endl;
    exit(0);
  }
  unsigned long *nums = new unsigned long[MAX_SIZE];
  int count = 0;
  while (!input.eof() && count < MAX_SIZE) {
    input >> nums[count];
    count++;
  }


  /* done reading from input file; close the stream */
  input.close();

  /* open output file stream for writing */
  output.open(output_filename);

  Cache *cache = new Cache(entries,assoc);
   Entry *entry = new Entry();

  /* no need to check if output file is open; we will create new file if it doesn't exist;
     overwrite otherwise */
  for (int i = 0; i < count; i++){
	   	entry->set_tag(cache->get_tag(nums[i]));
	   	entry->set_ref(nums[i]);
    output << "ADDR : " << nums[i] <<  ": ";
    entry->set_valid(cache->hit(output, nums[i]));
    if (!entry->get_valid()){
    	cache->update(output, nums[i]);
    }
  }

  /* close output stream */
  output.close();

  return 0;
}




