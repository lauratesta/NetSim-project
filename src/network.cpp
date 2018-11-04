#include <iostream>
#include "network.h"
#include <map>
#include <vector>
#include "random.h"
#include <algorithm>


void Network::resize(const size_t& size){
	values = std::vector<double> (size);
	RandomNumbers RNG;
	RNG.normal(values);	// on remplit le tableau de valurs aléatoires
}

bool Network::add_link(const size_t& a, const size_t& b){
		bool exist;	//vérification de l'existence du lien
		std::vector<long unsigned int> tmp = neighbors(a); // on stock tous les voisisn de a et on regarde si b en fait partie
			for (auto noeud : tmp){
				if (noeud == b){
					exist = true;
				} else exist = false;
			}
		if ( (values.size()>a) and (values.size()>b) and (b!=a) and (not exist) ){ 
			links.insert(std::make_pair(a,b));
			links.insert(std::make_pair(b,a));
		return true; 
		}
	 else return false;
}

size_t Network::random_connect(const double& mean_deg){
	links.erase(links.begin(), links.end());
	size_t  nodes(0); //compteur de liens
	for (size_t i(0); i < values.size(); ++i){ // on parcourt chaque noeud du tableau pour lequel le nombre de liens et les liens seront aléatoires
		int nb_links = RNG.poisson(mean_deg); 
		for (int j(0); j<nb_links; ++j) { 
			std::vector<int> linked (1); // on veut qu'il n'ait qu'un élément 
			do {
				RNG.uniform_int(linked, 0, values.size() - 1); 
			} while (add_link( i, linked[0]) != true); //la méthode add_link s'occupe déja d'établir le lien bidirectionnel donc de placer dans la multimap les liens entre i et l'élément du tableau
			++nodes;
		}
	}
	return nodes;
}

size_t Network::set_values(const std::vector<double>& val){
	if ( val.size() >= values.size()){ // si le vector en paramètre est plus grand, on prend n valeurs de ce vector pour les mettre dans notre attribut values ( n étant la taille de values)
		for (size_t i(0); i<values.size(); ++i){
			values[i] = val[i];
		} return values.size();
	} else { 	//si notre vector orignial de noeud est plus grand, on ne reset seulement les n premières valeurs correspondant à toutes les valeurs du vector en paramètre
		for (size_t i(0); i<val.size(); ++i){
			values[i] = val[i];
		} return val.size();
	}
}


size_t Network::size() const{
	return values.size();
}

size_t Network::degree(const size_t &_n) const{
	if (not links.empty()) {
		return links.count(_n); // retourne le nombre de fois que _n apparait dans les key de la multimap, et donc le nombre de lien que le noeud correspondant à _n  possède
	} else return 0;
}

double Network::value(const size_t &_n) const{
	if (values.size() > _n){
		return values[_n];
	} else { 
		std::cerr<<" ce noeud n'existe pas " << std::endl;
		return 0;
	}
}

std::vector<double> Network::sorted_values() const{
	std::vector<double> sorted_val = values;	//tableau qui va ontenir les valeurs des noeuds en ordre décroissant
	std::sort(sorted_val.begin(), sorted_val.end());	//tri en ordre croissant (bibliotheque algorithm)
	std::reverse(sorted_val.begin(), sorted_val.end());		//inverse les éléments pour être en ordre décroissant
	return sorted_val;
}
	

std::vector<size_t> Network::neighbors(const size_t& n) const{
	std::vector<size_t> neighbors_(0);
	if (not  links.empty()){
		auto range = links.equal_range(n);	//stock les voisins entre deux iterateurs
		for (auto i = range.first; i != range.second; ++i) { 
			neighbors_.push_back(i->second); // i.second représente la valeur
		}	
	} return neighbors_;	// si la multimap est vide, on retourne le tableau vide, sinon le tableau rempli des voisins
}




