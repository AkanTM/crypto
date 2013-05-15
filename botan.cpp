#include <botan/botan.h>
#include <botan/whrlpool.h>
#include <botan/base64.h>

#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/time.h>



using namespace Botan;

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " file\n" ;
		return 1;
	}

	LibraryInitializer init;

	
	AutoSeeded_RNG rng;
	HashFunction* hash = get_hash("Whirlpool");

	byte buffer[1025];
	FILE *myfile = fopen(argv[1], "rb");

	if( ! myfile )
	{
		std::cout<< "Could not open file " << argv[1] <<"\n";
		return 1;
	}

	int len=0;	

	while ( (len = fread (buffer, 1, 1024, myfile)) ) 
	{
		hash->update(buffer, len);
	}

	fclose(myfile);

	struct timeval  tv;
	gettimeofday(&tv, NULL);

	double time_start = (tv.tv_sec) * 1000 + tv.tv_usec / 1000.0 ;

	
	std::cout << base64_encode( hash->final() )<<"\n";

	gettimeofday(&tv, NULL);

	double time_stop = (tv.tv_sec) * 1000 + tv.tv_usec / 1000.0 ;

	std::cout << time_stop - time_start <<"\n";

	
	return 0;
}