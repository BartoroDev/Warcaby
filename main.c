#include <stdio.h>
#include <stdlib.h>

struct warcaby{
    int rozmiar;
    char** plansza; 
    int gracz;
};
struct warcaby* wczytaj(char sciezka[]);

struct warcaby* StworzPlansze();

struct warcaby* Inicjalizuj();

void wypelnijPlansze(struct warcaby* gra);

void zwolnij(struct warcaby* gra);

void wyswietl(struct warcaby* gra);

void ruch(struct warcaby* gra);

int sprawdz(struct warcaby* gra, int wiersz1, int kolumna1, int wiersz2, int kolumna2);

void zapisz(struct warcaby* gra);

void wyborPola();

void wyborPionka();

void ClrBfr();


//----------------G L O W N A - F U N K C J A--------------------------------//

int main()
{
    printf("To bedzie cudowna gra!\n");
    struct warcaby* gra = Inicjalizuj();
    for(int i = 0; i < 10; i++)
    {

        wyswietl(gra);
        zapisz(gra);
        ruch(gra);
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
    printf("\n\tCzy wczytac zapisana gre[Y/n]?\n");
    char znak;
    scanf("%c",&znak);

    switch (znak)
    {
        case 'Y': case 'y': case 10:
        {
            gra = wczytaj("zapis.txt");
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

void wyswietl(struct warcaby* gra)
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

//-------------W C Z Y T A J - G R E----------------//

struct warcaby* wczytaj(char sciezka[])
{
    FILE* in = fopen(sciezka,"r");
    struct warcaby* gra = malloc(sizeof(struct warcaby));
    fread(&gra->rozmiar,sizeof(int),1,in);
    gra->plansza = malloc(gra->rozmiar * sizeof(char*));
    for(int i = 0; i < gra->rozmiar; i++)
    {
        gra->plansza[i] = malloc(gra->rozmiar * sizeof(char));
    }

    for(int i = 0; i < gra->rozmiar; i++)
    {
        for(int j = 0; j < gra->rozmiar; j++)
        {
            fread(gra->plansza[i]+j,sizeof(char),1,in);
        }
    }
    fread(&gra->gracz,sizeof(int),1,in);

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
    gra->gracz = 0;

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

void ruch(struct warcaby* gra)
{
    int wiersz1,kolumna1,wiersz2,kolumna2,warunek = 1;
    do{                                 //wybor piona
        if(gra->gracz%2==0)
            printf("\nGracz 'O'\n");
        else
            printf("\nGracz 'X'\n");
    
        printf("Wybierz pionek\n");
        printf("Wybierz wiersz: ");
        scanf("%d", &wiersz1);
        ClrBfr();
        printf("Wybierz kolumne: ");
        scanf("%d",&kolumna1);
        ClrBfr();
        if(wiersz1 < 0 || wiersz1 >= gra->rozmiar || kolumna1 < 0 || kolumna1 >= gra->rozmiar)
        {
            warunek = 0;
            continue;
        }
        if(gra->gracz%2==0)
        {
            if(gra->plansza[wiersz1][kolumna1] == 'X')
                warunek = 0;
        }
        else
        {
            if(gra->plansza[wiersz1][kolumna1] == 'O')
                warunek = 0;
        }

    }while(warunek == 0 || gra->plansza[wiersz1][kolumna1] == 45);


    do
    {
        printf("\nWybierz pole\n");                                      //wypor pola
        printf("Wybierz wiersz: ");
        scanf("%d", &wiersz2);
        ClrBfr();
        printf("Wybierz kolumne: ");
        scanf("%d",&kolumna2);
        ClrBfr();
    }while(!sprawdz(gra,wiersz1,kolumna1,wiersz2,kolumna2));  

    gra->plansza[wiersz2][kolumna2] = gra->plansza[wiersz1][kolumna1];
    gra->plansza[wiersz1][kolumna1] = 45;  
    gra->gracz+=1;
}

//----------------S P R A W D Z A N I E----------------------------//

int sprawdz(struct warcaby* gra, int wiersz1, int kolumna1, int wiersz2, int kolumna2)
{
    //wyjscie poza plansze
    if(wiersz2 < 0 || wiersz2 > gra->rozmiar || kolumna2 < 0 || kolumna2 > gra->rozmiar)
        return 0;

    //sprawdzanie czy pole jest puste
    if(gra->plansza[wiersz2][kolumna2] != 45)
        return 0;

    /*
    ustawianie damki
    */  

    //sprawdzanie odleglosci
    int Hor,Ver;
    Hor = kolumna2 - kolumna1;
    Ver = wiersz2 - wiersz1;
    if((Hor > 2 || Hor < -2) || (Ver > 2 || Ver < -2))
        return 0;

    switch(gra->gracz%2)
    {
        case 0:

        if(Ver >= 0)//sprawdzanie kierunku
            return 0;
            
        if((Ver == -1) && (Hor == 1 || Hor == -1)) //przesuwanie o jedno pole
            return 1;

        if(gra->plansza[wiersz1 - 1][kolumna1 + (Hor/2)]=='X') // zbicie
            {
                //printf("\n%d\t%d\n",Hor,Ver);
                gra->plansza[wiersz1 + (Ver/2)][kolumna1 + (Hor/2)]=45;
                
                return 1;
            }
        else 
            return 0;

        break;

        case 1:
        if(Ver <= 0)//sprawdzanie kierunku
            return 0;
        if((Ver == 1) && (Hor == 1 || Hor == -1)) //przesuwanie o jedno pole
            return 1;

        if(gra->plansza[wiersz1 + 1][kolumna1 + (Hor/2)]=='O') // zbicie
            {
                //printf("\n%d\t%d\n",Hor,Ver);
                gra->plansza[wiersz1 + 1][kolumna1 + (Hor/2)]=45;
                //zbicie('X');
                return 1;
            }
        else 
            return 0;

        break;
    }
    return 0;

}

//--------------------Z A P I S Y W A N I E------------------------------------//

void zapisz(struct warcaby* gra)
{
    FILE* out = fopen("zapis.txt","w+");
    fwrite(&gra->rozmiar,sizeof(int),1,out);
    for(int i = 0; i < gra->rozmiar; i++)
    {
        for(int j = 0; j < gra->rozmiar; j++)
        {
            fwrite(gra->plansza[i]+j,sizeof(char),1,out);
        }
    }
    fwrite(&gra->gracz,sizeof(int),1,out);

    fclose(out);    
}
