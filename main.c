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

int sprawdzPionka(struct warcaby* gra, int w1, int k1, int w2, int k2);

int sprawdzDamke(struct warcaby* gra, int w1, int k1, int w2, int k2);

int MozliwoscRuchu(struct warcaby* gra,int wiersz1,int kolumna1);

int PustePole(struct warcaby* gra,int wiersz1,int kolumna1);

int Czymozna(struct warcaby* gra,int wiersz1,int kolumna1);

int CzymoznaKraw(struct warcaby* gra,int wiersz1,int kolumna1);

int CzyIstniejeBicie(struct warcaby* gra,int wiersz1,int kolumna1);

int MozliwoscRuchuDamki(struct warcaby* gra, int wiersz1,int kolumna1);

int MozliwoscBiciaDamka(struct warcaby* gra, int w, int k, int kierunek);

void zapisz(struct warcaby* gra);

void wyborPola();

void wyborPionka();

void ClrBfr();

int zbicie = 0;

int pozX,pozY;


//----------------G L O W N A - F U N K C J A--------------------------------//

int main()
{
    printf("To bedzie cudowna gra!\n");
    struct warcaby* gra = Inicjalizuj();
    for(int i = 0; i < 100; i++)
    {
        wyswietl(gra);
        ruch(gra);
        zapisz(gra);
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
    while-((c = getchar()) != '\n' && c != EOF);
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
    //fread(&gra->rozmiar,sizeof(int),1,in);
    fscanf(in,"%d ",&gra->rozmiar);
    gra->plansza = malloc(gra->rozmiar * sizeof(char*));
    for(int i = 0; i < gra->rozmiar; i++)
    {
        gra->plansza[i] = malloc(gra->rozmiar * sizeof(char));
    }

    for(int i = 0; i < gra->rozmiar; i++)
    {
        for(int j = 0; j < gra->rozmiar; j++)
        {
            fscanf(in,"%c ",&gra->plansza[i][j]);
            //fread(gra->plansza[i]+j,sizeof(char),1,in);
        }
    }
    //fread(&gra->gracz,sizeof(int),1,in);
    fscanf(in,"%d",&gra->gracz);

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
    
    if(zbicie == 0)   //nie bylo zbicia
    {
    do{                                 //wybor piona
        warunek = 1;
        
        //wyswietlanie gracza
        if(gra->gracz%2==0)
            printf("\nGracz 'O'\n");
        else
            printf("\nGracz 'X'\n");
        
        //wczytywanie piona
        printf("Wybierz pionek\n");
        printf("Wybierz wiersz: ");
        scanf("%d", &wiersz1);
        ClrBfr();
        printf("Wybierz kolumne: ");
        scanf("%d",&kolumna1);
        ClrBfr();
        
        //czy w planszy
        if(wiersz1 < 0 || wiersz1 >= gra->rozmiar || kolumna1 < 0 || kolumna1 >= gra->rozmiar)
        {
            warunek = 0;
            continue;
        }
        
        //czy pionek
        if(gra->plansza[wiersz1][kolumna1] == 45)
        {
            warunek = 0;
            continue;
        }
        
        //czy dobry pionek
        if(gra->gracz%2==0)
        {
            if(gra->plansza[wiersz1][kolumna1] == 'X' || gra->plansza[wiersz1][kolumna1] == '$')
            {
                warunek = 0;
                continue;
            }
        }
        else
        {
            if(gra->plansza[wiersz1][kolumna1] == 'O' || gra->plansza[wiersz1][kolumna1] == '@')
            {
                warunek = 0;
                continue;
            }
        }

        //czy istnieje mozliwosc ruchu
        if(MozliwoscRuchu(gra,wiersz1,kolumna1) == 0)
        {
            warunek = 0;
            continue;
        }
        
    }while(warunek == 0);
    }
    else   //nastapilo zbicie
    {
        kolumna1 = pozX;
        wiersz1 = pozY;
    }
    

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

    if(zbicie == 1 && CzyIstniejeBicie(gra,wiersz2,kolumna2) == 1)
    {
        return 0;
    }
    else
    {
        zbicie = 0;    
        gra->gracz+= 1;
        return 0;
    }
    
}

//----------------S P R A W D Z A N I E----------------------------//

int sprawdz(struct warcaby* gra, int wiersz1, int kolumna1, int wiersz2, int kolumna2)
{
    //wyjscie poza plansze
    if(wiersz2 < 0 || wiersz2 > gra->rozmiar - 1 || kolumna2 < 0 || kolumna2 > gra->rozmiar - 1)
    {
        return 0;
    }

    //sprawdzanie pola
    if(gra->plansza[wiersz2][kolumna2] != 45)
        return 0;
    else if((gra->plansza[wiersz1][kolumna1] == '@') || (gra->plansza[wiersz1][kolumna1] == '$'))
    {
        return sprawdzDamke(gra,wiersz1,kolumna1,wiersz2,kolumna2);
    }
    else 
        return sprawdzPionka(gra,wiersz1,kolumna1,wiersz2,kolumna2);

}

//---------------------M O Z L I W O S C - R U C H U-------------//

int MozliwoscRuchu(struct warcaby* gra, int wiersz1, int kolumna1)
{
    //sprawdzanie dla damusi
    if(gra->plansza[wiersz1][kolumna1] == '@' || gra->plansza[wiersz1][kolumna1] == '$')
    {
        printf("\nMozliwosc ruchu damki\n");
        if(MozliwoscRuchuDamki(gra,wiersz1,kolumna1) == 1)
            return 1;
        return 0;
    }

    //sprawdza czy sa puste miejsca
    if(PustePole(gra,wiersz1,kolumna1) == 1)
    {
        return 1;
    }
    else
    {
        //sprawdza czy istnieje zbicie
        if(CzyIstniejeBicie(gra,wiersz1,kolumna1) == 1)
        {
            return 1;
        }
    }  

    return 0;
}

//---------------------S P R A W D Z A N I E - P U S T Y C H - P O L------------//

int PustePole(struct warcaby* gra, int wiersz1, int kolumna1)
{
    //dla gracza '0'
    if(gra->gracz%2==0)
    {
        for(int i = 0; i < 3; i+=2)
        {
            if((kolumna1 - 1 + i) >= gra->rozmiar || (kolumna1 - 1 + i) < 0 || wiersz1 == 0)
                continue;
            if(gra->plansza[wiersz1 - 1][kolumna1 - 1 + i] == '-')
                return 1;
            
        }
    }
    //dla gracza 'X
    else
    {
        for(int i = 0; i < 3; i+=2)
        {
            if((kolumna1 - 1 + i) >= gra->rozmiar || (kolumna1 - 1 + i) < 0 || wiersz1 == (gra->rozmiar -1))
                continue;
            if(gra->plansza[wiersz1 + 1][kolumna1 - 1 + i] == '-')
                return 1;
            
        }
    }
    
    return 0;
}

//---------------------C Z Y - I S T N I E J E - B I C I E-------------//

int CzyIstniejeBicie(struct warcaby* gra,int wiersz1, int kolumna1)
{
    if(wiersz1 < 2 || wiersz1 > (gra->rozmiar - 2) || kolumna1 < 2 || kolumna1 > (gra->rozmiar - 2))
    {
        if(CzymoznaKraw(gra,wiersz1,kolumna1) == 1)
        {
            return 1;
        }
    }
    else
    {
        if(Czymozna(gra,wiersz1,kolumna1) == 1)
        {
            return 1;
        }
    }

    return 0;
}

//---------------------C Z Y - M O Z N A - Z B I C - W - P O L U-------------//

int Czymozna(struct warcaby* gra,int wiersz1, int kolumna1)
{
    if(gra->gracz%2==0)
    {
        if(wiersz1 - 2 <= 0)
            return 0;
        if((((gra->plansza[wiersz1 - 1][kolumna1 - 1] == 'X') || (gra->plansza[wiersz1 - 1][kolumna1 - 1] == '$')) && (gra->plansza[wiersz1 - 2][kolumna1 - 2] == '-')) || (((gra->plansza[wiersz1 - 1][kolumna1 + 1] == 'X') || (gra->plansza[wiersz1 - 1][kolumna1 + 1] == 'X')) && (gra->plansza[wiersz1 - 2][kolumna1 + 2] == '-')))
            return 1;
    }
    else
    {
        if(wiersz1 + 2 >= gra->rozmiar)
            return 0;
        if((((gra->plansza[wiersz1 + 1][kolumna1 - 1] == 'O') || (gra->plansza[wiersz1 + 1][kolumna1 - 1] == '@')) && (gra->plansza[wiersz1 + 2][kolumna1 - 2] == '-')) || (((gra->plansza[wiersz1 + 1][kolumna1 + 1] == 'O') || (gra->plansza[wiersz1 + 1][kolumna1 + 1] == '@')) && (gra->plansza[wiersz1 + 2][kolumna1 + 2] == '-')))
           return 1; 
    }
    return 0;
}

//---------------------C Z Y - M O Z N A - Z B I C - Z - K R A W E D Z I-------------//

int CzymoznaKraw(struct warcaby* gra,int wiersz1, int kolumna1)
{
    //dla gracza 'O'
    if(gra->gracz%2==0 && wiersz1 > 0)
        for(int i = 0; i < 3; i+=2)
        {
            if((kolumna1 - 1 + i) < 0 || (kolumna1 -1 + i) >= gra->rozmiar)
                continue;
            if(((gra->plansza[wiersz1 - 1][kolumna1 - 1 + i] == 'X') || (gra->plansza[wiersz1 - 1][kolumna1 - 1 + i] == '$')) && (gra->plansza[wiersz1 - 2][kolumna1 -2 + (2*i)] == '-'))
                return 1;
        }

    //dla gracza 'X'
    if(gra->gracz%2==1 && wiersz1 < gra->rozmiar - 2)
        for(int i = 0; i < 3; i+=2)
        {
            if((kolumna1 - 1 + i) < 0 || (kolumna1 -1 + i) >= gra->rozmiar)
                continue;
            if(((gra->plansza[wiersz1 + 1][kolumna1 - 1 + i] == 'O') || (gra->plansza[wiersz1 + 1][kolumna1 - 1 + i] == '@')) && (gra->plansza[wiersz1 + 2][kolumna1 -2 + (2*i)] == '-'))
                return 1;
        }

    return 0;
}

//--------------------Z A P I S Y W A N I E------------------------------------//

void zapisz(struct warcaby* gra)
{
    FILE* out = fopen("zapis.txt","w+");
    fprintf(out,"%d\n",gra->rozmiar);
    for(int i = 0; i < gra->rozmiar; i++)
    {
        for(int j = 0; j < gra->rozmiar; j++)
        {
            fprintf(out,"%c ",gra->plansza[i][j]);
        }
        fprintf(out,"\n");
    }
    fprintf(out,"%d",gra->gracz);

    fclose(out);    
}

//----------------------M O Z L I W O S C - R U C H U - D A M K I-------------//

int MozliwoscRuchuDamki(struct warcaby* gra, int w, int k)
{
    int kierunek;
    int i;

    //prawo gora
    i = 1;
    while( w - i > -1 && k + i < gra->rozmiar)
    {
        kierunek = 1;
        if(gra->plansza[w-i][k+i] == '-')
        {
            return 1;
        }
        else 
        {
            if(MozliwoscBiciaDamka(gra,w-i,k+i,kierunek) == 1)
                return 1;
        }
        break;
        i++;
    }    

    //lewo gora
    i = 1;
    while( w - i > -1 && k - i > -1)
    {
        kierunek = 2;
        if(gra->plansza[w-i][k-i] == '-')
            return 1;
        else
        {
            if(MozliwoscBiciaDamka(gra,w-i,k-i,kierunek) == 1)
                return 1;   
        }
        break;
        i++;
    }    
    
    //lewo dol
    i = 1;
    while( w + i < gra->rozmiar && k - i >= 0)
    {
        kierunek = 3;
        if(gra->plansza[w+i][k-i] == '-')
            return 1;
        else
        {
            if(MozliwoscBiciaDamka(gra,w+i,k-i,kierunek) == 1)
                return 1;
        }
        break;
        i++;
    }

    //prawo dol
    i = 1;
    while( w + i < gra->rozmiar && k + i < gra->rozmiar)
    {
        kierunek = 4;
        if(gra->plansza[w+i][k+i] == '-')
            return 1;
        else
        {
            if(MozliwoscBiciaDamka(gra,w+i,k+i,kierunek) == 1)
                return 1;
        }
        break;
        i++;
    }

    return 0;
}

//----------------------M O Z L I W O S C - B I C I A - D A M K A-------------//

int MozliwoscBiciaDamka(struct warcaby* gra, int w, int k, int kierunek)
{
    if(gra->gracz%2 == 0)
    {
        switch (kierunek)
        {
        case 1:
            if((w - 1 > -1) && (k + 1 < gra->rozmiar) && (gra->plansza[w][k] == 'X') && (gra->plansza[w-1][k+1] == '-'))
                return 1;        
            break;
    
        case 2:
            if((w -1 > -1) && (k - 1 > -1) && (gra->plansza[w][k] == 'X') && (gra->plansza[w-1][k-1] == '-'))
                return 1;
            break;
    
        case 3:
            if((w + 1 < gra->rozmiar) && (k - 1 > -1) && (gra->plansza[w][k] == 'X') && (gra->plansza[w+1][k-1] == '-'))
                return 1;
            break;

        case 4:
            if((w + 1 < gra->rozmiar) && (k + 1 < gra->rozmiar) && (gra->plansza[w][k] == 'X') && (gra->plansza[w+1][k+1] == '-'))
                return 1;
            break;
        }
    }
    else
    {
        switch (kierunek)
        {
        case 1:
            if((w - 1 > -1) && (k + 1 < gra->rozmiar) && (gra->plansza[w][k] == 'O') && (gra->plansza[w-1][k+1] == '-'))
                return 1;        
            break;
    
        case 2:
            if((w -1 > -1) && (k - 1 > -1) && (gra->plansza[w][k] == 'O') && (gra->plansza[w-1][k-1] == '-'))
                return 1;
            break;
    
        case 3:
            if((w + 1 < gra->rozmiar) && (k - 1 > -1) && (gra->plansza[w][k] == 'O') && (gra->plansza[w+1][k-1] == '-'))
                return 1;
            break;

        case 4:
            if((w + 1 < gra->rozmiar) && (k + 1 < gra->rozmiar) && (gra->plansza[w][k] == 'O') && (gra->plansza[w+1][k+1] == '-'))
                return 1;
            break;
        }  
    }
    
    return 0;
}

//-------------S P R A W D Z A N I E - P I O N K A-----------------------------//

int sprawdzPionka(struct warcaby* gra, int wiersz1, int kolumna1, int wiersz2, int kolumna2)
{
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
        {
            if(zbicie == 1)
                return 0;
            if(wiersz2 == 0)
                gra->plansza[wiersz2][kolumna2] = '@';
            else 
                gra->plansza[wiersz2][kolumna2] = gra->plansza[wiersz1][kolumna1]; 
            gra->plansza[wiersz1][kolumna1] = 45;
            zbicie = 0;
            return 1;
        }

        if(gra->plansza[wiersz1 - 1][kolumna1 + (Hor/2)]=='X' || gra->plansza[wiersz1 - 1][kolumna1 + (Hor/2)]=='$') // zbicie
            {
                gra->plansza[wiersz1 + (Ver/2)][kolumna1 + (Hor/2)]=45; 
                
                if(wiersz2 == 0)
                    gra->plansza[wiersz2][kolumna2] = '@';
                else 
                    gra->plansza[wiersz2][kolumna2] = gra->plansza[wiersz1][kolumna1]; 
                
                gra->plansza[wiersz1][kolumna1] = 45;
                zbicie = 1;
                pozX = kolumna2;
                pozY = wiersz2;
                return 1;
            }
        else 
            return 0;

        break;

        case 1:

        if(Ver <= 0)//sprawdzanie kierunku
            return 0;

        if((Ver == 1) && (Hor == 1 || Hor == -1)) //przesuwanie o jedno pole
        {
            if(zbicie == 1)
                return 0;
            zbicie = 0;
            if(wiersz2 == gra->rozmiar - 1)
                gra->plansza[wiersz2][kolumna2] = '$';
            else 
                gra->plansza[wiersz2][kolumna2] = gra->plansza[wiersz1][kolumna1];
            gra->plansza[wiersz1][kolumna1] = 45; 
            return 1;
        }

        if(gra->plansza[wiersz1 + 1][kolumna1 + (Hor/2)]=='O' || gra->plansza[wiersz1 + 1][kolumna1 + (Hor/2)]=='@') // zbicie
            {
                gra->plansza[wiersz1 + 1][kolumna1 + (Hor/2)]=45;
                if(wiersz2 == gra->rozmiar - 1)
                    gra->plansza[wiersz2][kolumna2] = '$';
                else 
                    gra->plansza[wiersz2][kolumna2] = gra->plansza[wiersz1][kolumna1]; 
                gra->plansza[wiersz1][kolumna1] = 45; 
                zbicie = 1;
                pozX = kolumna2;
                pozY = wiersz2;
                return 1;
            }
        else 
        {
            return 0;
        }

        break;
    }

}

//-------------S P R A W D Z A N I E - D A M K E-----------------------------//

int sprawdzDamke(struct warcaby* gra, int w1, int k1, int w2, int k2)
{
    int kierunek;
    //sprawdzanie czy ruch po skosie
    if((w2 - w1) != (k2 - k1) && (w2 - w1) != (k1 - k2))
        return 0;
    
    //okreslanie kierunku
    if((w2 - w1) < 0 && (k2 - k1) > 0)
        kierunek = 1;
    else if((w2 - w1) < 0 && (k2 - k1) < 0)
        kierunek = 2;
    else if((w2 - w1) > 0 && (k2 - k1) < 0)
        kierunek = 3;
    else
        kierunek = 4;

    int i = 1;
    //sprawdzanie pozycji dla gracza 0;
    if(gra->gracz%2==0)
    {
        switch (kierunek)
        {
        case 1:
            while(w1 - i > w2 + 1)
            {
                if(gra->plansza[w1-i][k1+i] != '-')
                    return 0; 
                i++;
            }
            if((gra->plansza[w2+1][k2-1] == 'X') || (gra->plansza[w2+1][k2-1] == '$'))//zbicie
            {
                //zbicie = 1;
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w2+1][k2-1] = '-';
                gra->plansza[w1][k1] = '-';
                return 1;
            } 
            else if(gra->plansza[w2+1][k2-1] == '-')//przesuniecie
            {
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w1][k1] = '-';
                return 1;
            }
            else
                return 0;
            
            break;
        
        case 2:
            while(w1 - i > w2 + 1)
            {
                if(gra->plansza[w1-i][k1-i] != '-')
                    return 0; 
                i++;
            }
            if((gra->plansza[w2+1][k2+1] == 'X') || (gra->plansza[w2+1][k2+1] == '$'))//zbicie
            {
                //zbicie = 1;
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w2+1][k2+1] = '-';
                gra->plansza[w1][k1] = '-';
                return 1;
            } 
            else if(gra->plansza[w2+1][k2+1] == '-')//przesuniecie
            {
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w1][k1] = '-';
                return 1;
            }
            else
                return 0;
            break;
        
        case 3:
            while(w1 + i < w2 - 1)
            {
                if(gra->plansza[w1+i][k1-i] != '-')
                    return 0;
                i++;
            }
            if((gra->plansza[w2-1][k2+1] == 'X') || (gra->plansza[w2-1][k2+1] == '$'))//zbicie
            {
                //zbicie = 1;
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w2-1][k2+1] = '-';
                gra->plansza[w1][k1] = '-';
                return 1;
            } 
            else if(gra->plansza[w2-1][k2+1] == '-')//przesuniecie
            {
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w1][k1] = '-';
                return 1;
            }
            else
                return 0;
            break;
        
        case 4:
            while(w1 + i < w2-1)
            {
                if(gra->plansza[w1+i][k1+i] != '-')
                    return 0; 
                i++;
            }
            if((gra->plansza[w2-1][k2-1] == 'X') || (gra->plansza[w2-1][k2-1] == '$'))//zbicie
            {
                //zbicie = 1;
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w2-1][k2-1] = '-';
                gra->plansza[w1][k1] = '-';
                return 1;
            } 
            else if(gra->plansza[w2-1][k2-1] == '-')//przesuniecie
            {
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w1][k1] = '-';
                return 1;
            }
            else
                return 0;
            break;
        }
    }
    else
    {
        switch (kierunek)
        {
        case 1:
            while(w1 - i > w2 + 1)
            {
                if(gra->plansza[w1-i][k1+i] != '-')
                    return 0; 
                i++;
            }
            if((gra->plansza[w2+1][k2-1] == 'O') || (gra->plansza[w2+1][k2-1] == '@'))//zbicie
            {
                //zbicie = 1;
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w2+1][k2-1] = '-';
                gra->plansza[w1][k1] = '-';
                return 1;
            } 
            else if(gra->plansza[w2+1][k2-1] == '-')//przesuniecie
            {
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w1][k1] = '-';
                return 1;
            }
            else
                return 0;
            
            break;
        
        case 2:
            while(w1 - i > w2 + 1)
            {
                if(gra->plansza[w1-i][k1-i] != '-')
                    return 0; 
                i++;
            }
            if((gra->plansza[w2+1][k2+1] == 'O') || (gra->plansza[w2+1][k2+1] == '@'))//zbicie
            {
                //zbicie = 1;
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w2+1][k2+1] = '-';
                gra->plansza[w1][k1] = '-';
                return 1;
            } 
            else if(gra->plansza[w2+1][k2+1] == '-')//przesuniecie
            {
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w1][k1] = '-';
                return 1;
            }
            else
                return 0;
            break;
        
        case 3:
            while(w1 + i < w2 - 1)
            {
                if(gra->plansza[w1+i][k1-i] != '-')
                    return 0;
                i++;
            }
            if((gra->plansza[w2-1][k2+1] == 'O') || (gra->plansza[w2-1][k2+1] == '@'))//zbicie
            {
                //zbicie = 1;
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w2-1][k2+1] = '-';
                gra->plansza[w1][k1] = '-';
                return 1;
            } 
            else if(gra->plansza[w2-1][k2+1] == '-')//przesuniecie
            {
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w1][k1] = '-';
                return 1;
            }
            else
                return 0;
            break;
        
        case 4:
            while(w1 + i < w2-1)
            {
                if(gra->plansza[w1+i][k1+i] != '-')
                    return 0; 
                i++;
            }
            if((gra->plansza[w2-1][k2-1] == 'O') || (gra->plansza[w2-1][k2-1] == '@'))//zbicie
            {
                //zbicie = 1;
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w2-1][k2-1] = '-';
                gra->plansza[w1][k1] = '-';
                return 1;
            } 
            else if(gra->plansza[w2-1][k2-1] == '-')//przesuniecie
            {
                gra->plansza[w2][k2] = gra->plansza[w1][k1];
                gra->plansza[w1][k1] = '-';
                return 1;
            }
            else
                return 0;
            break;
        }
    }    
    
    return 0;   
} 
