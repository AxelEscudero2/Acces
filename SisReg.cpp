#include <iostream>
#include <bits/stdc++.h>
#include <cctype>

//falta guardar hora
//falta registro en .txt
//falta acceso a anteriores registros(anteriores turnos)
//falta conocimiento de turno
//falta funcion show()

struct ingreso{
		  std::string hora;
		  std::string ingresantes;
		  std::string var1;
		  std::string var2;
		  //std::string var3;
		  //std::string var4;
};

int isNumber(std::string anuel){
	for(char x : anuel)
		if(!std::isdigit(x)) return 0;
	return 1;
}

int isTorre(std::string anuel){
	char a = anuel[0];
	if(('A' <= a && a <= 'C') || ('a' <= a && a <= 'c')) return 1;
	return 0;
}

int isInstruccion(std::string anuel){
	if(anuel == "end" || anuel == "busq") return 1;
	return 0;
}

int main(){
	
	std::vector<struct ingreso>sixSeven;

	while(true){
		std::string ingresante = ""; std::string var1 = ""; std::string var2 = "";
		std::cout << "ingreso:";
		std::string linea;
		std::getline(std::cin, linea);//recibimos la linea completa

		std::stringstream ss(linea);//procesa la linea
		int c = 0;
		if(ss >> ingresante) c++;
		if(ss >> var1) c++;
		if(ss >> var2) c++;

		//aqui inician las instrucciones
		if(isInstruccion(ingresante)){
		
		//instruccion end
			if(c == 1 && ingresante == "end"){
				return 0;
			}
		//instruccion busq
			if(c == 3 && ingresante == "busq"){
				int cont = 0;
				for(auto x : sixSeven){
					if(c == 3 && x.var1 == var1 && x.var2 == var2){
						std::cout << "si se ingreso este turno a las " << x.hora << ", data: " << x.ingresantes << "\n";
						break;
					}
					cont++;
				}
				if(cont == sixSeven.size())
					std::cout << "no hay registro de ingreso\n";
			}
			else if(c == 1 && ingresante == "busq"){
				std::string v1, v2;
				std::cout << "ingrese para buscar: ";
				std::cin >> v1 >> v2;
				int cont = 0;
				for(auto x : sixSeven){
					if(v1 == x.var1 && v2 == x.var2){
						std::cout << "si se ingreso este turno a las " << x.hora << ", data: " << x.ingresantes << "\n";
						break;
					}
					cont++;
				}
				if(cont == sixSeven.size())
					std::cout << "no hay registro de ingreso\n";
				std::cin.ignore();

		////insertar instruccion nueva////
			}
		} else {
		//aqui sigue el ingreso normal
			if(c == 3 && !isInstruccion(ingresante) && isNumber(var1) && isTorre(var2))		//ingreso: propietario 304 c
				sixSeven.push_back({"00:00", ingresante, var1, var2});
			else if(c == 3 && isNumber(ingresante) && isTorre(var1) && !isTorre(var2))		//ingreso: 304 c propietario
				sixSeven.push_back({"00:00", var2, ingresante, var1});
			if(c == 2 && isNumber(ingresante) && isTorre(var1))					//ingreso: 304 c
				sixSeven.push_back({"00:00", "propietario(automatico)", ingresante, var1});
		}
	}
	return 0;
}
