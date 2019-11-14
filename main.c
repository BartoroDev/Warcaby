void nowaPlansza(struct warcaby* gra){
for(int i=0;i<gra->rozmiar;i++){
    for(int j=0;j<gra->rozmiar;j++){          
        gra->plansza[i][j] = ' ';          //zapisywanie calej planszy spacjmi
    }
}
    for(int i=0;i<3;i++){
        for(int j=0;j<gra->rozmiar;j++){
            if((i+j)%2==0)
            gra->plansza[i][j]=' ';
            else
           gra->plansza[i][j]='X';           //zapisywanie pierwszych trzech wierszy X-ami
        }
    }
    for(int i=(gra->rozmiar-3);i<gra->rozmiar;i++){
        for(int j=0;j<gra->rozmiar;j++){
            if((i+j)%2==0)
            gra->plansza[i][j]=' ';
            else
           gra->plansza[i][j]='O';     //zapisywanie ostatnich trzech wierszy O-ami
        }
    }
    for(int i = 0; i < gra->rozmiar; i++,printf("\n"))
        for(int j = 0; j < gra->rozmiar; j++)
            printf("%2c",gra->plansza[i][j]);    //wyswietlanie
}
