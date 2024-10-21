#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	//standardowe wej�cie/wyj�cie
#include <stdlib.h>	//standardowe funkcje, rand(), malloc(), realloc(), itd.
#include <string.h> //do napis�w char
#include <time.h>	//do funkcji obs�ugi czasu
#include <locale.h>	//do funkcji setlocale()

//1. Struktura na liczb� zespolon�.
struct liczba_zespolona //struktura, w tym przypadku jako zmienna globalna(poza main)
						//jest to tylko deklaracja struktury, nie zmiennej strukturalnej
{
	float Re;	//cz�� rzeczywista
	float Im;	//cz�� urojona
};

//2. Struktura � element stosu, zawieraj�ca liczb� zespolon� i adres do struktury tego samego typu
struct element	//instrukcja
{
	struct liczba_zespolona dane;
	struct element *poprzedni;	//wska�nik � adres poprzedniego elementu stosu
};

//push, pop - post�puj� wed�ug instrukcji
//"Chc�c zamieni� wska�niki na struktur� wewn�trz funkcji, nale�y przekazywa� do funkcji ich adres, czyli wska�nik na wska�nik na struktur�."
void push(struct element** gorny, float pierwsza, float druga)
{
	struct element *nowy;
	nowy = malloc(sizeof(struct element));	//alokacja danych
	nowy->dane.Re = pierwsza;				//przypisujemy dane.Re warto�� pierwszej podanej liczby
	nowy->dane.Im = druga;
	nowy->poprzedni = *gorny;				//dotychczasowy stos to gorny i poprzedni, odt�d gorny staje si� poprzednim w nowym stosie
	*gorny = nowy;							//od teraz nasz g�rny element to nowy
}

void pop(struct element **gorny)
{
	struct element *pomocniczy;
	pomocniczy = (*gorny)->poprzedni;		//do pomocniczego zapisujemy warto�� poprzedniego
	free(*gorny);							//usuwamy g�r� 
	*gorny = pomocniczy;					//teraz nasz g�rny to ten poprzedni 
}

void suma(struct element **gorny)
{
	struct element *pierwszy, *drugi; 
	struct element *suma;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;						//drugi to element poprzedni, bo sumujemy dwa g�rne ze stosu
	suma = malloc(sizeof(struct element));				//alokujemy pami�� na t� sum�
	suma->dane.Re = pierwszy->dane.Re + drugi->dane.Re; //sumujemy cz�ci rzeczywiste
	suma->dane.Im = pierwszy->dane.Im + drugi->dane.Im; //sumujemy cz�ci urojone
	/*bo ta operacja robi sum� z dw�ch g�rnych liczb na stosie, i k�adzie zamiast nich t� sum�*/
	pop(gorny);									//�ci�ga g�rny element, w pop jest zamiana i g�rny2 to teraz poprzedni g�rnego1
	pop(gorny);									//�ci�ga wi�c te� ten poprzedni g�rnego1
	push(gorny, suma->dane.Re, suma->dane.Im); //k�adzie sum�
	suma->poprzedni = *gorny;	//teraz poprzedni w sumie to nasz g�rny element, czyli 3 od g�ry wcze�niej (poprzedni g�rnego2)
	free(suma);					//zwalniamy zaalokowan� wcze�niej pami��, bo ju� jej nie potrzebujemy
}

void roznica(struct element **gorny)
{
	struct element *pierwszy, *drugi;
	struct element *roznica;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;							//drugi to element poprzedni, bo odejmujemy dwa g�rne ze stosu
	roznica = malloc(sizeof(struct element));				//alokujemy pami�� na t� r�nic�
	roznica->dane.Re = pierwszy->dane.Re - drugi->dane.Re; //odejmujemy cz�ci rzeczywiste
	roznica->dane.Im = pierwszy->dane.Im - drugi->dane.Im; //odejmujemy cz�ci urojone
	/*bo ta operacja robi r�nic� z dw�ch g�rnych liczb na stosie, i k�adzie zamiast nich t� r�nic�*/
	pop(gorny);						//�ci�ga g�rny element, w pop jest zamiana i g�rny2 to teraz poprzedni g�rnego1
	pop(gorny);						//�ci�ga wi�c te� ten poprzedni g�rnego1
	push(gorny, roznica->dane.Re, roznica->dane.Im); //k�adzie r�nic�
	roznica->poprzedni = *gorny;	//teraz poprzedni w sumie to nasz g�rny element, czyli 3 od g�ry wcze�niej (poprzedni g�rnego2)
	free(roznica);					//zwalniamy zaalokowan� wcze�niej pami��, bo ju� jej nie potrzebujemy
}

void iloczyn(struct element **gorny)
{
	struct element *pierwszy, *drugi;
	struct element *iloczyn;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;				//drugi to element poprzedni, bo mno�ymy dwa g�rne ze stosu
	iloczyn = malloc(sizeof(struct element));	//alokujemy pami�� na ten iloczyn
	iloczyn->dane.Re = 3*(pierwszy->dane.Re * drugi->dane.Re - pierwszy->dane.Im * drugi->dane.Im); //mno�ymy cz�ci rzeczywiste
	iloczyn->dane.Im = 3*( pierwszy->dane.Re * drugi->dane.Im + drugi->dane.Re * pierwszy->dane.Im); //mno�ymy cz�ci urojone
	/*bo ta operacja robi iloczyn z dw�ch g�rnych liczb na stosie, i k�adzie zamiast nich ten iloczyn */
	pop(gorny);						//�ci�ga g�rny element, w pop jest zamiana i g�rny2 to teraz poprzedni g�rnego1
	pop(gorny);						//�ci�ga wi�c te� ten poprzedni g�rnego1
	push(gorny, iloczyn->dane.Re, iloczyn->dane.Im); //k�adzie iloczyn
	iloczyn->poprzedni = *gorny;	//teraz poprzedni w sumie to nasz g�rny element, czyli 3 od g�ry wcze�niej (poprzedni g�rnego2)
	free(iloczyn);					//zwalniamy zaalokowan� wcze�niej pami��, bo ju� jej nie potrzebujemy
}

void iloraz(struct element **gorny)
{
	struct element *pierwszy, *drugi;
	struct element *iloraz;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;				//drugi to element poprzedni, bo dzielimy dwa g�rne ze stosu
	iloraz = malloc(sizeof(struct element));	//alokujemy pami�� na ten iloraz

	if ((drugi->dane.Re * drugi->dane.Re - drugi->dane.Im * (- drugi->dane.Im ))!= 0) //drugi element, czyli dzielnik nie mo�e by� zerem, bo przez to dzielimy w�a�nie
	{
		//wszystko ni�ej wynika z dzielenia dw�ch liczb zespolonych czyli pomno�enie mianownika i licznika przez sprz�enie mianownika
		iloraz->dane.Re = (pierwszy->dane.Re * drugi->dane.Re - pierwszy->dane.Im * (- drugi->dane.Im)) / (drugi->dane.Re * drugi->dane.Re - drugi->dane.Im * (- drugi->dane.Im));
		iloraz->dane.Im = (pierwszy->dane.Im * drugi->dane.Re + pierwszy->dane.Re * (- drugi->dane.Im)) / (drugi->dane.Re * drugi->dane.Re - drugi->dane.Im * (-drugi->dane.Im));
		pop(gorny);								//�ci�ga g�rny element, w pop jest zamiana i g�rny2 to teraz poprzedni g�rnego1
		pop(gorny);								//�ci�ga wi�c te� ten poprzedni g�rnego1
		push(gorny, iloraz->dane.Re, iloraz->dane.Im);//k�adzie iloraz
		iloraz->poprzedni = *gorny;				//teraz poprzedni w sumie to nasz g�rny element, czyli 3 od g�ry wcze�niej (poprzedni g�rnego2)
	}
	else										//je�li jest zerem
	{
		printf("\nDzielenie przez zero to operacja niedozwolona.\n");
	}
	free(iloraz);								//zwalniamy zaalokowan� wcze�niej pami��, bo ju� jej nie potrzebujemy
}

void wyswietl_stos(struct element **gorny)
{
	struct element *pomocniczy;
	pomocniczy = *gorny;
	if (pomocniczy != NULL)			//je�li nie jest pusty wykonaj 
	{
		printf("Wy�wietlam stos:\n");
		while (pomocniczy != NULL)	//wykonuj dop�ki nie b�dzie pusty (czyli a� b�dzie w ko�cu pusty)
		{
			if (pomocniczy->dane.Im >= 0)
				printf("%f + %fi\n", pomocniczy->dane.Re, pomocniczy->dane.Im); //a + bi; f od float
			else
				printf("%f - %fi\n", pomocniczy->dane.Re, pomocniczy->dane.Im);	//a - bi
			pomocniczy = pomocniczy->poprzedni;//i p�tla leci dalej, a� nie b�dziemy na dnie i b�dzie pusty
		}

		//wy�wietlenie MENU, bo po wy�wietleniu stosu u�ytkownik m�g�by chcie� co� wprowadzi�
		printf("			Kalkulator liczb zespolonych\n                                 MENU WYBORU\nProsz� poda� liczb� rzeczywist� i urojon� oddzielone spacj� i zatwierdzi� klawiszem enter.\n");
		printf("W celu wykonania operacji arytmetycznych na wpisanych liczbach prosz� wpisa� +, -, * lub / i zatwierdzi� klawiszem enter.\n");
		printf("Pozosta�e mo�liwo�ci: AC - wyczy�� pami�� kalkulatora, Z - zako�cz prac� w programie.\n");
	}
	else
	{
		printf("Nie mo�na wykona� tej operacji.\n");
	}
}

int czy_mozna_wykonac(struct element **gorny)
{
	if (*gorny == NULL)					//sprawdzamy czy s� dwie liczby, tu nie ma nawet tej pierwszej 
	{
		printf("Nie mo�na wykona� tej operacji.\n");
		return 0;
	}
	else
	{
		if ((*gorny)->poprzedni == NULL) //to oznacza, �e jest tylko jedna liczba (gorny), a do ka�dej z operacji potrzebne s� dwie
		{
			printf("Nie mo�na wykona� tej operacji.\n");
			return 0;
		}
		else							//je�li jest i pierwsza Re i druga Im to jest ok, zwraca 1
			return 1;
	}
}

int main()
{
	setlocale(LC_ALL, "polish_poland");	//do polskich znak�w
	struct element *gorny = NULL;		//deklaracja zmiennej strukturalnej, jest to tablica, w kt�rej b�dziemy przechowywa� struktury
										//wska�nik, aby mo�na by�o �atwo edytowa� ewentualne zmiany
										//to jest w�a�nie nasza dynamiczna baza do przechowywania w pami�ci i obs�ugi element�w
										//NULL, bo na poczatku po odpaleniu programu nie ma zadnego obrazu na liscie, jest ona pusta
	float Re, Im;						//Re - cz�� rzeczywista, Im - cz�� urojona
	char tablica_znakow[100];

	printf("			Kalkulator liczb zespolonych\n                                 MENU WYBORU\nProsz� poda� liczb� rzeczywist� i urojon� oddzielone spacj� i zatwierdzi� klawiszem enter.\n");
	printf("W celu wykonania operacji arytmetycznych na wpisanych liczbach prosz� wpisa� +, -, * lub / i zatwierdzi� klawiszem enter.\n");
	printf("Pozosta�e mo�liwo�ci: AC - wyczy�� pami�� kalkulatora, Z - zako�cz prac� w programie.\n");

	int zakonczenie = 0;				//potrzebne, aby p�niej m�c wybra� Z i za zako�czenie przyj�� 1 aby zako�czy� p�tle do..while, takie rzeczy deklarujemy PRZED p�tl�

	do
	{
		fgets(tablica_znakow, 100, stdin);	//funkcja fgets odczytuje znaki z pliku dop�ki nie odczyta podanej ilo�ci, nie napotka ko�ca linii lub ko�ca pliku; zapisuje znaki do podanej tablicy; stdin - wprowadzane z klawiatury
		int ile;							//ile liczb podaje u�ytkownik
		ile = sscanf(tablica_znakow, "%f %f", &Re, &Im);//sscanf sprawdzi ile u�ytkownik poda� danych: 0, 1 czy 2
		/*
			"sscanf() zwraca ilo�� poprawnie dopasowanych do formatu element�w 
			lub - 1 w przypadku b��du. Mo�esz wykorzysta� ten fakt do interpretacji wprowadzanych danych" - instrukcja
		*/
		if (ile == 2)				//jest podana Re i Im
		{
			if (Re == -0) //je�li kto� wpisze -0 to zamieniamy na 0
			{
				Re = 0;
			}
			if (Im == - 0) //je�li kto� wpisze -0 to zamieniamy na 0
			{
				Im = 0;
			}
			push(&gorny, Re, Im);
			wyswietl_stos(&gorny);	//"Po ka�dej komendzie aktualny stan stosu powinien by� wy�wietlany." - instrukcja
		}
		if (ile == 1)				//tylko Re, za Im przyjmujemy 0
		{
			if (Re == -0) //je�li kto� wpisze -0 to zamieniamy na 0
			{
				Re = 0;
			}
			Im = 0;
			push(&gorny, Re, Im);
			wyswietl_stos(&gorny);
		}
		if (ile == 0)	//je�li nie poda� liczby to co� innego, sprawdzamy czy znak operacji artymetycznych lub innych dozwolonych operacji
		{
			switch (tablica_znakow[0])
			{
			case '+':
			{	
				printf("Wyb�r dodawania.\n");
				if (czy_mozna_wykonac(&gorny) == 1)							//czy mo�na w og�le wykona� operacj�? sprawdzamy
				{
					suma(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne s� dwie liczby, aby m�c je doda�.\n");//je�li nie mo�na
				break;
			}

			case '-':
			{
				printf("Wyb�r odejmowania.\n");
				if (czy_mozna_wykonac(&gorny) == 1)							//podobnie jak wy�ej
				{
					roznica(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne s� dwie liczby, aby m�c je odj��.\n");
				break;
			}

			case '*':
			{
				printf("Wyb�r mno�enia.\n");
				if (czy_mozna_wykonac(&gorny) == 1)
				{
					iloczyn(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne s� dwie liczby, aby m�c je pomno�y�.\n");
				break;
			}

			case '/':
			{
				printf("Wyb�r dzielenia.\n");
				printf("Pami�taj, �e nie mo�na dzieli� przez zero!\n");
				if (czy_mozna_wykonac(&gorny) == 1)
				{
					iloraz(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne s� dwie liczby, aby m�c je podzieli�.\n");
				break;
			}

			case 'AC':
			{
				printf("Wyb�r czyszczenia zawarto�ci stosu (pami�ci kalkulatora).\n");
				if (czy_mozna_wykonac(&gorny) == 1)
				{
					while (gorny != NULL)	//je�li nasza baza danych nie jest pusta to wykonuj operacj� pop, a� b�dzie pusta
					{
						pop(&(*gorny));
					}
				}
				else
					printf("Stos ju� jest pusty.\n");
				break;
			}

			case 'Z':
			{
				printf("Zako�czenie pracy z programem.\n");
				while (gorny != NULL)	//je�li nasza baza danych nie jest pusta to wykonuj operacj� pop, a� b�dzie pusta
				{
					pop(&(*gorny));
				}
				printf("Program zako�czono.\n");
				zakonczenie = 1;		//teraz p�tla do..while si� sko�czy
				break;
			}

			default:					//inny znak + kopia fragmentu MENU
			{
				printf("Niestety tej opcji nie ma w MENU.\n\nAby dodac liczby zespolone do stosu wprowadzaj liczby oddzielone spacja\nAby wykonac operacje arytmetyczne wpisz odpowiednio +, -, *, /\nInne operacje: k-koniec programu, w-wyczysc stos, u-usun ostatni element\n\n");
			}
			}
		}
	} while (zakonczenie != 1);
	return 0;
}