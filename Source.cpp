//Adnan Rashid Intro to computer secrutiy 
//Program to encrypt and decrypt file text with user entered key using RC4 algorithim

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class RC4
{
public:
	RC4();
	void swap(int* S, int i, int j);
	void KSA(char* key);
	char PRGA();
	char* doRC4(char* ciphertext);
private:
	int S[256];

};
RC4::RC4()
{}

void RC4::swap(int* S, int i, int j)
{
	int temp_i, temp_j = 0;
	// Assign temp values for swapping
	temp_i = S[i];
	temp_j = S[j];
	// Reassign values
	S[i] = temp_j;
	S[j] = temp_i;
}

void RC4::KSA(char* key)
{
	int key_length;

	for (int i = 0; i < 256; i++)
	{
		S[i] = i;
	}

	int j = 0;

	key_length = strlen(key);

	for (int i = 0; i < 256; i++)
	{
		j = (j + S[i] + key[i % key_length]) % 256;

		// swap values of S[i] and S[j]
		swap(S, i, j);
	}
}
char RC4::PRGA()
{
	int i = 0;
	int j = 0;
	i = (i + 1) % 256;
	j = (j + S[i]) % 256;

	// swap values of S[i] and S[j]


	int k = S[(S[i] + S[j]) % 256];
	return k;

}
char* RC4::doRC4(char* ciphertext)
{
	int k;
	int ciphertext_len;
	ciphertext_len = strlen(ciphertext);


	for (int i = 0; i <= ciphertext_len; i++)
	{
		k = PRGA();
		ciphertext[i] = ciphertext[i] ^ k;
	}
	return ciphertext;

}


int main()
{
	//input file with provided plaintext stored in text.txt

	ifstream inputFile("text.txt", ios::in && ios::binary);
	string inputKey;
	string text;


	cout << "Enter a 5 to 32 byte key to encrypt and decrypt with: " << endl;
	cin >> inputKey;
	cout << endl;

	// transfering file contents to a string text, tried using other methods so that a char[500] could be exact but none worked
	if (inputFile.is_open())
	{

		getline(inputFile, text);
	}

	char C_Text[500]; //converting the strings from file to char for encryption/decryption


	for (int i = 0; i <= text.length(); i++)
	{

		C_Text[i] = text[i]; //storing in C_Text(original file->encrypted->decrypted)

	}


	char key[32]; //maximum size of 32 but i believe compiler will padd the key even if the key is less than 32 
	for (int i = 0; i <= inputKey.length(); i++)
	{
		key[i] = inputKey[i];

	}
	// objects for enncrypting and decrypting
	RC4 encrypt;
	RC4 decrypt;

	encrypt.KSA(key);
	ofstream results;
	results.open("results.txt");
	cout << "Text to be encrypted: " << "\n" << text << endl;
	char* p = encrypt.doRC4(C_Text);
	int plen = strlen(C_Text);
	//hex conversion

	stringstream ss;
	ss << hex << setfill('0');
	for (int i = 0; i < plen; ++i)
		ss << setw(2) << static_cast<unsigned>(p[i]);
	string mystr = ss.str();
	cout << "Encrytion with the key in hex: " << inputKey << "\n";
	cout << setw(2) << mystr << "\n";
	results << "Encrypted text converted to hex \n" << mystr; // writing encryption to a file called results.txt 
	cout << "Encryption without hex conversion: " << "\n";
	cout << C_Text << "\n";

	//depending on size of key or combination get weird characters not sure why might have something to do with array new lines
	//c++ doesnt offer a way of using something like varchar there is malloc but due to time constraints cant use it 
	// hex conversion cffffcf seems to be related to array size believe due to padding 
	// decrypting the encryption with the same key use doRC4 
	decrypt.KSA(key);
	ofstream decipher;
	decipher.open("decipher.txt");
	//decipher sometimes will carry weird symbols over depending on key
	char* newplaintext = decrypt.doRC4(C_Text);
	cout << "Using the same key the decryption is :" << "\n";
	cout << newplaintext << "\n";
	decipher << newplaintext; // write the decipher to the file

	system("pause");
	return 0;

}