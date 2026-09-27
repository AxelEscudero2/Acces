#include <iostream>
#include <bits/stdc++.h>
#include <cctype>
#include <fstream>
#include <sstream>

struct ingreso{
		  int h;
		  int min;
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
	if(anuel == "end" || anuel == "busq" || anuel == "save"|| anuel == "load") return 1;
	return 0;
}

void escribirEnRegistro(const std::string& ruta, const std::string& texto) {
    std::ofstream archivo(ruta, std::ios::app); // std::ios::app evita que se borre el contenido anterior
    if (archivo.is_open()) {
        archivo << texto << "\n";
        archivo.close();
    } else {
        std::cerr << "Error al abrir o crear el archivo para escribir." << std::endl;
    }
}

void guardar(std::vector<struct ingreso> sixseven, const std::string& ruta){
	std::ofstream archivo(ruta, std::ios::app);
	for(auto x: sixseven){
		std::string c = (x.min < 10)? "0":"";
		archivo << "ingreso a las " << x.h << ":" << c << x.min << " a dptto " << x.var1 << x.var2 << ", data: " << x.ingresantes << "\n";
	}
	archivo.close();

}

void load(std::vector<struct ingreso>& sixseven, const std::string& ruta) {
    std::ifstream archivo(ruta);

    std::string linea;
    // Procesamos el archivo línea por línea
    while(std::getline(archivo, linea)){
        struct ingreso nuevoIngreso;
        int hora = 0, minuto = 0;
        
        // Buscamos la posición de las etiquetas clave para recortar los strings de forma segura
        size_t posHora = linea.find("ingreso a las ") + 14;
        size_t posDpto = linea.find(" a dptto ") + 9;
        size_t posData = linea.find("data: ") + 6;

        // 1. Extraer Hora y Minutos usando un stringstream intermedio
        std::string seccionTiempo = linea.substr(posHora, posDpto - 9 - posHora); // Extrae "HH:MM"
        char dosPuntos;
        std::stringstream ssTiempo(seccionTiempo);
        ssTiempo >> nuevoIngreso.h >> dosPuntos >> nuevoIngreso.min;

        // 2. Extraer Departamento (var1) y Torre (var2)
        std::string seccionDpto = linea.substr(posDpto, posData - 6 - posDpto); // Extrae el número y la torre (ej: "304c ")
        std::stringstream ssDpto(seccionDpto);
        ssDpto >> nuevoIngreso.var1 >> nuevoIngreso.var2;

        // 3. Extraer los Ingresantes (el resto de la línea)
        nuevoIngreso.ingresantes = linea.substr(posData);

        // Insertamos el registro recuperado de vuelta en nuestro vector operativo
        sixseven.push_back(nuevoIngreso);
    }
    archivo.close();
    //std::cout << "Se cargaron " << sixseven.size() << " registros previos del archivo.\n";
}

int main(){
	
	std::vector<struct ingreso>sixSeven;
	std::string nombre = "registro.txt";
	//:escribirEnRegistro(nombre, "inicio de turno noche");

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
					if(x.var1 == var1 && x.var2 == var2 || x.var1 == var2 && x.var2 == var1){
						std::string p = (x.min < 10)?"0":"";
						std::cout << "hubo ingreso a las " << x.h << ":" << p << x.min << ", data: " << x.ingresantes << "\n";
						cont++;
					}
					
				}
				if(cont == 0)
					std::cout << "no hay registro de ingreso\n";
			}
			if(c == 1 && ingresante == "busq"){
				std::string v1, v2;
				std::cout << "ingrese para buscar: ";
				std::cin >> v1 >> v2;
				int cont = 0;
				for(auto x : sixSeven){
					if(v1 == x.var1 && v2 == x.var2 || v1 == x.var2 && v2 == x.var1){
						std::string p = (x.min < 10)?"0":"";
						std::cout << "hubo ingreso a las " << x.h << ":" << p << x.min << ", data: " << x.ingresantes << "\n";
						cont++;
					}
				}
				if(cont == 0)
					std::cout << "no hay registro de ingreso\n";
				std::cin.ignore();
			}
		//instruccion save
			if(c == 1 && ingresante == "save"){
				guardar(sixSeven, nombre);
				sixSeven = {};
				sixSeven.clear();
			}
		//instruccion load
			if(c == 1 && ingresante == "load")
				load(sixSeven, nombre);

		////insertar instruccion nueva////
		} else {
		//aqui sigue el ingreso normal
			if(c == 3 && !isInstruccion(ingresante) && isNumber(var1) && isTorre(var2)){		//ingreso: propietario 304 c
				auto ahora = std::chrono::system_clock::now();
				std::time_t tiempoC = std::chrono::system_clock::to_time_t(ahora);
				std::tm* tiempoLocal = std::localtime(&tiempoC);
				sixSeven.push_back({(tiempoLocal->tm_hour) % 12, tiempoLocal->tm_min, ingresante, var1, var2});
			}
			else if(c == 3 && isNumber(ingresante) && isTorre(var1) && !isTorre(var2)){		//ingreso: 304 c propietario
				auto ahora = std::chrono::system_clock::now();
				std::time_t tiempoC = std::chrono::system_clock::to_time_t(ahora);
				std::tm* tiempoLocal = std::localtime(&tiempoC);
				sixSeven.push_back({(tiempoLocal->tm_hour) % 12, tiempoLocal->tm_min, var2, ingresante, var1});
			}
			if(c == 2 && isNumber(ingresante) && isTorre(var1)){					//ingreso: 304 c
				auto ahora = std::chrono::system_clock::now();
				std::time_t tiempoC = std::chrono::system_clock::to_time_t(ahora);
				std::tm* tiempoLocal = std::localtime(&tiempoC);
				sixSeven.push_back({(tiempoLocal->tm_hour) % 12, tiempoLocal->tm_min, "propietario(automatico)", ingresante, var1});
			}
		}
	}
	return 0;
}
