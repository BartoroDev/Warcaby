#include <stdio.h>
#include <stdlib.h>

struct warcaby{
    int rozmiar;
    char** plansza; 
    int gracz;
};

//LISTA RUCHOW

struct ruch
{
    int z[2],na[2];
    int wartosc;
    int czyBicie;
};

struct listaRuchow{
    struct ruch ri;
    struct listaRuchow* nastepny;
};

//LISTA PIONKOW

struct Pionek{
    int wiersz;
    int kolumna;
    int wartosc;
};

struct PionkiX{
    struct Pionek pionek;
    struct PionkiX* nastepny;
    struct listaRuchow* lista;
};

struct PionkiY{
    struct Pionek pionek;
    struct PionkiY* nastepny;
    struct listaRuchow* lista;
};

//FUNKCJE

void wyswietl(struct warcaby* gra);

int MozliwoscRuchu(struct warcaby* gra,int wiersz1,int kolumna1);

int Czymozna(struct warcaby* gra,int wiersz1,int kolumna1);

int MozliwoscRuchuDamki(struct warcaby* gra, int wiersz1,int kolumna1,int tryb);

int MozliwoscBiciaDamka(struct warcaby* gra, int w, int k, int kierunek);

void pokazListeX(struct PionkiX* lx);

void pokazListeY(struct PionkiY* ly);

void listaRuchowX(struct PionkiX* lX,struct warcaby* gra);

void listaRuchowY(struct PionkiY* lY,struct warcaby* gra);

void usunPionekX(struct PionkiX** lista,struct Pionek pionus);

void wykonajRuch(struct warcaby* gra,struct ruch move);

void usunListeRuchowX(struct PionkiX* lista);

void usunListeRuchowY(struct PionkiY* lista);

void przypiszruch(struct ruch* to,struct ruch* from);

void reset(struct warcaby* gra, struct warcaby* tmp);

void wczytajPionki(struct warcaby* gra,struct PionkiX** lX,struct PionkiY** lY);

void cofnijRuch(struct warcaby* gra, struct ruch move);

struct ruch znajdzNajlepszy(struct PionkiX* lX);

struct PionkiX* dodajX(struct PionkiX* l,struct Pionek pionekN);

struct PionkiY* dodajY(struct PionkiY* l,struct Pionek pionekN);

struct listaRuchow* RPustePola(struct warcaby* gra,int wiersz1,int kolumna1,int zawodnik);

struct listaRuchow* RCzymoznaKrawP(struct warcaby* gra,int wiersz1,int kolumna1,int zawodnik);

struct listaRuchow* RCzyIstniejeBicieP(struct warcaby* gra,int wiersz1,int kolumna1,int zawodnik);

struct warcaby* wczytaj(char sciezka[],struct PionkiX** listaX,struct PionkiY** listaY);

void alfabeta(struct warcaby* gra,struct PionkiX* listaX, struct PionkiY* listaY, int glebokosc,int* ileRuchow,int* suma,struct ruch* best,struct ruch* rozpatrywany,int poziom);

int oceniajaca(struct warcaby* gra);

int zbicie = 0;

//---------------------------------G Ł Ó W N A - F U N K C J A----------------------------//

int main()
{
    struct PionkiX* listaX = NULL;
    struct PionkiY* listaY = NULL;    
    struct warcaby* gra = wczytaj("zapis.txt",&listaX,&listaY);

    wyswietl(gra);

    //listaRuchowX(listaX,gra);
    
    //listaRuchowY(listaY,gra);

    struct ruch* naj = malloc(sizeof(struct ruch)); 
    struct ruch* rozpatrywany;
    naj->wartosc = -10000000;
    int glebokosc = 6;//nie ustawiac na wiecej niz 6!
    int poziom = 1;
    int suma = 0;
    int bigos = 0;
    alfabeta(gra,listaX,listaY,glebokosc,&bigos,&suma,naj,rozpatrywany,poziom);

    wykonajRuch(gra,(*naj));
    wyswietl(gra);    


    return 0;
}

//-------------------------------D O D A W A N I E - P I O N K Ó W---------------------------------//

struct PionkiX* dodajX(struct PionkiX* l,struct Pionek pionekN)
{
    struct PionkiX* nowy = malloc(sizeof(struct PionkiX));
    nowy->pionek = pionekN;
    nowy->nastepny = NULL;
    if(l==NULL)
    {
        return nowy;
    }
    struct PionkiX* glowa = l;
    while(l->nastepny)
    {
        l=l->nastepny;
    }
    l->nastepny = nowy;
    return glowa;
}

struct PionkiY* dodajY(struct PionkiY* l,struct Pionek pionekN)
{
    struct PionkiY* nowy = malloc(sizeof(struct PionkiY));
    nowy->pionek = pionekN;
    nowy->nastepny = NULL;
    if(l==NULL)
    {
        return nowy;
    }
    struct PionkiY* glowa = l;
    while(l->nastepny)
    {
        l=l->nastepny;
    }
    l->nastepny = nowy;
    return glowa;
}

//-----------------------------W Y S W I E T L-------------------------------------------//

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

//-------------------------------W C Z Y T A J-------------------------------------------//

struct warcaby* wczytaj(char sciezka[],struct PionkiX** listaX,struct PionkiY** listaY)
{
    FILE* in = fopen(sciezka,"r");
    struct warcaby* gra = malloc(sizeof(struct warcaby));
    fscanf(in,"%d ",&gra->rozmiar);
    gra->plansza = malloc(gra->rozmiar * sizeof(char*));
    for(int i = 0; i < gra->rozmiar; i++)
    {
        gra->plansza[i] = malloc(gra->rozmiar * sizeof(char));
    }
    struct Pionek pion;
    for(int i = 0; i < gra->rozmiar; i++)
    {
        for(int j = 0; j < gra->rozmiar; j++)
        {
            fscanf(in,"%c ",&gra->plansza[i][j]);
            if(gra->plansza[i][j] == 'X')
            {
                pion.kolumna = j;
                pion.wiersz = i;
                pion.wartosc = 5;
                (*listaX) = dodajX((*listaX),pion);
            }
            else if(gra->plansza[i][j] == '$')
            {
                pion.kolumna = j;
                pion.wiersz = i;
                pion.wartosc = 25;
                (*listaX) = dodajX((*listaX),pion);
            }
            else if(gra->plansza[i][j] == 'O')
            {
                pion.kolumna = j;
                pion.wiersz = i;
                pion.wartosc = 5;
                (*listaY) = dodajY((*listaY),pion);
            }
            else if(gra->plansza[i][j] == '@')
            {
                pion.kolumna = j;
                pion.wiersz = i;
                pion.wartosc = 25;
                (*listaY) = dodajY((*listaY),pion);
            }
        }
    }
    fscanf(in,"%d",&gra->gracz);
    fclose(in);
    return gra;
}

//-------------------------------------U S U W A N I E - L I S T Y - R U C H O W------------------------//

void usunListeRuchowX(struct PionkiX* lX)
{
    while(lX)
    {
        while(lX->lista)
        {
            struct listaRuchow* tmp = lX->lista;
            lX->lista = lX->lista->nastepny;
            free(tmp);
        }
        lX = lX->nastepny;

    }
}

void usunListeRuchowY(struct PionkiY* lY)
{
    while(lY)
    {
        while(lY->lista)
        {
            struct listaRuchow* tmp = lY->lista;
            lY->lista = lY->lista->nastepny;
            free(tmp); 
        }
        lY = lY->nastepny;
    }
}

//-------------------------------U S U W A N I E - P I O N K A-----------------------------------------//

void usunPionekX(struct PionkiX** lista,struct Pionek pionus)
{
    if((*lista)->pionek.wiersz == pionus.wiersz && (*lista)->pionek.kolumna == pionus.kolumna)
    {
        struct PionkiX* tmp = (*lista);
        (*lista) = (*lista)->nastepny;
        free(tmp);
    }
    else
    {
        struct PionkiX* tmp = (*lista);
        while((*lista)->pionek.wiersz != pionus.wiersz || (*lista)->pionek.kolumna != pionus.kolumna)
        {
            tmp = (*lista);
            (*lista) = (*lista)->nastepny;
        }

        if(tmp->nastepny->nastepny->nastepny)
        {
            tmp->nastepny = tmp->nastepny->nastepny->nastepny;
            free((*lista));
        }
        else
        {
            tmp->nastepny = NULL;
            free((*lista));
        }
        
    }
    
}

//--------------------------P O K A Z Y W A N I E - L I S T Y-----------------------------------------------//

void pokazListeX(struct PionkiX* lx)
{
    while(lx)
    {
        printf("Z: %d, %d; Wartosc: %d\n",lx->pionek.wiersz,lx->pionek.kolumna,lx->pionek.wartosc);
        lx = lx->nastepny;        
    }
}

void pokazListeY(struct PionkiY* ly)
{
  
    while(ly)
    {
        printf("Z: %d, %d; Wartosc: %d\n",ly->pionek.wiersz,ly->pionek.kolumna,ly->pionek.wartosc);
        ly = ly->nastepny;
    }
}

//--------------------------------G E N E R A T O R - R U C H Ó W------------------------------------------//

void listaRuchowX(struct PionkiX* lX,struct warcaby* gra)
{
    struct listaRuchow* lR = NULL;
    struct listaRuchow* glowa = NULL;
    int w = lX->pionek.wiersz;
    int k = lX->pionek.kolumna;
    int wartosc = lX->pionek.wartosc;
    if(wartosc == 5)
    {
        lR = RPustePola(gra,w,k,1);
        if(lR)
        {
            glowa = lR;
            while(lR->nastepny)
            {
                lR=lR->nastepny;
            }

            if(k < 2 || k > gra->rozmiar-3)
            {
                lR->nastepny = RCzymoznaKrawP(gra,w,k,1);
            }
            else
                lR->nastepny = RCzyIstniejeBicieP(gra,w,k,1);         
        }
        else
        {
            if(k < 2 || k > gra->rozmiar-3)
            {
                lR = RCzymoznaKrawP(gra,w,k,1);
            }
            else
                lR = RCzyIstniejeBicieP(gra,w,k,1);
            
            glowa = lR;
        }
    }
          
    lX->lista = glowa;
    
    /*
    printf("\n\n-------nowy pionek------------\n");
    while(lX->lista)
    {
        printf("z: %d, %d; na: %d, %d; wartosc: %d\n",lX->lista->ri.z[0],lX->lista->ri.z[1],lX->lista->ri.na[0],lX->lista->ri.na[1],lX->lista->ri.wartosc);
        lX->lista=lX->lista->nastepny;
    }*/

    lX->lista = glowa;

    if(lX->nastepny)
        listaRuchowX(lX->nastepny,gra);
}

void listaRuchowY(struct PionkiY* lY,struct warcaby* gra)
{
    if(lY==NULL)
        return;
    
    struct listaRuchow* lR = NULL;
    struct listaRuchow* glowa = NULL;
    int w = lY->pionek.wiersz;
    int k = lY->pionek.kolumna;
    int wartosc = lY->pionek.wartosc;
    if(wartosc == 5)
    {
        lR = RPustePola(gra,w,k,0);
        if(lR)
        {
            glowa = lR;
            while(lR->nastepny)
            {
                lR=lR->nastepny;
            }

            if(k < 2 || k > gra->rozmiar-3)
                lR->nastepny = RCzymoznaKrawP(gra,w,k,0);
            else
                lR->nastepny = RCzyIstniejeBicieP(gra,w,k,0);         
        }
        else
        {
            if(k < 2 || k > gra->rozmiar-3)
                lR = RCzymoznaKrawP(gra,w,k,0);
            else
                lR = RCzyIstniejeBicieP(gra,w,k,0);
            
            glowa = lR;
        }
    }
    
    lY->lista = glowa;
    
    /*
    printf("\n\n-------nowy pionek------------\n");
    while(lY->lista)
    {
        printf("z: %d, %d; na: %d, %d; wartosc: %d\n",lY->lista->ri.z[0],lY->lista->ri.z[1],lY->lista->ri.na[0],lY->lista->ri.na[1],lY->lista->ri.wartosc);
        lY->lista=lY->lista->nastepny;
    }
    */
    lY->lista = glowa;

    if(lY->nastepny)
        listaRuchowY(lY->nastepny,gra);
}

//------------------------------N A J L E P S Z Y - R U C H-------------------------------------------//

struct ruch znajdzNajlepszy(struct PionkiX* lX)
{
    int temp=0;
    struct ruch bestChoice;
    struct listaRuchow* lR;
    while(lX)
    {
        lR=lX->lista;
        while(lR)
        {
            if(lR->ri.wartosc>temp)
            {
                temp=lR->ri.wartosc;
                bestChoice.z[0]=lR->ri.z[0];
                bestChoice.z[1]=lR->ri.z[1];
                bestChoice.na[0]=lR->ri.na[0];
                bestChoice.na[1]=lR->ri.na[1];
                bestChoice.wartosc = temp;
                bestChoice.czyBicie = lR->ri.czyBicie;
            }
            lR=lR->nastepny;
        }
        lX=lX->nastepny;
    }

    return bestChoice;
}

//--------------------------------W Y K O N A J - R U C H - P I O N K A--------------------------//

void wykonajRuch(struct warcaby* gra,struct ruch move)
{
    if(move.na[0] == gra->rozmiar - 1)
    {
        gra->plansza[move.z[0]][move.z[1]] = '$';
    }

    if(move.czyBicie == 0)
    {
        gra->plansza[move.na[0]][move.na[1]] = gra->plansza[move.z[0]][move.z[1]]; 
        gra->plansza[move.z[0]][move.z[1]] = '-';
    }
    else
    {
        gra->plansza[move.na[0]][move.na[1]] = gra->plansza[move.z[0]][move.z[1]]; 
        gra->plansza[move.z[0]][move.z[1]] = '-';
        gra->plansza[move.z[0] + ((move.na[0] - move.z[0])/2)][move.z[1] + ((move.na[1] - move.z[1])/2)] = '-';
    }
    
    
}

//----------------------------------M O Z L I W O S C - R U C H U--------------------------//

int MozliwoscRuchu(struct warcaby* gra, int wiersz1, int kolumna1)
{
    //sprawdzanie dla damusi
    if(gra->plansza[wiersz1][kolumna1] == '@' || gra->plansza[wiersz1][kolumna1] == '$')
    {
        if(MozliwoscRuchuDamki(gra,wiersz1,kolumna1,0) == 1)
            return 1;
        return 0;
    }

    //sprawdza czy sa puste miejsca
    /*if(PustePole(gra,wiersz1,kolumna1) == 1)
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
    }*/  

    return 0;
}

//----------------------------------S P R A W D Z A N I E - P U S T Y C H - P O L-------------------------//

struct listaRuchow* RPustePola(struct warcaby* gra, int wiersz1, int kolumna1,int zawodnik)
{
    struct listaRuchow* lista = NULL;
    struct listaRuchow* glowa = NULL;
    //dla gracza '0'
    if(zawodnik==0)
    {
        for(int i = 0; i < 2; i++)
        {
            if((kolumna1 - 1 + (2*i)) >= gra->rozmiar || (kolumna1 - 1 + (2*i)) < 0 || wiersz1 == 0)
                continue;
            if(gra->plansza[wiersz1 - 1][kolumna1 - 1 + (2*i)] == '-')
            {
                if(lista==NULL)
                {
                    lista = malloc(sizeof(struct listaRuchow));
                    glowa = lista;
                }
                else
                {
                    while(lista->nastepny)
                    {
                        lista = lista->nastepny;
                    }
                    lista->nastepny = malloc(sizeof(struct listaRuchow));
                    lista = lista->nastepny;
                }
                lista->ri.z[0]=wiersz1;
                lista->ri.z[1]=kolumna1;
                lista->ri.na[0]=wiersz1-1;
                lista->ri.na[1]=kolumna1-1+(2*i);
                if(wiersz1-1==0)
                    lista->ri.wartosc = 5 + 2*(gra->rozmiar - (wiersz1 - 1));
                else
                    lista->ri.wartosc = (5 + gra->rozmiar - (wiersz1 - 1));                              
                lista->nastepny=NULL;
                lista->ri.czyBicie = 0;
            }
            
        }
    }
    else //dla gracza 'X'
    {
        for(int i = 0; i < 2; i++)
        {
            if((kolumna1 - 1 + (2*i)) >= gra->rozmiar || (kolumna1 - 1 + (2*i)) < 0 || wiersz1 == (gra->rozmiar -1))
                continue;
            if(gra->plansza[wiersz1 + 1][kolumna1 - 1 + (2*i)] == '-')
            {
                if(lista==NULL)
                {
                    lista = malloc(sizeof(struct listaRuchow));
                    glowa = lista;
                }
                else
                {
                    while(lista->nastepny)
                    {
                        lista = lista->nastepny;
                    }
                    lista->nastepny = malloc(sizeof(struct listaRuchow));
                    lista = lista->nastepny;
                }
                lista->ri.z[0]=wiersz1;
                lista->ri.z[1]=kolumna1;
                lista->ri.na[0]=wiersz1+1;
                lista->ri.na[1]=kolumna1-1+(2*i); 
                if(wiersz1 + 1 == gra->rozmiar - 1) 
                    lista->ri.wartosc = 5 + 2*(wiersz1 + 1);
                else
                    lista->ri.wartosc = 5 + wiersz1 + 1;                            
                lista->nastepny=NULL;
                lista->ri.czyBicie = 0;
            }
            
        }
    }

    return glowa;
}

//----------------------------------C Z Y - I S T N I E J E - B I C I E--------------------------//
/*
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
}*/

//------------------------------------C Z Y - M O Z N A - Z B I C - W - P O L U--------------------------//

struct listaRuchow* RCzyIstniejeBicieP(struct warcaby* gra,int wiersz1, int kolumna1,int zawodnik)
{
    struct listaRuchow* lista = NULL;
    struct listaRuchow* glowa = NULL;
    if(zawodnik==0)
    {
        if(wiersz1 < 2)        
            return glowa;

        for(int i = 0; i < 2; i++)
        {
            if((gra->plansza[wiersz1 - 1][kolumna1 -1 + 2*i] == 'X' || gra->plansza[wiersz1 - 1][kolumna1 -1 + 2*i] == '$') &&(gra->plansza[wiersz1 - 2][kolumna1 - 2 + 4*i] == '-'))
            {
                if(lista==NULL)
                {
                    lista = malloc(sizeof(struct listaRuchow));
                    glowa = lista;
                }
                else
                {
                    while(lista->nastepny)
                    {
                        lista = lista->nastepny;
                    }
                    lista->nastepny = malloc(sizeof(struct listaRuchow));
                    lista = lista->nastepny;
                } 
                lista->ri.z[0] = wiersz1;
                lista->ri.z[1] = kolumna1;
                lista->ri.na[0] = wiersz1 - 2;
                lista->ri.na[1] = kolumna1 - 2 + 4*i;
                if(gra->plansza[wiersz1-1][kolumna1 -1 + 2*i]== '$')
                    lista->ri.wartosc = 4*(gra->rozmiar - (wiersz1 - 2)  +5);
                else
                    lista->ri.wartosc = 3*(gra->rozmiar - (wiersz1 - 2) + 5);
                if(wiersz1 - 2 == 0)
                    lista->ri.wartosc = lista->ri.wartosc * 2;
                lista->nastepny=NULL;
                lista->ri.czyBicie = 5;
            }
        }
    }
    else
    {
        if(wiersz1 > gra->rozmiar-3)
            return glowa;

        for(int i = 0; i < 2; i++)
        {
            if((gra->plansza[wiersz1 + 1][kolumna1 -1 + 2*i] == 'O' || gra->plansza[wiersz1 + 1][kolumna1 -1 + 2*i] == '@') &&(gra->plansza[wiersz1 + 2][kolumna1 - 2 + 4*i] == '-'))//TUTAJ SKONCZYLEM - jakos uniwersalnie zapisac te petle
            {
                if(lista==NULL)
                {
                    lista = malloc(sizeof(struct listaRuchow));
                    glowa = lista;
                }
                else
                {
                    while(lista->nastepny)
                    {
                        lista = lista->nastepny;
                    }
                    lista->nastepny = malloc(sizeof(struct listaRuchow));
                    lista = lista->nastepny;
                } 
                lista->ri.z[0] = wiersz1;
                lista->ri.z[1] = kolumna1;
                lista->ri.na[0] = wiersz1 + 2;
                lista->ri.na[1] = kolumna1 - 2 + 4*i;
                if(gra->plansza[wiersz1+1][kolumna1 -1 + 2*i]== '@')
                    lista->ri.wartosc = 4*(wiersz1 + 2 +5);
                else
                    lista->ri.wartosc = 3*(wiersz1 + 2 + 5);
                if(wiersz1 + 2 == gra->rozmiar-1)
                    lista->ri.wartosc = lista->ri.wartosc * 2;
                lista->nastepny=NULL;
                lista->ri.czyBicie = 5;
            }
        }
    }

    return glowa;
}

//--------------------------C Z Y - M O Z N A - Z B I C - Z - K R A W E D Z I-------------------------//

struct listaRuchow* RCzymoznaKrawP(struct warcaby* gra,int wiersz1, int kolumna1,int zawodnik)
{
    struct listaRuchow* lista = NULL;
 
    if(zawodnik==0 && wiersz1 > 1)
    {
        for(int i = 0; i < 2; i++)
        {
            if((kolumna1 - 1 + 2*i) < 0 || (kolumna1 - 1 + 2*i) >= gra->rozmiar || (kolumna1 - 2 + 4*i) < 0 || (kolumna1 - 1 + 4*i)>=gra->rozmiar)
                continue;
            if(((gra->plansza[wiersz1 - 1][kolumna1 - 1 + 2*i] == 'X') || (gra->plansza[wiersz1 - 1][kolumna1 - 1 + 2*i] == '$')) && (gra->plansza[wiersz1 - 2][kolumna1 -2 + (4*i)] == '-'))
            {
                lista = malloc(sizeof(struct listaRuchow));
                lista->ri.z[0] = wiersz1;
                lista->ri.z[1] = kolumna1;
                lista->ri.na[0] = wiersz1 - 2;
                lista->ri.na[1] = kolumna1 -2 + (2*i);
                if(gra->plansza[wiersz1-1][kolumna1 -1 + 2*i]== '$')
                    lista->ri.wartosc = 4*(gra->rozmiar - (wiersz1 - 2)  +5);
                else
                    lista->ri.wartosc = 3*(gra->rozmiar - (wiersz1 - 2) + 5);
                if(wiersz1 - 2 == 0)
                    lista->ri.wartosc = lista->ri.wartosc * 2;                    
                lista->nastepny=NULL;
                lista->ri.czyBicie = 5;
                return lista;
            }
        }
    }

    if(zawodnik==1 && wiersz1 < gra->rozmiar - 2)
    {
        for(int i = 0; i < 2; i++)
        {
            if((kolumna1 - 1 + 2*i) < 0 || (kolumna1 - 1 + 2*i) >= gra->rozmiar || (kolumna1 - 2 + 4*i) < 0 || (kolumna1 - 1 + 4*i)>=gra->rozmiar)
            {
                continue;
            }
            if(((gra->plansza[wiersz1 + 1][kolumna1 - 1 + 2*i] == 'O') || (gra->plansza[wiersz1 + 1][kolumna1 - 1 + 2*i] == '@')) && (gra->plansza[wiersz1 + 2][kolumna1 -2 + (4*i)] == '-'))
            {
                lista = malloc(sizeof(struct listaRuchow));
                lista->ri.z[0] = wiersz1;
                lista->ri.z[1] = kolumna1;
                lista->ri.na[0] = wiersz1 + 2;
                lista->ri.na[1] = kolumna1 -2 + (4*i);
                if(gra->plansza[wiersz1+1][kolumna1 -1 + 2*i]== '@')
                    lista->ri.wartosc = 4*(wiersz1 + 2 +5);
                else
                    lista->ri.wartosc = 3*(wiersz1 + 2 + 5);
                if(wiersz1 + 2 == gra->rozmiar-1)
                    lista->ri.wartosc = lista->ri.wartosc * 2;
                lista->nastepny=NULL;
                lista->ri.czyBicie = 5;
                return lista;
            }
        }
    }

    return NULL;
}

//----------------------------M O Z L I W O S C - R U C H U - D A M K I------------------------------//

int MozliwoscRuchuDamki(struct warcaby* gra, int w, int k,int tryb)
{
    int kierunek;
    int i;

    //prawo gora
    i = 1;
    while( w - i > -1 && k + i < gra->rozmiar)
    {
        kierunek = 1;
        if(gra->plansza[w-i][k+i] == '-' && tryb == 0)
        {
            return 1;
        }
        else 
        {
            if(MozliwoscBiciaDamka(gra,w-i,k+i,kierunek) == 1)
                return 1;
        }
        if(tryb == 0)
            break;
        i++;
    }    

    //lewo gora
    i = 1;
    while( w - i > -1 && k - i > -1)
    {
        kierunek = 2;
        if(gra->plansza[w-i][k-i] == '-' && tryb == 0)
            return 1;
        else
        {
            if(MozliwoscBiciaDamka(gra,w-i,k-i,kierunek) == 1)
                return 1;   
        }
        if(tryb == 0)
            break;
        i++;
    }    
    
    //lewo dol
    i = 1;
    while( w + i < gra->rozmiar && k - i >= 0)
    {
        kierunek = 3;
        if(gra->plansza[w+i][k-i] == '-' && tryb == 0)
            return 1;
        else
        {
            if(MozliwoscBiciaDamka(gra,w+i,k-i,kierunek) == 1)
                return 1;
        }
        if(tryb == 0)
            break;
        i++;
    }

    //prawo dol
    i = 1;
    while( w + i < gra->rozmiar && k + i < gra->rozmiar)
    {
        kierunek = 4;
        if(gra->plansza[w+i][k+i] == '-' && tryb == 0)
            return 1;
        else
        {
            if(MozliwoscBiciaDamka(gra,w+i,k+i,kierunek) == 1)
                return 1;
        }
        if(tryb == 0)
            break;
        i++;
    }

    return 0;
}

//-------------------------------M O Z L I W O S C - B I C I A - D A M K A------------------------//

int MozliwoscBiciaDamka(struct warcaby* gra, int w, int k, int kierunek)
{
    if(gra->gracz%2 == 0)
    {
        switch (kierunek)
        {
        case 1:
            if((w - 1 > -1) && (k + 1 < gra->rozmiar) && ((gra->plansza[w][k] == 'X') || (gra->plansza[w][k] == '$')) && (gra->plansza[w-1][k+1] == '-'))
                return 1;        
            break;
    
        case 2:
            if((w -1 > -1) && (k - 1 > -1) && ((gra->plansza[w][k] == 'X') || (gra->plansza[w][k] == '$')) && (gra->plansza[w-1][k-1] == '-'))
                return 1;
            break;
    
        case 3:
            if((w + 1 < gra->rozmiar) && (k - 1 > -1) && ((gra->plansza[w][k] == 'X') || (gra->plansza[w][k] == '$')) && (gra->plansza[w+1][k-1] == '-'))
                return 1;
            break;

        case 4:
            if((w + 1 < gra->rozmiar) && (k + 1 < gra->rozmiar) && ((gra->plansza[w][k] == 'X') || (gra->plansza[w][k] == '$')) && (gra->plansza[w+1][k+1] == '-'))
                return 1;
            break;
        }
    }
    else
    {
        switch (kierunek)
        {
        case 1:
            if((w - 1 > -1) && (k + 1 < gra->rozmiar) && ((gra->plansza[w][k] == 'O') || (gra->plansza[w][k] == '@')) && (gra->plansza[w-1][k+1] == '-'))
                return 1;        
            break;
    
        case 2:
            if((w -1 > -1) && (k - 1 > -1) && ((gra->plansza[w][k] == 'O') || (gra->plansza[w][k] == '@')) && (gra->plansza[w-1][k-1] == '-'))
                return 1;
            break;
    
        case 3:
            if((w + 1 < gra->rozmiar) && ((k - 1 > -1) && (gra->plansza[w][k] == 'O') || (gra->plansza[w][k] == '@')) && (gra->plansza[w+1][k-1] == '-'))
                return 1;
            break;

        case 4:
            if((w + 1 < gra->rozmiar) && (k + 1 < gra->rozmiar) && ((gra->plansza[w][k] == 'O') || (gra->plansza[w][k] == '@')) && (gra->plansza[w+1][k+1] == '-'))
                return 1;
            break;
        }  
    }
    
    return 0;
}

//----------------------------------A L F A - B E T A-------------------------------------//

void alfabeta(struct warcaby* gra,struct PionkiX* listaX, struct PionkiY* listaY, int glebokosc, int* bigos,int* suma,struct ruch* best,struct ruch* rozpatrywany,int poziom)
{
    struct listaRuchow* lRuchow;
    struct PionkiX* roboczyX;
    struct PionkiY* roboczyY;

        
    //tworzenie roboczej planszy-----------------------------
    struct warcaby* tmp = malloc(sizeof(struct warcaby));
    tmp->rozmiar = gra->rozmiar;
    tmp->gracz = gra->gracz;
    tmp->plansza = malloc(tmp->rozmiar * sizeof(char*));
    for(int i = 0; i < tmp->rozmiar;i++)
    {
        tmp->plansza[i] = malloc(tmp->rozmiar * sizeof(char));
        for(int j = 0; j < tmp->rozmiar; j++)
        {
            tmp->plansza[i][j] = gra->plansza[i][j];
        }
    }
    //--------------------------------------------------------

    

    if(poziom == 1)
    {
        listaRuchowX(listaX,tmp);
        while(listaX)
        {
            lRuchow = listaX->lista;
            while(lRuchow)
            {
                struct ruch obecny;
                przypiszruch(&obecny,&lRuchow->ri);
                wykonajRuch(tmp,obecny);
                *bigos+=1;
                wczytajPionki(tmp,&roboczyX,&roboczyY);
                alfabeta(tmp,roboczyX,roboczyY,glebokosc,bigos,suma,best,&obecny,poziom+1);
                cofnijRuch(tmp,obecny);
                lRuchow = lRuchow->nastepny;
            }
            listaX = listaX->nastepny;
        }
        
        printf("\nILOSC ZASYMULOWANYCH RUCHOW: %d\n",*bigos);
        return;
    }

    if(poziom%2==1)// ruchy 'X'
    {
        listaRuchowX(listaX,tmp);
        while(listaX)
        {
            lRuchow = listaX->lista;
            while(lRuchow)
            {
                struct ruch obecny;
                przypiszruch(&obecny,&lRuchow->ri);
                wykonajRuch(tmp,obecny);
                *bigos+=1;
                if(poziom == glebokosc)
                {
                    *suma = oceniajaca(tmp);
                    if(*suma > best->wartosc)
                    {
                        //wyswietl(tmp);
                        przypiszruch(best,rozpatrywany); 
                        best->wartosc = *suma;
                    }
                }
                else
                {
                    wczytajPionki(tmp,&roboczyX,&roboczyY);
                    alfabeta(tmp,roboczyX,roboczyY,glebokosc,bigos,suma,best,rozpatrywany,poziom + 1);
                }
                lRuchow = lRuchow->nastepny;
                cofnijRuch(tmp,obecny);
            }
            listaX = listaX->nastepny;
        }
        return;
    }
    else// ruchy 'O'
    {
        listaRuchowY(listaY,tmp);
        while(listaY)
        {
            lRuchow = listaY->lista;
            while(lRuchow)
            {
                struct ruch obecny;
                przypiszruch(&obecny,&lRuchow->ri);
                wykonajRuch(tmp,obecny);
                *bigos+=1;
                if(poziom == glebokosc)
                {
                    *suma = oceniajaca(tmp);
                    if(*suma > best->wartosc)
                    {
                        //wyswietl(tmp);
                        przypiszruch(best,rozpatrywany); 
                        best->wartosc = *suma;
                    }
                }
                else
                {
                    wczytajPionki(tmp,&roboczyX,&roboczyY);
                    alfabeta(tmp,roboczyX,roboczyY,glebokosc,bigos,suma,best,rozpatrywany,poziom + 1);
                }
                lRuchow = lRuchow->nastepny;
                cofnijRuch(tmp,obecny);
            }
            listaY = listaY->nastepny;
        }
        return;
    }
    
    return;
}

//--------------------P R Z Y P I S Y W A N I E - R U C H U-----------------------//

void przypiszruch(struct ruch* to, struct ruch* from)
{

    to->czyBicie = from->czyBicie;
    to->z[0] = from->z[0];
    to->z[1] = from->z[1];
    to->na[0] = from->na[0];
    to->na[1] = from->na[1];
    to->wartosc = from->wartosc;

}

//----------------------------R E S E TO W A N I E - P L A N S Z Y-------------------------//

void reset(struct warcaby* gra, struct warcaby* tmp)
{
    for(int i = 0; i < gra->rozmiar;i++)
    {
        for(int j = 0; j < gra->rozmiar;j++)
        {
            tmp->plansza[i][j] = gra->plansza[i][j];
        }
    }
}

//------------------------------O C E N I A J Ą C A---------------------------------------------//

int oceniajaca(struct warcaby* gra)
{
    int wynik = 0;
    for(int i = 0; i < gra->rozmiar; i++)
    {
        for(int j = 0; j < gra->rozmiar; j++)
        {
            switch (gra->plansza[i][j])
            {
            case 'X':
                wynik = wynik + 5 + i;  
                break;
            
            case '$':
                wynik = wynik + 25 + i;
                break;

            case 'O':
                wynik = wynik - 5 - gra->rozmiar + i;
                break;
            
            case '@':
                wynik = wynik -25 - gra->rozmiar + i;
                break;

            default:
                break;
            }
        }
    }
    //printf("\n\tWYNIK PO CALOSCI: %d\n",wynik);


    return wynik;
}

//------------------------------W C Z Y T A J - P I O N K I----------------------------------//

void wczytajPionki(struct warcaby* gra,struct PionkiX** lX,struct PionkiY** lY)
{
    (*lY) = NULL;
    (*lX) = NULL;     
    struct Pionek pion; 
    for(int i = 0; i < gra->rozmiar; i++)
    {
        for(int j = 0; j  < gra->rozmiar;j++)
        {
            switch (gra->plansza[i][j])
            {
            case 'O':
                pion.wiersz = i;
                pion.kolumna = j;
                pion.wartosc = 5;
                (*lY) = dodajY((*lY),pion);
                break;
            
            case '@':
                pion.wiersz = i;
                pion.kolumna = j;
                pion.wartosc = 25;
                (*lY) = dodajY((*lY),pion);
                break;
            
            case 'X':
                pion.wiersz = i;
                pion.kolumna = j;
                pion.wartosc = 5;
                (*lX) = dodajX((*lX),pion);
                break;
            
            case '$':
                pion.wiersz = i;
                pion.kolumna = j;
                pion.wartosc = 25;
                (*lX) = dodajX((*lX),pion);
                break;

                default:
                    break;
            }
        }
    }
}

//---------------------------------C O F N I J - R U C H-----------------------------------------------//

void cofnijRuch(struct warcaby* gra, struct ruch move)
{
    if(move.czyBicie == 0)
    {
        gra->plansza[move.z[0]][move.z[1]] = gra->plansza[move.na[0]][move.na[1]];
        gra->plansza[move.na[0]][move.na[1]] = '-';
    }
    else if(move.czyBicie == 5)
    {
        int hor = move.na[1] - move.z[1];
        int ver = move.na[0] - move.z[0];
        hor = hor/2;
        ver = ver/2;
        char znak = gra->plansza[move.na[0]][move.na[1]];
        if(znak == 'X' || znak == '$')
        {
            gra->plansza[move.z[0]][move.z[1]] = gra->plansza[move.na[0]][move.na[1]];
            gra->plansza[move.na[0]][move.na[1]] = '-';
            gra->plansza[move.z[0] + ver][move.z[1] + hor] = 'O';
        }
        else
        {
            gra->plansza[move.z[0]][move.z[1]] = gra->plansza[move.na[0]][move.na[1]];
            gra->plansza[move.na[0]][move.na[1]] = '-';
            gra->plansza[move.z[0] + ver][move.z[1] + hor] = 'X';
        }
        
    }
    else
    {

    }
}
