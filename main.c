#include <stdio.h>
#include <stdlib.h>

struct warcaby{
    int rozmiar;
    char** plansza; 
};
struct warcaby* wczytaj(char sciezka[]);

struct warcaby* StworzPlansze();

struct warcaby* Inicjalizuj();

void wypelnijPlansze(struct warcaby* gra);

void zwolnij(struct warcaby* gra);

void Wyswietl(struct warcaby* gra);

void Ruch(struct warcaby* gra);

int Sprawdz(struct warcaby* gra, int wiersz, int kolumna);

void ClrBfr();


//----------------G L O W N A - F U N K C J A--------------------------------//

int main()
{
    printf("To bedzie cudowna gra!\n");
    struct warcaby* gra = Inicjalizuj();
    for(int i = 0; i < 10; i++)
    {
        Wyswietl(gra);
        Ruch(gra);
    }
    
    zwolnij(gra);
    //Start();

    return 0;
}

//----------- I N I C J A L I Z U J ------------------------//

struct warcaby* Inicjalizuj()
{
    struct warcaby* gra;
    int warunek = 0;
    while(warunek == 0)
    {
    printf("\n\tCzy wczytac konfiguracje domyslna[Y/n]?\n");
    char znak;
    scanf("%c",&znak);

    switch (znak)
    {
        case 'Y': case 'y': case 10:
        {
            gra = wczytaj("conf.txt");
            warunek = 1; 
            ClrBfr();  
            break;
        }
        case 'n':
        {
            ClrBfr();
            gra = StworzPlansze(); 
            warunek = 1;
            break;   
        } 
        default:
        {
            ClrBfr();
            printf("Blad, prosze sprobowac ponownie.\n");
            warunek = 0;
            break;
        }    
    }
    }
    
    return gra;
}

//---------------W Y C Z Y S C - B U F O R - W E J S C I A------------------------//
void ClrBfr()
{
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}

//-------------Z W O L N I J - P A M I E C----------------------------//

void zwolnij(struct warcaby* gra)
{
    for(int i = 0; i < gra->rozmiar; i++)
        free(gra->plansza[i]);
    free(gra);
}

//----------------W Y S W I E T L------------------------------//

void Wyswietl(struct warcaby* gra)
{
    printf("\n\tWyswietlanie planszy\n\n");

    printf("   ");

    for(int i = 0; i < gra->rozmiar;i++)
        printf("%2d", i);
    
    printf("\n");    
    
    for(int i = 0; i < gra->rozmiar; i++,printf("\n"))
    {
        printf("%2d|",i);
        for(int j = 0; j< gra->rozmiar; j++)
        {
            printf("%2c",gra->plansza[i][j]);            
        }
    }

    printf("\n");

}

//-------------D O M Y Ś L N A - K O N F I G U R A C J A----------------//

struct warcaby* wczytaj(char sciezka[])
{
    FILE* in = fopen(sciezka,"r");
    struct warcaby* gra = malloc(sizeof(struct warcaby));
    fscanf(in,"%d",&gra->rozmiar);
    gra->plansza = malloc(gra->rozmiar * sizeof(char*));
    for(int i = 0; i < gra->rozmiar; i++)
    {
        gra->plansza[i] = malloc(gra->rozmiar * sizeof(char));
    }

    wypelnijPlansze(gra);

    fclose(in);
    return gra;
}

//---------------S T W O R Z - P L A N S Z E----------------------//

struct warcaby* StworzPlansze()
{
    struct warcaby* gra;

    gra = malloc(sizeof(struct warcaby));
    printf("\nPodaj rozmiar planszy: ");
    scanf("%d", &gra->rozmiar);
    printf("\n%d\n",gra->rozmiar);
    gra->plansza = malloc(gra->rozmiar * sizeof(char*));
    for(int i = 0; i < gra->rozmiar; i++)
        gra->plansza[i] = malloc(gra->rozmiar * sizeof(char)); 

    wypelnijPlansze(gra);    

    return gra;
}

//------------------W Y P E L N I A N I E - P L A N S Z Y-----------------//

void wypelnijPlansze(struct warcaby* gra)
{
    int stosunek=(gra->rozmiar*2)/5;
    for(int i=0;i<gra->rozmiar;i++)
    {    
        for(int j=0;j<gra->rozmiar;j++)
        {          
            gra->plansza[i][j] = '-';          //zapisywanie calej planszy spacjmi
        }
    }

    for(int i=0;i<stosunek;i++)
    {
        for(int j=0;j<gra->rozmiar;j++)
        {
            if((i+j)%2==0)
                gra->plansza[i][j]='-';
            else
                gra->plansza[i][j]='X';           //zapisywanie pierwszych trzech wierszy X-ami
        }
    }

    for(int i=(gra->rozmiar-stosunek);i<gra->rozmiar;i++)
    {
        for(int j=0;j<gra->rozmiar;j++)
        {
            if((i+j)%2==0)
                gra->plansza[i][j]='-';
            else
                gra->plansza[i][j]='O';     //zapisywanie ostatnich trzech wierszy O-ami
        }
    }
}

//----------------------------R U C H----------------------------//

void Ruch(struct warcaby* gra)
{
    int wiersz1,kolumna1,wiersz2,kolumna2;
    do{                                 //wybor piona
    printf("Wybierz pionek\n");
    printf("Wybierz wiersz: ");
    scanf("%d", &wiersz1);
    ClrBfr();
    printf("Wybierz kolumne: ");
    scanf("%d",&kolumna1);
    ClrBfr();
    if(wiersz1 < 0 || wiersz1 > gra->rozmiar || kolumna1 < 0 || kolumna1 > gra->rozmiar)
        continue;
    }while(gra->plansza[wiersz1][kolumna1] == 45);


    do{
    printf("\nWybierz pole\n");                                      //wypor pola
    printf("Wybierz wiersz: ");
    scanf("%d", &wiersz2);
    ClrBfr();
    printf("Wybierz kolumne: ");
    scanf("%d",&kolumna2);
    ClrBfr();
    }while(!Sprawdz(gra,wiersz2,kolumna2));  

    gra->plansza[wiersz2][kolumna2] = gra->plansza[wiersz1][kolumna1];
    gra->plansza[wiersz1][kolumna1] = 45;  
}

//----------------S P R A W D Z A N I E----------------------------//

int Sprawdz(struct warcaby* gra, int wiersz, int kolumna)
{
    if(wiersz < 0 || wiersz > gra->rozmiar || kolumna < 0 || kolumna > gra->rozmiar)
        return 0;
}
