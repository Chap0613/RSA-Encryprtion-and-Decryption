#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

struct Number_Pair {
    int first_num = 0;
    int second_num = 0;
};

vector<char> convert_key();
int exponent(int base, int power, int mod);

int main(int argc, char* argv[])
{
    Number_Pair public_key;
    public_key.first_num = atoi(argv[1]);
    public_key.second_num = atoi(argv[2]);

    Number_Pair n_factors;
//Find factors of n
    int check_factors = sqrt(public_key.second_num);
    for (int i = 2; i <= check_factors; i++) {
        if (public_key.second_num % i == 0) {
            n_factors.first_num = i;
            i = check_factors + 1;
        }
    }
    n_factors.second_num = public_key.second_num / n_factors.first_num;
    int num_coprime = (n_factors.first_num - 1) * (n_factors.second_num - 1);

    if (*argv[3] == 'd') {
//Find d
        int d = 1;
        for (int i = 1; i < num_coprime; i++) {
            int temp = public_key.first_num * i;
            if (temp % num_coprime == 1) {
                d = i;
            }
        }
//Read in and convert numbers in vector
        vector<int> message;
        int input;
        string filename = argv[4];
        ifstream inFS;
        inFS.open(filename.c_str());
        if (!inFS) {
            cout << "Could not open " << filename << endl;
            return 1;
        }
        while(!inFS.eof()) {
            inFS >> input;
            message.push_back(exponent(input,d,public_key.second_num));
        }
        //message.pop_back();
        inFS.close();
//output vector into file
        vector<char> key = convert_key();
        ofstream outFS;
        outFS.open("decrypted.txt");
        if (!outFS.is_open()) {
            cout << "decrypted.txt not open" << endl;
            return 1;
        }

        for(int i = 0; i < message.size(); i++) {
            outFS << key.at(message.at(i));
        }
        outFS.close();
	cout << "decrypted.txt" << endl;
        return 0;
    }
    else if (*argv[3] == 'e') {
//Store and encode message into vector
        vector<int> message;
        vector<char> key = convert_key();
	string filename = argv[4];
	ifstream inFS;
        inFS.open(filename.c_str());
        if (!inFS) {
            cout << "Could not open " << filename << endl;
            return 1;
        }

	string words;
        while(!inFS.eof()) {
		getline(inFS,words);
		for (int i = 0; i < words.size(); i++) {
			for (int j = 0; j < key.size(); j++) {
				if (words.at(i) == key.at(j)) {
					message.push_back(exponent(j,public_key.first_num,public_key.second_num));
				}
			}
		}
        }
	//message.pop_back();
        inFS.close();

//output vector into file
        ofstream outFS;
        outFS.open("incrypted.txt");
        if (!outFS.is_open()) {
            cout << "incrypted.txt not open" << endl;
            return 1;
        }

        for(int i = 0; i < message.size(); i++) {
            outFS << message.at(i) << " ";
        }
        outFS.close();
	cout << "incrypted.txt" << endl;
        return 0;
    }
}


int exponent(int base, int power, int mod){
    if (power == 0) {
        return 1;
    }
    else if (power == 1) {
        return base % mod;
    }
    int result = 1;
    while (power > 0) {
        if (power % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        power = power / 2;
    }
    return result;
}

vector<char> convert_key() {
    vector<char> key;
    key.push_back('!');
    key.push_back('=');
    for (int i = 0; i < 26; i++) {
        key.push_back('A' + i);
    }
    key.push_back(' ');
    for (int i = 0; i < 26; i++) {
        key.push_back('a' + i);
    }
    for (int i = 0; i < 10; i++) {
        key.push_back('0'+i);
    }
    for (int i = 0 ; i < 13; i++) {
        key.push_back('#' + i);
    }
    return key;
}
