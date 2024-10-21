#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	//standardowe wejście/wyjście
#include <stdlib.h>	//standardowe funkcje, rand(), malloc(), realloc(), itd.
#include <string.h> //do napisów char
#include <time.h>	//do funkcji obsługi czasu
#include <locale.h>	//do funkcji setlocale()

//1. Struktura na liczbę zespoloną.
struct liczba_zespolona //struktura, w tym przypadku jako zmienna globalna(poza main)
						//jest to tylko deklaracja struktury, nie zmiennej strukturalnej
{
	float Re;	//część rzeczywista
	float Im;	//część urojona
};

//2. Struktura – element stosu, zawierająca liczbę zespoloną i adres do struktury tego samego typu
struct element	//instrukcja
{
	struct liczba_zespolona dane;
	struct element *poprzedni;	//wskaźnik – adres poprzedniego elementu stosu
};

//push, pop - postępuję według instrukcji
//"Chcąc zamienić wskaźniki na strukturę wewnątrz funkcji, należy przekazywać do funkcji ich adres, czyli wskaźnik na wskaźnik na strukturę."
void push(struct element** gorny, float pierwsza, float druga)
{
	struct element *nowy;
	nowy = malloc(sizeof(struct element));	//alokacja danych
	nowy->dane.Re = pierwsza;				//przypisujemy dane.Re wartość pierwszej podanej liczby
	nowy->dane.Im = druga;
	nowy->poprzedni = *gorny;				//dotychczasowy stos to gorny i poprzedni, odtąd gorny staje się poprzednim w nowym stosie
	*gorny = nowy;							//od teraz nasz górny element to nowy
}

void pop(struct element **gorny)
{
	struct element *pomocniczy;
	pomocniczy = (*gorny)->poprzedni;		//do pomocniczego zapisujemy wartość poprzedniego
	free(*gorny);							//usuwamy górę 
	*gorny = pomocniczy;					//teraz nasz górny to ten poprzedni 
}

void suma(struct element **gorny)
{
	struct element *pierwszy, *drugi; 
	struct element *suma;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;						//drugi to element poprzedni, bo sumujemy dwa górne ze stosu
	suma = malloc(sizeof(struct element));				//alokujemy pamięć na tę sumę
	suma->dane.Re = pierwszy->dane.Re + drugi->dane.Re; //sumujemy części rzeczywiste
	suma->dane.Im = pierwszy->dane.Im + drugi->dane.Im; //sumujemy części urojone
	/*bo ta operacja robi sumę z dwóch górnych liczb na stosie, i kładzie zamiast nich tę sumę*/
	pop(gorny);									//ściąga górny element, w pop jest zamiana i górny2 to teraz poprzedni górnego1
	pop(gorny);									//ściąga więc też ten poprzedni górnego1
	push(gorny, suma->dane.Re, suma->dane.Im); //kładzie sumę
	suma->poprzedni = *gorny;	//teraz poprzedni w sumie to nasz górny element, czyli 3 od góry wcześniej (poprzedni górnego2)
	free(suma);					//zwalniamy zaalokowaną wcześniej pamięć, bo już jej nie potrzebujemy
}

void roznica(struct element **gorny)
{
	struct element *pierwszy, *drugi;
	struct element *roznica;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;							//drugi to element poprzedni, bo odejmujemy dwa górne ze stosu
	roznica = malloc(sizeof(struct element));				//alokujemy pamięć na tę różnicę
	roznica->dane.Re = pierwszy->dane.Re - drugi->dane.Re; //odejmujemy części rzeczywiste
	roznica->dane.Im = pierwszy->dane.Im - drugi->dane.Im; //odejmujemy części urojone
	/*bo ta operacja robi różnicę z dwóch górnych liczb na stosie, i kładzie zamiast nich tę różnicę*/
	pop(gorny);						//ściąga górny element, w pop jest zamiana i górny2 to teraz poprzedni górnego1
	pop(gorny);						//ściąga więc też ten poprzedni górnego1
	push(gorny, roznica->dane.Re, roznica->dane.Im); //kładzie różnicę
	roznica->poprzedni = *gorny;	//teraz poprzedni w sumie to nasz górny element, czyli 3 od góry wcześniej (poprzedni górnego2)
	free(roznica);					//zwalniamy zaalokowaną wcześniej pamięć, bo już jej nie potrzebujemy
}

void iloczyn(struct element **gorny)
{
	struct element *pierwszy, *drugi;
	struct element *iloczyn;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;				//drugi to element poprzedni, bo mnożymy dwa górne ze stosu
	iloczyn = malloc(sizeof(struct element));	//alokujemy pamięć na ten iloczyn
	iloczyn->dane.Re = 3*(pierwszy->dane.Re * drugi->dane.Re - pierwszy->dane.Im * drugi->dane.Im); //mnożymy części rzeczywiste
	iloczyn->dane.Im = 3*( pierwszy->dane.Re * drugi->dane.Im + drugi->dane.Re * pierwszy->dane.Im); //mnożymy części urojone
	/*bo ta operacja robi iloczyn z dwóch górnych liczb na stosie, i kładzie zamiast nich ten iloczyn */
	pop(gorny);						//ściąga górny element, w pop jest zamiana i górny2 to teraz poprzedni górnego1
	pop(gorny);						//ściąga więc też ten poprzedni górnego1
	push(gorny, iloczyn->dane.Re, iloczyn->dane.Im); //kładzie iloczyn
	iloczyn->poprzedni = *gorny;	//teraz poprzedni w sumie to nasz górny element, czyli 3 od góry wcześniej (poprzedni górnego2)
	free(iloczyn);					//zwalniamy zaalokowaną wcześniej pamięć, bo już jej nie potrzebujemy
}

void iloraz(struct element **gorny)
{
	struct element *pierwszy, *drugi;
	struct element *iloraz;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;				//drugi to element poprzedni, bo dzielimy dwa górne ze stosu
	iloraz = malloc(sizeof(struct element));	//alokujemy pamięć na ten iloraz

	if ((drugi->dane.Re * drugi->dane.Re - drugi->dane.Im * (- drugi->dane.Im ))!= 0) //drugi element, czyli dzielnik nie może być zerem, bo przez to dzielimy właśnie
	{
		//wszystko niżej wynika z dzielenia dwóch liczb zespolonych czyli pomnożenie mianownika i licznika przez sprzężenie mianownika
		iloraz->dane.Re = (pierwszy->dane.Re * drugi->dane.Re - pierwszy->dane.Im * (- drugi->dane.Im)) / (drugi->dane.Re * drugi->dane.Re - drugi->dane.Im * (- drugi->dane.Im));
		iloraz->dane.Im = (pierwszy->dane.Im * drugi->dane.Re + pierwszy->dane.Re * (- drugi->dane.Im)) / (drugi->dane.Re * drugi->dane.Re - drugi->dane.Im * (-drugi->dane.Im));
		pop(gorny);								//ściąga górny element, w pop jest zamiana i górny2 to teraz poprzedni górnego1
		pop(gorny);								//ściąga więc też ten poprzedni górnego1
		push(gorny, iloraz->dane.Re, iloraz->dane.Im);//kładzie iloraz
		iloraz->poprzedni = *gorny;				//teraz poprzedni w sumie to nasz górny element, czyli 3 od góry wcześniej (poprzedni górnego2)
	}
	else										//jeśli jest zerem
	{
		printf("\nDzielenie przez zero to operacja niedozwolona.\n");
	}
	free(iloraz);								//zwalniamy zaalokowaną wcześniej pamięć, bo już jej nie potrzebujemy
}

void wyswietl_stos(struct element **gorny)
{
	struct element *pomocniczy;
	pomocniczy = *gorny;
	if (pomocniczy != NULL)			//jeśli nie jest pusty wykonaj 
	{
		printf("Wyświetlam stos:\n");
		while (pomocniczy != NULL)	//wykonuj dopóki nie będzie pusty (czyli aż będzie w końcu pusty)
		{
			if (pomocniczy->dane.Im >= 0)
				printf("%f + %fi\n", pomocniczy->dane.Re, pomocniczy->dane.Im); //a + bi; f od float
			else
				printf("%f - %fi\n", pomocniczy->dane.Re, pomocniczy->dane.Im);	//a - bi
			pomocniczy = pomocniczy->poprzedni;//i pętla leci dalej, aż nie będziemy na dnie i będzie pusty
		}

		//wyświetlenie MENU, bo po wyświetleniu stosu użytkownik mógłby chcieć coś wprowadzić
		printf("			Kalkulator liczb zespolonych\n                                 MENU WYBORU\nProszę podać liczbę rzeczywistą i urojoną oddzielone spacją i zatwierdzić klawiszem enter.\n");
		printf("W celu wykonania operacji arytmetycznych na wpisanych liczbach proszę wpisać +, -, * lub / i zatwierdzić klawiszem enter.\n");
		printf("Pozostałe możliwości: AC - wyczyść pamięć kalkulatora, Z - zakończ pracę w programie.\n");
	}
	else
	{
		printf("Nie można wykonać tej operacji.\n");
	}
}

int czy_mozna_wykonac(struct element **gorny)
{
	if (*gorny == NULL)					//sprawdzamy czy są dwie liczby, tu nie ma nawet tej pierwszej 
	{
		printf("Nie można wykonać tej operacji.\n");
		return 0;
	}
	else
	{
		if ((*gorny)->poprzedni == NULL) //to oznacza, że jest tylko jedna liczba (gorny), a do każdej z operacji potrzebne są dwie
		{
			printf("Nie można wykonać tej operacji.\n");
			return 0;
		}
		else							//jeśli jest i pierwsza Re i druga Im to jest ok, zwraca 1
			return 1;
	}
}

int main()
{
	setlocale(LC_ALL, "polish_poland");	//do polskich znaków
	struct element *gorny = NULL;		//deklaracja zmiennej strukturalnej, jest to tablica, w której będziemy przechowywać struktury
										//wskaźnik, aby można było łatwo edytować ewentualne zmiany
										//to jest właśnie nasza dynamiczna baza do przechowywania w pamięci i obsługi elementów
										//NULL, bo na poczatku po odpaleniu programu nie ma zadnego obrazu na liscie, jest ona pusta
	float Re, Im;						//Re - część rzeczywista, Im - część urojona
	char tablica_znakow[100];

	printf("			Kalkulator liczb zespolonych\n                                 MENU WYBORU\nProszę podać liczbę rzeczywistą i urojoną oddzielone spacją i zatwierdzić klawiszem enter.\n");
	printf("W celu wykonania operacji arytmetycznych na wpisanych liczbach proszę wpisać +, -, * lub / i zatwierdzić klawiszem enter.\n");
	printf("Pozostałe możliwości: AC - wyczyść pamięć kalkulatora, Z - zakończ pracę w programie.\n");

	int zakonczenie = 0;				//potrzebne, aby później móc wybrać Z i za zakończenie przyjąć 1 aby zakończyć pętle do..while, takie rzeczy deklarujemy PRZED pętlą

	do
	{
		fgets(tablica_znakow, 100, stdin);	//funkcja fgets odczytuje znaki z pliku dopóki nie odczyta podanej ilości, nie napotka końca linii lub końca pliku; zapisuje znaki do podanej tablicy; stdin - wprowadzane z klawiatury
		int ile;							//ile liczb podaje użytkownik
		ile = sscanf(tablica_znakow, "%f %f", &Re, &Im);//sscanf sprawdzi ile użytkownik podał danych: 0, 1 czy 2
		/*
			"sscanf() zwraca ilość poprawnie dopasowanych do formatu elementów 
			lub - 1 w przypadku błędu. Możesz wykorzystać ten fakt do interpretacji wprowadzanych danych" - instrukcja
		*/
		if (ile == 2)				//jest podana Re i Im
		{
			if (Re == -0) //jeśli ktoś wpisze -0 to zamieniamy na 0
			{
				Re = 0;
			}
			if (Im == - 0) //jeśli ktoś wpisze -0 to zamieniamy na 0
			{
				Im = 0;
			}
			push(&gorny, Re, Im);
			wyswietl_stos(&gorny);	//"Po każdej komendzie aktualny stan stosu powinien być wyświetlany." - instrukcja
		}
		if (ile == 1)				//tylko Re, za Im przyjmujemy 0
		{
			if (Re == -0) //jeśli ktoś wpisze -0 to zamieniamy na 0
			{
				Re = 0;
			}
			Im = 0;
			push(&gorny, Re, Im);
			wyswietl_stos(&gorny);
		}
		if (ile == 0)	//jeśli nie podał liczby to coś innego, sprawdzamy czy znak operacji artymetycznych lub innych dozwolonych operacji
		{
			switch (tablica_znakow[0])
			{
			case '+':
			{	
				printf("Wybór dodawania.\n");
				if (czy_mozna_wykonac(&gorny) == 1)							//czy można w ogóle wykonać operację? sprawdzamy
				{
					suma(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne są dwie liczby, aby móc je dodać.\n");//jeśli nie można
				break;
			}

			case '-':
			{
				printf("Wybór odejmowania.\n");
				if (czy_mozna_wykonac(&gorny) == 1)							//podobnie jak wyżej
				{
					roznica(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne są dwie liczby, aby móc je odjąć.\n");
				break;
			}

			case '*':
			{
				printf("Wybór mnożenia.\n");
				if (czy_mozna_wykonac(&gorny) == 1)
				{
					iloczyn(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne są dwie liczby, aby móc je pomnożyć.\n");
				break;
			}

			case '/':
			{
				printf("Wybór dzielenia.\n");
				printf("Pamiętaj, że nie można dzielić przez zero!\n");
				if (czy_mozna_wykonac(&gorny) == 1)
				{
					iloraz(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne są dwie liczby, aby móc je podzielić.\n");
				break;
			}

			case 'AC':
			{
				printf("Wybór czyszczenia zawartości stosu (pamięci kalkulatora).\n");
				if (czy_mozna_wykonac(&gorny) == 1)
				{
					while (gorny != NULL)	//jeśli nasza baza danych nie jest pusta to wykonuj operację pop, aż będzie pusta
					{
						pop(&(*gorny));
					}
				}
				else
					printf("Stos już jest pusty.\n");
				break;
			}

			case 'Z':
			{
				printf("Zakończenie pracy z programem.\n");
				while (gorny != NULL)	//jeśli nasza baza danych nie jest pusta to wykonuj operację pop, aż będzie pusta
				{
					pop(&(*gorny));
				}
				printf("Program zakończono.\n");
				zakonczenie = 1;		//teraz pętla do..while się skończy
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
