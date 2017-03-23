#include <C_General.hpp>
#include <C_Trace.hpp>
#include <C_File.hpp>
#include <C_Arguments.hpp>
#include <C_Matrix.hpp>
#include <C_Image.hpp>
#include <C_Arguments.hpp>
#include <iostream>

using namespace std;

//Definicion de los métodos que vamos a utilizar
void RedVecCer(C_Image imagen, int nanchoImg, int naltoImg);
void RedLineal(C_Image imagen, int nanchoImg, int naltoImg);
void Pause();

int main()
{
	
	C_Image imagen;
	double porcentaje;
	string nombre;
	int metodo;
	int anchoImg;
	int altoImg;
	int nanchoImg;
	int naltoImg;
	string nombreImg;

	//Nos debe dar el nombre de la imagen añadiendo también la extensión
	cout << "Introduce el nombre de la imagen que desea redimensionar: " << endl;
	cin >> nombreImg;
	//Transformamos el string en una char para poder usar la funcion ReadBMP
	const char * cadena = nombreImg.c_str();
	imagen.ReadBMP(cadena);

	cout<<"Leyendo imagen"<<endl;

	anchoImg = imagen.LastCol();
	altoImg = imagen.LastRow();

	//Con este bucle vamos a permitir usar el programa varias veces sin tener que parar y volver a ejecutar
	do{

		//Dame el valor al que quieres redimensionar
		// 100% la deja igual -> 50% ponerla a la mitad 200% implica duplicar el tamaño
		cout << "Introduce el porcentage de redimensionamiento (No es necesario insertar %): " <<endl;
		cin >> porcentaje;

		//Pasamos el porcentaje a numero decimal y creamos las nuevas medidas
		porcentaje = porcentaje / 100;
		nanchoImg = anchoImg * porcentaje;
		naltoImg = altoImg * porcentaje;


		//Con este bucle prevenimos de que no se inserten otros valores que no sean los del menú
		do{

			cout << "****Introduce el metodo mediante el que desea redimensionar**** "<<endl;
			cout << "1 --> Interpolacion Lineal" << endl;
			cout << "2 --> Metodo Vecino mas cercano" << endl;
			cout << "3 --> Salir" << endl;
			cin >> metodo;
		} while (metodo < 0 && metodo >= 4);

	
	
		switch (metodo){
		case 1:
			RedLineal(imagen, nanchoImg, naltoImg);
			break;
		case 2:
			RedVecCer(imagen, nanchoImg, naltoImg);
			break;
		case 3:
			cout << "ADIOS" << endl;
			Pause();
			return 0;
			break;
		};//Fin del switch

	} while (true);//Fin del bucle de programa
	return 0;
}//Fin del MAIN


//Redimensionamiento por interpolación Lineal
  //En un punto juntaremos 4 puntos de la imagen anterior
void RedLineal(C_Image imagen, int nanchoImg, int naltoImg){
	C_Image imagen2(1, naltoImg, 1, nanchoImg);

	double x1 = imagen.LastRow() - 1;
	double y1 = imagen.LastCol() - 1;
	double x2 = imagen2.LastRow() - 1;
	double y2 = imagen2.LastCol() - 1;
	double z1 = x1 / x2;
	double z2 = y1 / y2;
	int a = 1;
	int b = 1;


	for (int i = 1; i <= naltoImg; i++){
		for (int j = 1; j <= nanchoImg; j++){
			a = 1;
			b = 1;

			//Con estos if prevenimos salirnos de la matriz a la hora de elegir los puntos
			if (i == naltoImg){
				a = 0;
			}
			if (j == nanchoImg){
				b = 0;
			}
			
			imagen2(i, j) = (
				// Ahora tenemos que hacer esto 4 veces y hacer la media
				//Vamos a elegir un punto y ademas el de su derecha, el de abajo y el diagonal
				//Con estos 4 valores vamos a realizar una media
				imagen(((i - 1)*z1) + 1			,	((j - 1)*z2) + 1)		+
				imagen(((i - 1)*z1) + 1 + a		,	((j - 1)*z2) + 1)		+
				imagen(((i - 1)*z1) + 1			,	((j - 1)*z2) + 1 + b)	+
				imagen(((i - 1)*z1) + 1 + a		,	((j - 1)*z2) + 1 + b) 
				)/4;

		}
	}

	cout << "LISTO" << endl;
	imagen2.WriteBMP("RedLineal.bmp");
	Pause();

}



//Redimensionamiento por vecino más cercano
void RedVecCer(C_Image imagen, int nanchoImg, int naltoImg){
	C_Image imagen2(1, naltoImg, 1, nanchoImg);

	double x1 = imagen.LastRow() - 1;
	double y1 = imagen.LastCol() - 1;
	double x2 = imagen2.LastRow() - 1;
	double y2 = imagen2.LastCol() - 1;
	double z1 = x1 / x2;
	double z2 = y1 / y2;


		for (int i = 1; i <= naltoImg; i++){
			for (int j = 1; j <= nanchoImg; j++){
				imagen2(i, j) = imagen(((i - 1)*z1) + 1
											, 
										((j - 1)*z2) + 1);

			}
		}
		
	cout << "LISTO" << endl;
	imagen2.WriteBMP("RedVecino.bmp");
	Pause();
}
