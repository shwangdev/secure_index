#include "cryptopp/base64.h"
#include "cryptopp/base32.h"
#include "cryptopp/des.h"        // DES
#include "cryptopp/modes.h"      // CBC_Mode< >
#include "cryptopp/filters.h"    // StringSource
#include "cryptopp/files.h"
#include <iostream>


using namespace std;



int main(int argc ,char *argv[])
{
    
    
    std::string DncodedText;
    
    std::string EncodedText;
    
    std::string RecoveredText="asdfdsf";
    


    byte key[ CryptoPP::DES::DEFAULT_KEYLENGTH ] ={0x29, 0x4F, 0x5B, 0x4E, 0x42, 0x5D, 0x36, 0x2C};
    
    byte iv[ CryptoPP::DES::BLOCKSIZE ] ={0};
    

    // Encryptor
    CryptoPP::CBC_Mode<CryptoPP::DES>::Encryption Encryptor(key, sizeof(key),iv);
    
   
    // Encryption
    CryptoPP::FileSource( argv[1], true,
                            new CryptoPP::StreamTransformationFilter(Encryptor,
                                                                     new CryptoPP::Base64Encoder(
                                                                         new CryptoPP::StringSink( EncodedText )
                                                                         ) // Base64Encoder
                                ) // StreamTransformationFilter
        );
    // StringSource


    std::cout << "Base64 Encoded Cipher Text:" << std::endl;
    
    std::cout << "   " << EncodedText << std::endl;
    
    std::cout << std::endl;
    
    

    // Decryption
    CryptoPP::CBC_Mode<CryptoPP::DES>::Decryption Decryptor(key, sizeof(key), iv);
    
       
    
    // Decryption
    CryptoPP::StringSource( EncodedText, true,
                            new CryptoPP::Base64Decoder(
                                new CryptoPP::StreamTransformationFilter( Decryptor,
                                                                          new CryptoPP::FileSink(argv[2])
                                    ) // StreamTransformationFilter
                                ) // Base32Decoder
        );
    // StringSource

    std::cout << "Base64 Decoded Cipher Text:" << std::endl;
    
    std::cout << "   " << DncodedText << std::endl;
    
    std::cout << std::endl;
    

    return 0;
    
}
