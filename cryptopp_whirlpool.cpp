#include <cryptopp/whrlpool.h>

#include <cryptopp/base64.h>



#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/time.h>

using namespace CryptoPP;

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " file\n" ;
		return 1;
	}

	Whirlpool hash;

	FILE *myfile = fopen(argv[1], "rb");
	byte buffer[1025];
	int len=0;	

	while ( (len = fread (buffer, 1, 1024, myfile)) ) 
	{
		hash.Update(buffer, len);
	}

	fclose(myfile);

	byte * pbOutputBuffer = (byte*) calloc(sizeof(byte), 128);


	struct timeval  tv;
	gettimeofday(&tv, NULL);

	double time_start = (tv.tv_sec) * 1000 + tv.tv_usec / 1000.0 ;


	hash.Final(pbOutputBuffer);



	gettimeofday(&tv, NULL);

	double time_stop = (tv.tv_sec) * 1000 + tv.tv_usec / 1000.0 ;

	


	Base64Encoder encoder(NULL, false);
	std::string output;
	encoder.Attach( new CryptoPP::StringSink( output ) );
	encoder.Put( pbOutputBuffer, 64 );
	encoder.MessageEnd();

	std::cout << output << "\n";
	std::cout << time_stop - time_start <<"\n";

	return 0;
}