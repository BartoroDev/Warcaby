#include <stdio.h>
#include <stdlib.h>

struct warcaby{
    int rozmiar;
    char** plansza; 
    int gracz;
};
int Czymozna(struct warcaby* gra,int wiersz1,int kolumna1);

int CzymoznaKraw(struct warcaby* gra,int wiersz1,int kolumna1);

int CzyIstniejeBicie(struct warcaby* gra,int wiersz1,int kolumna1);

struct ruch
{
    int z[2],na[2];
    int wartosc;
};

struct listaRuchow{
    struct ruch ri;
    struct listaRuchow* nastepny;
};

//lista pionkow
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
    struct ListaRuchow* lista;
};

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

//------------------W C Z Y T A J------------------------------//

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

//usun liste

struct listaRuchow* usunListe(struct listaRuchow* l)
{
    struct listaRuchow* tmp = l;
    while(l->nastepny)
    {
        l=l->nastepny;
        usunListe(l);
    }
    free(tmp);
}

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

//pokaz liste

void pokazListeX(struct PionkiX* lx)
{
  
    printf("Z: %d, %d; Wartosc: %d\n",lx->pionek.wiersz,lx->pionek.kolumna,lx->pionek.wartosc);
    if(lx->nastepny)
    {
        lx = lx->nastepny;
        pokazListeX(lx);
    }
}

void pokazListeY(struct PionkiY* ly)
{
  
    printf("Z: %d, %d; Wartosc: %d\n",ly->pionek.wiersz,ly->pionek.kolumna,ly->pionek.wartosc);
    if(ly->nastepny)
    {
        ly = ly->nastepny;
        pokazListeY(ly);
    }
}

//generator ruchow (AI)

void listaRuchowX(struct PionkiX* lX,struct warcaby* gra)
{
    struct listaRuchow* lR = NULL;
    struct listaRuchow* glowa;
    int w = lX->pionek.wiersz;
    int k = lX->pionek.kolumna;
    int wartosc = lX->pionek.wartosc;
    lX->lista = NULL;
    if(wartosc == 5)
    {
        if(w < gra->rozmiar - 2) //generowanie ruchu z mozliwym zbiciem
        {
            for(int i = 0; i < 2; i++)
            {
                if(i == 0)
                {
                    if(k - 1 < 0)
                        continue;
                    else
                    {
                        if(gra->plansza[w+1][k-1] == '-')
                        {
                            if(lR == NULL)
                            {
                                lR = malloc(sizeof(struct listaRuchow));
                                glowa = lR;
                                lR->ri.z[0] = w;
                                lR->ri.z[1] = k;
                                lR->ri.na[0] = w+1;
                                lR->ri.na[1] = k-1;
                                lR->nastepny = NULL;
                                lR->ri.wartosc = 5;
                                printf("JESTEM TU?");
                            }
                            else
                            {
                                while(lR->nastepny)
                                {
                                    lR = lR->nastepny; 
                                }
                                lR->nastepny = malloc(sizeof(struct listaRuchow));
                                lR = lR->nastepny;
                                lR = malloc(sizeof(struct listaRuchow));
                                lR->ri.z[0] = w;
                                lR->ri.z[1] = k;
                                lR->ri.na[0] = w+1;
                                lR->ri.na[1] = k-1;
                                lR->ri.wartosc = 5;
                                lR->nastepny = NULL;
                            }
                            
                        }
                    }
                }
            }
            if(gra->plansza[w+1][k-1] == 'O' || gra->plansza[w+1][k-1] == '@')
                {
                    if(CzyIstniejeBicie(gra,w,k) == 1)
                        {

                        }
                    }
                }        
                }
                else
                {
                    if(k + 1 > gra->rozmiar-1)
                        continue;
                    else
                    {
                        if(gra->plansza[w+1][k+1] == '-')
                        {
                            if(lR == NULL)
                            {
                                lR = malloc(sizeof(struct listaRuchow));
                                glowa = lR;
                                lR->ri.z[0] = w;
                                lR->ri.z[1] = k;
                                lR->ri.na[0] = w+1;
                                lR->ri.na[1] = k+1;
                                lR->ri.wartosc = 5;
                                lR->nastepny = NULL;
                                printf("BYLEM TU!\n");
                            }
                            else
                            {
                                while(lR->nastepny)
                                {
                                    lR = lR->nastepny; 
                                }
                                lR->nastepny = malloc(sizeof(struct listaRuchow));
                                lR = lR->nastepny;
                                lR->ri.z[0] = w;
                                lR->ri.z[1] = k;
                                lR->ri.na[0] = w+1;
                                lR->ri.na[1] = k+1;
                                lR->ri.wartosc = 5;
                                lR->nastepny = NULL;
                                printf("TU TEZ!\n");
                            }
                            
                        }
                    }
                }
                
            }
            if(CzyIstniejeBicie(gra,w,k) == 1)
            {
                if(lR == NULL)
                {
                    lR = malloc(sizeof(struct listaRuchow));
                    glowa = lR;
                    lR->ri.z[0] = w;
                    lR->ri.z[1] = k;
                    lR->ri.na[0] = w+1;
                    lR->ri.na[1] = k+1;
                    lR->ri.wartosc = 5;
                    lR->nastepny = NULL;
                    printf("BYLEM TU!\n");
                }
                else
                {
                    while(lR->nastepny)
                    {
                        lR = lR->nastepny; 
                    }
                    lR->nastepny = malloc(sizeof(struct listaRuchow));
                    lR = lR->nastepny;
                    lR->ri.z[0] = w;
                    lR->ri.z[1] = k;
                    lR->ri.na[0] = w+1;
                    lR->ri.na[1] = k+1;
                    lR->ri.wartosc = 5;
                    lR->nastepny = NULL;
                    printf("TU TEZ!\n");
                }            
            }

        }
        else //jestesmy na przedostatniej krawedzi
        {
            for(int i = 0; i < 2; i++)
            {
                if(i == 0)
                {
                    if(k - 1 < 0)
                        continue;
                    else
                    {
                        if(gra->plansza[w+1][k-1] == '-')
                        {
                            if(lR == NULL)
                            {
                                lR = malloc(sizeof(struct listaRuchow));
                                glowa = lR;
                                lR->ri.z[0] = w;
                                lR->ri.z[1] = k;
                                lR->ri.na[0] = w+1;
                                lR->ri.na[1] = k-1;
                                lR->nastepny = NULL;
                                lR->ri.wartosc = 5;
                                //printf("JESTEM TU?");
                            }
                            else
                            {
                                while(lR->nastepny)
                                {
                                    lR = lR->nastepny; 
                                }
                                lR->nastepny = malloc(sizeof(struct listaRuchow));
                                lR = lR->nastepny;
                                lR = malloc(sizeof(struct listaRuchow));
                                lR->ri.z[0] = w;
                                lR->ri.z[1] = k;
                                lR->ri.na[0] = w+1;
                                lR->ri.na[1] = k-1;
                                lR->ri.wartosc = 5;
                                lR->nastepny = NULL;
                            }
                            
                        }
                    }
                    
                }
                else
                {
                    if(k + 1 > gra->rozmiar-1)
                        continue;
                    else
                    {
                        if(gra->plansza[w+1][k+1] == '-')
                        {
                            if(lR == NULL)
                            {
                                lR = malloc(sizeof(struct listaRuchow));
                                glowa = lR;
                                lR->ri.z[0] = w;
                                lR->ri.z[1] = k;
                                lR->ri.na[0] = w+1;
                                lR->ri.na[1] = k+1;
                                lR->ri.wartosc = 5;
                                lR->nastepny = NULL;
                                //printf("BYLEM TU!\n");
                            }
                            else
                            {
                                while(lR->nastepny)
                                {
                                    lR = lR->nastepny; 
                                }
                                lR->nastepny = malloc(sizeof(struct listaRuchow));
                                lR = lR->nastepny;
                                lR->ri.z[0] = w;
                                lR->ri.z[1] = k;
                                lR->ri.na[0] = w+1;
                                lR->ri.na[1] = k+1;
                                lR->ri.wartosc = 5;
                                lR->nastepny = NULL;
                                //printf("TU TEZ!\n");
                            }
                            
                        }
                    }
                }
                
            }
        }           
    }
    if(lR)
    {
        lX->lista = glowa;   
    }
    
    while(lX->lista)
    {
        printf("\nz: %d, %d; na: %d, %d; wartosc: %d\n",lX->lista->ri.z[0],lX->lista->ri.z[1],lX->lista->ri.na[0],lX->lista->ri.na[1],lX->lista->ri.wartosc);
        lX->lista=lX->lista->nastepny;
    }
        //printf("\nz: %d, %d; na: %d, %d; wartosc: %d\n",lX->lista->ri.z[0],lX->lista->ri.z[1],lX->lista->ri.na[0],lX->lista->ri.na[1],lX->lista->ri.wartosc);

    if(lX->nastepny)
        listaRuchowX(lX->nastepny,gra);
}


//glowna funckja

int main()
{
    struct PionkiX* listaX = NULL;
    struct PionkiY* listaY = NULL;    
    struct warcaby* gra = wczytaj("zapis.txt",&listaX,&listaY);

    wyswietl(gra);
    if(listaX)
        pokazListeX(listaX);
    printf("\n");
    if(listaY)
        pokazListeY(listaY);

    listaRuchowX(listaX,gra);



    return 0;
}

//---------------------C Z Y - I S T N I E J E - B I C I E-------------//

int CzyIstniejeBicie(struct warcaby* gra,int wiersz1, int kolumna1)
{
    if(wiersz1 < 2 || wiersz1 >= (gra->rozmiar - 2) || kolumna1 < 2 || kolumna1 >= (gra->rozmiar - 2))
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
    if((((gra->plansza[wiersz1 + 1][kolumna1 - 1] == 'O') || (gra->plansza[wiersz1 + 1][kolumna1 - 1] == '@')) && (gra->plansza[wiersz1 + 2][kolumna1 - 2] == '-')) || (((gra->plansza[wiersz1 + 1][kolumna1 + 1] == 'O') || (gra->plansza[wiersz1 + 1][kolumna1 + 1] == '@')) && (gra->plansza[wiersz1 + 2][kolumna1 + 2] == '-')))
        return 1; 
    return 0;
}

//---------------------C Z Y - M O Z N A - Z B I C - Z - K R A W E D Z I-------------//

int CzymoznaKraw(struct warcaby* gra,int wiersz1, int kolumna1)
{

    for(int i = 0; i < 3; i+=2)
    {
        if((kolumna1 - 1 + i) < 0 || (kolumna1 -1 + i) >= gra->rozmiar)
            continue;
        if((gra->plansza[wiersz1 + 2][kolumna1 -2 + (2*i)] == '-'))
           return 1;
    }

    return 0;
}
