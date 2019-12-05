struct ruch
{
    int z[2],na[2];
    int wartosc;
};

struct listaRuchow{
    struct ruch ri;
    struct listaRuchow* nastepny;
};

struct listaRuchow* dodaj(struct listaRuchow* l,struct ruch r)
{
    struct listaRuchow* x = malloc(sizeof(struct listaRuchow));
    x->ri = r;
    x->nastepny = NULL;
    if(!l)
        return x;
    struct listaRuchow* glowa = l;
    while(l->nastepny)
        l=l->nastepny;
    l->nastepny = x;
    return glowa;
}

struct listaRuchow* usunListe(struct listaRuchow* l)
{
    struct listaRuchow* tmp = l;
    while(l->nastepny)
    {
        l=l->nastepny;
        usunListe(l);
    }
    free(tmp)
}

void pokazListe(struct listaRuchow* l)
{
    while(l->nastepny)
    {
        printf("\nZ: %d,%d    Na: %d,%d   Wartosc: %d",l->ri.z[1],l->ri.z[2],l->ri.na[1],l->ri.na[2],l->ri.wartosc);
        l = l->nastepny;
        pokazListe(l);
    }
}