/*
 * Format.cxx
 *
 *  Created on: Feb 17, 2011
 *      Author: Xiaochuan Qin
 */

#include <ftw.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

int fmtswc(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);

char destroot[1024];
char destpath[2048];
int N = 0;

int main(int argc, char** argv)
{
	if( argc < 3){
		cout << "Usage: "<< argv[0] << " path/to/raw/data/ path/to/new/data/";
		return 1;
	}

	mkdir(argv[2], 0777); // create a directory anyway.
	strcpy(destpath, argv[2]);
	struct stat filestat;
	for( int i = 1; i <= 2; ++i ){
		if( -1 == stat( argv[i], &filestat) ){
			cout << "Error:" << strerror(errno) <<". Error number: "<<errno <<endl;
			return -1;
		}
		if(!S_ISDIR(filestat.st_mode)){
			cout << argv[i] << " is not a valid directory." << endl;
			return -1;
		}
	}
	if( !strcmp(argv[1], argv[2])){
		cout << "Destination and source path cannot be the same."<<endl;
		return 0;
	}

	N = 0;
	if( 0 != nftw( argv[1], fmtswc, 500, 0) ){
		cout << "Error in SWC file modification: "<< strerror(errno) <<". Error number: " << errno << endl;
		return -1;
	}
	cout <<"Totally "<<N <<" SWC files reformatted and copied." << endl;
	return 0;
}

void fmtswc(istream& inswc, ostream& outswc, const string& name, const string& name)
{
	// assuming the file formats are correct, otherwise throw exceptions
	string buffer;
	getline(inswc, buffer);
	while(inswc){ // skip all the comments
		size_t pos = buffer.find_first_not_of(" \t"); // skill possible blank spaces
		if(pos != string::npos && buffer[pos] != '#') // and check if it is a '#'
			break; //stops only when it is a non empty string not starting with #.
		getline( inswc, buffer );
	}

	list<string> data; // buffer all data
	while( inswc ){
		getline(inswc, buffer);
		data.push_back( buffer );
	}

	size_t pos = data.back().find_first_of(" \t"); // assuming all data line begin with numbers
	string largest_id = data.back().substr(0, pos);
	// use the number inside the file instead of our statistics. Just in case there are
	// non-continuous numbering.

	outswc << "# " << type << " " << name << largest_id << endl; // write a new header
	for (list<string>::const_iterator it = data.begin(); it != data.end(); ++it) // write all data
		outswc << *it << endl;
}

int filefmt(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	if( 1 != ftwbuf->level)
		return 0;
	strcpy(destpath, destroot);
	strcat(destpath, "/");
	strcat(destpath, fpath);
	if( typeflag == FTW_F){
		ifstream inswc( fpath);
		ofstream outswc( destpath);
		if( !inswc || !outswc){
			cout <<" Cannot open or create SWC file: "<<fpath<< endl;
			return -1;
		}
		string type(fpath);
		size_t pos = type.find_last_of('/');
		if( pos != string::npos){
			type = type.substr(0, pos);
			pos = type.find_last_of('/');
			if(pos != string::npos)
				type = type.substr(pos+1);
		} // type is the neuron type extracted from fpath


		N++;
	}
	if( typeflag == FTW_D){
	  mkdir(destpath, 0777);
	}
	return 0;
}
