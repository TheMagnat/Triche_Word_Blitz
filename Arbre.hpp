

#include <unordered_map>
#include <string>
#include <iostream>


class Arbre{
	public:
		
		Arbre() : end(false) {



		}


		//return 0 if you can't continue, 1 if you can, 2 if it's a word
		int find(std::string const& mot){


			Arbre *node(this);

			//std::cout << "Mot : " << mot << std::endl;

			for(size_t i(0); i < mot.size(); ++i){

				//std::cout << "Lettre : " << mot[i] << std::endl;

				if(node->next.find(mot[i]) == node->next.end()){
					return 0;
				}

				node = &node->next[mot[i]];


			}

			if(node->end){
				return 2;
			}

			return 1;

		}


		void add(std::string const& mot){

			Arbre *node(this);

			//std::cout << "Mot : " << mot << std::endl;

			for(size_t i(0); i < mot.size(); ++i){


				//std::cout << "CHar : " << mot[i] << std::endl;

				if(node->next.find(mot[i]) == node->next.end()){
					node->next.emplace(mot[i], Arbre());
				}

				node = &node->next[mot[i]];

				//std::cout << "Char : " << (int)mot[i] << std::endl;

			}

			node->end = true;


		}


	private:

		bool end;

		std::unordered_map<char, Arbre> next;


};



/*

stiz
wetr
ueau
cpts

*/