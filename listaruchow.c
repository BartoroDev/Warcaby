#include <stdio.h>
#include <stdlib.h>

struct warcaby{
    int rozmiar;
    char** plansza; 
    int gracz;
};

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
    int w = lX->pionek.wiersz;
    int k = lX->pionek.kolumna;
    int wartosc = lX->pionek.wartosc;
    lX->lista->ri.z[0] = w;
    lX->lista->ri.z[1] = k;
    if(wartosc == 5)
    {
        if(w < gra->rozmiar) //generowanie ruchu z mozliwym zbiciem
        {

        }
        else //jestesmy na przedostatniej krawedzi
        {
            if(k -1 < 0 || k + 1 > gra->rozmiar -1)
                continue;
            else if(gra->plansza[w+1][k-1] == 'O' || gra->plansza[w+1][k-1] == '@')
                lX->lista
        }           
    }
}


//glowna funckja

int main()
{
    struct PionkiX* listaX = NULL;
    struct PionkiY* listaY = NULL;    
    struct warcaby* gra = wczytaj("zapis.txt",&listaX,&listaY);

    wyswietl(gra);
    pokazListeX(listaX);
    printf("\n");
    pokazListeY(listaY);



    return 0;
}
