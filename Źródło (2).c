#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	//standardowe wejœcie/wyjœcie
#include <stdlib.h>	//standardowe funkcje, rand(), malloc(), realloc(), itd.
#include <string.h> //do napisów char
#include <time.h>	//do funkcji obs³ugi czasu
#include <locale.h>	//do funkcji setlocale()

//1. Struktura na liczbê zespolon¹.
struct liczba_zespolona //struktura, w tym przypadku jako zmienna globalna(poza main)
						//jest to tylko deklaracja struktury, nie zmiennej strukturalnej
{
	float Re;	//czêœæ rzeczywista
	float Im;	//czêœæ urojona
};

//2. Struktura – element stosu, zawieraj¹ca liczbê zespolon¹ i adres do struktury tego samego typu
struct element	//instrukcja
{
	struct liczba_zespolona dane;
	struct element *poprzedni;	//wskaŸnik – adres poprzedniego elementu stosu
};

//push, pop - postêpujê wed³ug instrukcji
//"Chc¹c zamieniæ wskaŸniki na strukturê wewn¹trz funkcji, nale¿y przekazywaæ do funkcji ich adres, czyli wskaŸnik na wskaŸnik na strukturê."
void push(struct element** gorny, float pierwsza, float druga)
{
	struct element *nowy;
	nowy = malloc(sizeof(struct element));	//alokacja danych
	nowy->dane.Re = pierwsza;				//przypisujemy dane.Re wartoœæ pierwszej podanej liczby
	nowy->dane.Im = druga;
	nowy->poprzedni = *gorny;				//dotychczasowy stos to gorny i poprzedni, odt¹d gorny staje siê poprzednim w nowym stosie
	*gorny = nowy;							//od teraz nasz górny element to nowy
}

void pop(struct element **gorny)
{
	struct element *pomocniczy;
	pomocniczy = (*gorny)->poprzedni;		//do pomocniczego zapisujemy wartoœæ poprzedniego
	free(*gorny);							//usuwamy górê 
	*gorny = pomocniczy;					//teraz nasz górny to ten poprzedni 
}

void suma(struct element **gorny)
{
	struct element *pierwszy, *drugi; 
	struct element *suma;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;						//drugi to element poprzedni, bo sumujemy dwa górne ze stosu
	suma = malloc(sizeof(struct element));				//alokujemy pamiêæ na tê sumê
	suma->dane.Re = pierwszy->dane.Re + drugi->dane.Re; //sumujemy czêœci rzeczywiste
	suma->dane.Im = pierwszy->dane.Im + drugi->dane.Im; //sumujemy czêœci urojone
	/*bo ta operacja robi sumê z dwóch górnych liczb na stosie, i k³adzie zamiast nich tê sumê*/
	pop(gorny);									//œci¹ga górny element, w pop jest zamiana i górny2 to teraz poprzedni górnego1
	pop(gorny);									//œci¹ga wiêc te¿ ten poprzedni górnego1
	push(gorny, suma->dane.Re, suma->dane.Im); //k³adzie sumê
	suma->poprzedni = *gorny;	//teraz poprzedni w sumie to nasz górny element, czyli 3 od góry wczeœniej (poprzedni górnego2)
	free(suma);					//zwalniamy zaalokowan¹ wczeœniej pamiêæ, bo ju¿ jej nie potrzebujemy
}

void roznica(struct element **gorny)
{
	struct element *pierwszy, *drugi;
	struct element *roznica;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;							//drugi to element poprzedni, bo odejmujemy dwa górne ze stosu
	roznica = malloc(sizeof(struct element));				//alokujemy pamiêæ na tê ró¿nicê
	roznica->dane.Re = pierwszy->dane.Re - drugi->dane.Re; //odejmujemy czêœci rzeczywiste
	roznica->dane.Im = pierwszy->dane.Im - drugi->dane.Im; //odejmujemy czêœci urojone
	/*bo ta operacja robi ró¿nicê z dwóch górnych liczb na stosie, i k³adzie zamiast nich tê ró¿nicê*/
	pop(gorny);						//œci¹ga górny element, w pop jest zamiana i górny2 to teraz poprzedni górnego1
	pop(gorny);						//œci¹ga wiêc te¿ ten poprzedni górnego1
	push(gorny, roznica->dane.Re, roznica->dane.Im); //k³adzie ró¿nicê
	roznica->poprzedni = *gorny;	//teraz poprzedni w sumie to nasz górny element, czyli 3 od góry wczeœniej (poprzedni górnego2)
	free(roznica);					//zwalniamy zaalokowan¹ wczeœniej pamiêæ, bo ju¿ jej nie potrzebujemy
}

void iloczyn(struct element **gorny)
{
	struct element *pierwszy, *drugi;
	struct element *iloczyn;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;				//drugi to element poprzedni, bo mno¿ymy dwa górne ze stosu
	iloczyn = malloc(sizeof(struct element));	//alokujemy pamiêæ na ten iloczyn
	iloczyn->dane.Re = 3*(pierwszy->dane.Re * drugi->dane.Re - pierwszy->dane.Im * drugi->dane.Im); //mno¿ymy czêœci rzeczywiste
	iloczyn->dane.Im = 3*( pierwszy->dane.Re * drugi->dane.Im + drugi->dane.Re * pierwszy->dane.Im); //mno¿ymy czêœci urojone
	/*bo ta operacja robi iloczyn z dwóch górnych liczb na stosie, i k³adzie zamiast nich ten iloczyn */
	pop(gorny);						//œci¹ga górny element, w pop jest zamiana i górny2 to teraz poprzedni górnego1
	pop(gorny);						//œci¹ga wiêc te¿ ten poprzedni górnego1
	push(gorny, iloczyn->dane.Re, iloczyn->dane.Im); //k³adzie iloczyn
	iloczyn->poprzedni = *gorny;	//teraz poprzedni w sumie to nasz górny element, czyli 3 od góry wczeœniej (poprzedni górnego2)
	free(iloczyn);					//zwalniamy zaalokowan¹ wczeœniej pamiêæ, bo ju¿ jej nie potrzebujemy
}

void iloraz(struct element **gorny)
{
	struct element *pierwszy, *drugi;
	struct element *iloraz;
	pierwszy = *gorny;
	drugi = (*gorny)->poprzedni;				//drugi to element poprzedni, bo dzielimy dwa górne ze stosu
	iloraz = malloc(sizeof(struct element));	//alokujemy pamiêæ na ten iloraz

	if ((drugi->dane.Re * drugi->dane.Re - drugi->dane.Im * (- drugi->dane.Im ))!= 0) //drugi element, czyli dzielnik nie mo¿e byæ zerem, bo przez to dzielimy w³aœnie
	{
		//wszystko ni¿ej wynika z dzielenia dwóch liczb zespolonych czyli pomno¿enie mianownika i licznika przez sprzê¿enie mianownika
		iloraz->dane.Re = (pierwszy->dane.Re * drugi->dane.Re - pierwszy->dane.Im * (- drugi->dane.Im)) / (drugi->dane.Re * drugi->dane.Re - drugi->dane.Im * (- drugi->dane.Im));
		iloraz->dane.Im = (pierwszy->dane.Im * drugi->dane.Re + pierwszy->dane.Re * (- drugi->dane.Im)) / (drugi->dane.Re * drugi->dane.Re - drugi->dane.Im * (-drugi->dane.Im));
		pop(gorny);								//œci¹ga górny element, w pop jest zamiana i górny2 to teraz poprzedni górnego1
		pop(gorny);								//œci¹ga wiêc te¿ ten poprzedni górnego1
		push(gorny, iloraz->dane.Re, iloraz->dane.Im);//k³adzie iloraz
		iloraz->poprzedni = *gorny;				//teraz poprzedni w sumie to nasz górny element, czyli 3 od góry wczeœniej (poprzedni górnego2)
	}
	else										//jeœli jest zerem
	{
		printf("\nDzielenie przez zero to operacja niedozwolona.\n");
	}
	free(iloraz);								//zwalniamy zaalokowan¹ wczeœniej pamiêæ, bo ju¿ jej nie potrzebujemy
}

void wyswietl_stos(struct element **gorny)
{
	struct element *pomocniczy;
	pomocniczy = *gorny;
	if (pomocniczy != NULL)			//jeœli nie jest pusty wykonaj 
	{
		printf("Wyœwietlam stos:\n");
		while (pomocniczy != NULL)	//wykonuj dopóki nie bêdzie pusty (czyli a¿ bêdzie w koñcu pusty)
		{
			if (pomocniczy->dane.Im >= 0)
				printf("%f + %fi\n", pomocniczy->dane.Re, pomocniczy->dane.Im); //a + bi; f od float
			else
				printf("%f - %fi\n", pomocniczy->dane.Re, pomocniczy->dane.Im);	//a - bi
			pomocniczy = pomocniczy->poprzedni;//i pêtla leci dalej, a¿ nie bêdziemy na dnie i bêdzie pusty
		}

		//wyœwietlenie MENU, bo po wyœwietleniu stosu u¿ytkownik móg³by chcieæ coœ wprowadziæ
		printf("			Kalkulator liczb zespolonych\n                                 MENU WYBORU\nProszê podaæ liczbê rzeczywist¹ i urojon¹ oddzielone spacj¹ i zatwierdziæ klawiszem enter.\n");
		printf("W celu wykonania operacji arytmetycznych na wpisanych liczbach proszê wpisaæ +, -, * lub / i zatwierdziæ klawiszem enter.\n");
		printf("Pozosta³e mo¿liwoœci: AC - wyczyœæ pamiêæ kalkulatora, Z - zakoñcz pracê w programie.\n");
	}
	else
	{
		printf("Nie mo¿na wykonaæ tej operacji.\n");
	}
}

int czy_mozna_wykonac(struct element **gorny)
{
	if (*gorny == NULL)					//sprawdzamy czy s¹ dwie liczby, tu nie ma nawet tej pierwszej 
	{
		printf("Nie mo¿na wykonaæ tej operacji.\n");
		return 0;
	}
	else
	{
		if ((*gorny)->poprzedni == NULL) //to oznacza, ¿e jest tylko jedna liczba (gorny), a do ka¿dej z operacji potrzebne s¹ dwie
		{
			printf("Nie mo¿na wykonaæ tej operacji.\n");
			return 0;
		}
		else							//jeœli jest i pierwsza Re i druga Im to jest ok, zwraca 1
			return 1;
	}
}

int main()
{
	setlocale(LC_ALL, "polish_poland");	//do polskich znaków
	struct element *gorny = NULL;		//deklaracja zmiennej strukturalnej, jest to tablica, w której bêdziemy przechowywaæ struktury
										//wskaŸnik, aby mo¿na by³o ³atwo edytowaæ ewentualne zmiany
										//to jest w³aœnie nasza dynamiczna baza do przechowywania w pamiêci i obs³ugi elementów
										//NULL, bo na poczatku po odpaleniu programu nie ma zadnego obrazu na liscie, jest ona pusta
	float Re, Im;						//Re - czêœæ rzeczywista, Im - czêœæ urojona
	char tablica_znakow[100];

	printf("			Kalkulator liczb zespolonych\n                                 MENU WYBORU\nProszê podaæ liczbê rzeczywist¹ i urojon¹ oddzielone spacj¹ i zatwierdziæ klawiszem enter.\n");
	printf("W celu wykonania operacji arytmetycznych na wpisanych liczbach proszê wpisaæ +, -, * lub / i zatwierdziæ klawiszem enter.\n");
	printf("Pozosta³e mo¿liwoœci: AC - wyczyœæ pamiêæ kalkulatora, Z - zakoñcz pracê w programie.\n");

	int zakonczenie = 0;				//potrzebne, aby póŸniej móc wybraæ Z i za zakoñczenie przyj¹æ 1 aby zakoñczyæ pêtle do..while, takie rzeczy deklarujemy PRZED pêtl¹

	do
	{
		fgets(tablica_znakow, 100, stdin);	//funkcja fgets odczytuje znaki z pliku dopóki nie odczyta podanej iloœci, nie napotka koñca linii lub koñca pliku; zapisuje znaki do podanej tablicy; stdin - wprowadzane z klawiatury
		int ile;							//ile liczb podaje u¿ytkownik
		ile = sscanf(tablica_znakow, "%f %f", &Re, &Im);//sscanf sprawdzi ile u¿ytkownik poda³ danych: 0, 1 czy 2
		/*
			"sscanf() zwraca iloœæ poprawnie dopasowanych do formatu elementów 
			lub - 1 w przypadku b³êdu. Mo¿esz wykorzystaæ ten fakt do interpretacji wprowadzanych danych" - instrukcja
		*/
		if (ile == 2)				//jest podana Re i Im
		{
			if (Re == -0) //jeœli ktoœ wpisze -0 to zamieniamy na 0
			{
				Re = 0;
			}
			if (Im == - 0) //jeœli ktoœ wpisze -0 to zamieniamy na 0
			{
				Im = 0;
			}
			push(&gorny, Re, Im);
			wyswietl_stos(&gorny);	//"Po ka¿dej komendzie aktualny stan stosu powinien byæ wyœwietlany." - instrukcja
		}
		if (ile == 1)				//tylko Re, za Im przyjmujemy 0
		{
			if (Re == -0) //jeœli ktoœ wpisze -0 to zamieniamy na 0
			{
				Re = 0;
			}
			Im = 0;
			push(&gorny, Re, Im);
			wyswietl_stos(&gorny);
		}
		if (ile == 0)	//jeœli nie poda³ liczby to coœ innego, sprawdzamy czy znak operacji artymetycznych lub innych dozwolonych operacji
		{
			switch (tablica_znakow[0])
			{
			case '+':
			{	
				printf("Wybór dodawania.\n");
				if (czy_mozna_wykonac(&gorny) == 1)							//czy mo¿na w ogóle wykonaæ operacjê? sprawdzamy
				{
					suma(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne s¹ dwie liczby, aby móc je dodaæ.\n");//jeœli nie mo¿na
				break;
			}

			case '-':
			{
				printf("Wybór odejmowania.\n");
				if (czy_mozna_wykonac(&gorny) == 1)							//podobnie jak wy¿ej
				{
					roznica(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne s¹ dwie liczby, aby móc je odj¹æ.\n");
				break;
			}

			case '*':
			{
				printf("Wybór mno¿enia.\n");
				if (czy_mozna_wykonac(&gorny) == 1)
				{
					iloczyn(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne s¹ dwie liczby, aby móc je pomno¿yæ.\n");
				break;
			}

			case '/':
			{
				printf("Wybór dzielenia.\n");
				printf("Pamiêtaj, ¿e nie mo¿na dzieliæ przez zero!\n");
				if (czy_mozna_wykonac(&gorny) == 1)
				{
					iloraz(&gorny);
					wyswietl_stos(&gorny);
				}
				else
					printf("Potrzebne s¹ dwie liczby, aby móc je podzieliæ.\n");
				break;
			}

			case 'AC':
			{
				printf("Wybór czyszczenia zawartoœci stosu (pamiêci kalkulatora).\n");
				if (czy_mozna_wykonac(&gorny) == 1)
				{
					while (gorny != NULL)	//jeœli nasza baza danych nie jest pusta to wykonuj operacjê pop, a¿ bêdzie pusta
					{
						pop(&(*gorny));
					}
				}
				else
					printf("Stos ju¿ jest pusty.\n");
				break;
			}

			case 'Z':
			{
				printf("Zakoñczenie pracy z programem.\n");
				while (gorny != NULL)	//jeœli nasza baza danych nie jest pusta to wykonuj operacjê pop, a¿ bêdzie pusta
				{
					pop(&(*gorny));
				}
				printf("Program zakoñczono.\n");
				zakonczenie = 1;		//teraz pêtla do..while siê skoñczy
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