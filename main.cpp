
#include <fstream>
#include <string>
#include <iostream>

#include "Arbre.hpp"

#include <vector>
#include <set>
#include <unordered_map>

struct classcomp {
  bool operator() (const std::string& lhs, const std::string& rhs) const
  {return lhs.size()>=rhs.size();}
};


Arbre myTree;
std::set<std::string> allFound;
std::set<std::string, classcomp> allFound2;
std::unordered_map<std::string, std::vector<uint8_t>> stringToHisto;

void removeAccented( std::string& str ) {
    char *p = str.data();
    while ( (*p)!=0 ) {
        const char*
        //   "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
        tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
        unsigned char ch = (*p);
        if ( ch >=192 ) {
            (*p) = tr[ ch-192 ];
        }
        ++p; // http://stackoverflow.com/questions/14094621/
    }
}

void readAndFillTree(Arbre& tree){
	std::ifstream fichier ("mots.txt", std::ifstream::in);

	std::string ligne;

	while(fichier.good()){

		fichier >> ligne;

		removeAccented(ligne);

		//std::cout << ligne << std::endl;
		tree.add(ligne);
	}


	fichier.close();
}

std::vector<uint8_t> nextPossible(std::string& tab, uint8_t index){

	std::vector<uint8_t> ret;

	//Pas tout en haut
	uint8_t up, down, left, right;


	if(index > 3){
		up = 1;
	}
	else{
		up = 0;
	}

	if(index < 12) down = 1;
	else down = 0;

	if(index%4 > 0) left = 1;
	else left = 0;

	if(index%4 < 3) right = 1;
	else right = 0;


	if(up){
		ret.emplace_back(index - 4);

		if(left){
			ret.emplace_back((index-4) - 1);
		}
		if(right){
			ret.emplace_back((index-4) + 1);
		}

	}

	if(down){
		ret.emplace_back(index + 4);

		if(left){
			ret.emplace_back((index+4) - 1);
		}
		if(right){
			ret.emplace_back((index+4) + 1);
		}

	}

	if(right)
		ret.emplace_back(index + 1);

	if(left)
		ret.emplace_back(index - 1);

	return ret;

}


void findAll(std::string& tab, std::string stack, uint8_t depth, uint8_t index, std::vector<uint8_t>& histo){


	stack.push_back(tab[index]);
	histo[index] = depth;
	

	int next = myTree.find(stack);

	if(next == 0){
		stack.pop_back();
		histo[index] = 0;
		return;
	}
	else if(next == 2){
		allFound.emplace(stack);
		stringToHisto.emplace(stack, histo);
	}


	std::vector<uint8_t> moves(nextPossible(tab, index));

	if(moves.empty()){
		stack.pop_back();
		histo[index] = 0;
		return;
	}

	for(size_t i(0); i < moves.size(); ++i){

		if(histo[moves[i]]){
			continue;
		}

		findAll(tab, stack, depth+1, moves[i], histo);

	}

	stack.pop_back();
	histo[index] = 0;

}



void findAllWord(std::string& tab){
 
	for(size_t i(0); i < 16; ++i){

		std::vector<uint8_t> emptyHisto(16, 0);
		std::string emptyWord;
		findAll(tab, emptyWord, 1, i, emptyHisto);

	}

}



void printTab(std::string const& theString){


	std::vector<uint8_t> &myVec(stringToHisto[theString]);

	// std::cout << "Myvec size : " << myVec.size() << std::endl;
	// for(auto j : myVec){
	// 	std::cout << "Myvec : " << j << std::endl;
	// }



	std::cout << theString << std::endl << std::endl;


	std::cout << "---------" << std::endl;

	for(size_t i(0); i < 4; ++i){

		std::cout << "|";

		for(size_t j(0); j < 4; ++j){
			if(myVec[i * 4 + j] == 0){
				std::cout << " ";
			}
			else if(myVec[i * 4 + j] == 1){
				std::cout << "▓";
			}
			else{
				std::cout << (int)myVec[i * 4 + j];
			}

			std::cout << "|";
		}

		std::cout << std::endl << "---------" << std::endl;

	}



}




int main(int argc, char const *argv[]){
	

	readAndFillTree(myTree);



	//std::string tab = "nbeabelqrlbmabev";
	std::string tab;

	std::cout << "rentrez le tableau : ";
	getline(std::cin, tab);

	//std::cin >> tab;

	//std::string tab = argv[1];
	


	//std::cout << "Mot : " << myTree.find("steatite") << std::endl;

	findAllWord(tab);

	for(auto e : allFound){
		allFound2.emplace(e);
	}

	std::string s;
	for(auto e : allFound2){
		printTab(e);
		getline(std::cin, s);
		//std::cin >> s;
		if(!s.empty()){
			break;
		}
	}




	return 0;
}








