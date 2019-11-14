#include <stdio.h>
#include <stdlib.h>

struct warcaby{
    int rozmiar;
    int** plansza; 
};

struct warcaby* StworzPlansze();

void Inicjalizuj();


int main()
{
    printf("To bedzie cudowna gra!\n");


    Inicjalizuj();
    //Start();

    return 0;
}

//----------- I N I C J A L I Z U J ------------------------//

void Inicjalizuj()
{
    struct warcaby* gra;

    printf("\n\tCzy wczytac konfiguracje domyslna[Y/n]?\n");
    char znak = getchar();

    switch (znak)
    {
    case 'Y': case 'y': case 10:
        gra = wczytaj("conf.txt");
        break;
    
    case 'n':
        gra = StworzPlansze(); 
        break;
    }

}

//--------------- S T W O R Z - P L A N S Z E----------------------//

struct warcaby* StworzPlansze()
{
    struct warcaby* gra;

    gra = malloc(sizeof(struct warcaby));
    printf("\nPodaj rozmiar planszy: ");
    gra->rozmiar=getchar();
    gra->plansza = malloc(gra->rozmiar * sizeof(int));
    for(int i = 0; i < gra->rozmiar; i++)
        gra->plansza[i]=malloc(gra->rozmiar * sizeof(int));

    return gra;
}