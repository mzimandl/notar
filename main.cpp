#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>

#include "functions.h"
#include "ctimer.h"

#include "cvypinac.h"
#include "cprepinac.h"
#include "ctlacitko.h"
#include "cklaviatura.h"
#include "cosnova.h"

#define width 800
#define height 600
#define bpp 0

#define klaviatura_x1 230
#define klaviatura_x2 710
#define klaviatura_y1 250
#define klaviatura_y2 550

#define ovladani_x 10
#define ovladani_y 460

#define Tick 20

TTF_Font *font;

SDL_Surface *screen = NULL;
SDL_Surface *tl_pause = NULL;
SDL_Surface *tl_play = NULL;
SDL_Surface *text_stat_noty = NULL;
SDL_Surface *text_stat_cas = NULL;
SDL_Surface *vp_housl_vyp = NULL;
SDL_Surface *vp_housl_zap = NULL;
SDL_Surface *vp_bas_vyp = NULL;
SDL_Surface *vp_bas_zap = NULL;
SDL_Surface *krbe_vyp = NULL;
SDL_Surface *krbe_zap = NULL;
SDL_Event event;

Uint32 bila;

cTimer Statistika;
cTimer Timer;

cKlaviatura Klaviatura;
cTlacitko Pauza;
cTlacitko Otext;

cVypinac KrBe;
cVypinac KlicG;
cVypinac KlicF;

cPrepinac DurMol;
cPrepinac p_Stup;
cPrepinac p_Pocet;

time_t cas;

cOsnova Lajny;

//-----Init a Final

bool init()
{
	printf("Inicializuji SDL ...");

	if ( SDL_Init(SDL_INIT_EVERYTHING) == -1 )
	{
		printf("\n Nemohu inicializovat SDL: %s\n", SDL_GetError());
		return false;
	}
	
	screen = SDL_SetVideoMode(width, height, bpp, SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( screen == NULL )
	{
		printf("\n Nemohu nastavit obraz: %s\n", SDL_GetError());
		return false;
	}
	
	printf(" hotovo\n");
	//-------------------------------
	printf("Inicializuji TTF ...");
	
	if ( TTF_Init() == -1 )
	{
		printf("\n Nemohu inicializovat fonty: %s\n", TTF_GetError());
		return false;
	}
	
	font = TTF_OpenFont("data/font.ttf",26);
	if (font == NULL)
	{
		printf("\n Chyba pri nacitani fontu: %s\n", TTF_GetError());
		return false;
	}
	
	printf(" hotovo\n");
	//-------------------------------
	printf("Nacitam data ...");
	
	tl_play = LoadIMG("data/play.png");
	tl_pause = LoadIMG("data/pause.png");
	vp_housl_vyp = LoadIMG("data/housl_vyp.png");
	vp_housl_zap = LoadIMG("data/housl_zap.png");
	vp_bas_vyp = LoadIMG("data/bas_vyp.png");
	vp_bas_zap = LoadIMG("data/bas_zap.png");
	krbe_vyp = LoadIMG("data/krbe_vyp.png");
	krbe_zap = LoadIMG("data/krbe_zap.png");
	
	printf(" hotovo\n");
	//-------------------------------
	printf("Nastavuji zbytek ...");
	
	bila = SDL_MapRGB(screen->format, 255, 255, 255);
	
	cas = time(NULL);
	srand(cas);	
	
	Pauza.Init(ovladani_x,ovladani_y,ovladani_x+60,ovladani_y+60);
	Pauza.NastavBarvuNic(255,255,255);
	Pauza.NastavBarvuNad(235,235,235);
	Pauza.NastavBarvuKlik(200,200,200);
	Pauza.NastavObrazek(tl_play);
	
	Otext.Init(ovladani_x,ovladani_y-70,ovladani_x+60,ovladani_y-10);
	Otext.NastavBarvuNic(255,255,255);
	Otext.NastavBarvuNad(235,235,235);
	Otext.NastavBarvuKlik(200,200,200);
	Otext.NastavPopisCenterBlack("abc", font);
	
	KlicG.Init(ovladani_x,10,ovladani_x+60,70,true);
	KlicG.NastavObrazky(vp_housl_vyp, vp_housl_zap);
	KlicF.Init(ovladani_x,80,ovladani_x+60,140,true);
	KlicF.NastavObrazky(vp_bas_vyp, vp_bas_zap);
	KrBe.Init(ovladani_x,150,ovladani_x+60,210,false);
	KrBe.NastavObrazky(krbe_vyp, krbe_zap);
	
	p_Stup.Init(ovladani_x,220,ovladani_x+60,280,-7,7,0);
	p_Stup.NastavText("C", font);
	
	p_Pocet.Init(ovladani_x+70,40,ovladani_x+70+60,100,0,1,0);
	p_Pocet.NastavText("1", font);
	
	DurMol.Init(ovladani_x,290,ovladani_x+60,350,0,1,0);
	DurMol.NastavText("dur", font);
	
	Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,0);
	Klaviatura.SkrytPopis(true);
	
	Lajny.Init();
	Lajny.Nastaveni(klaviatura_x1, klaviatura_x2, 60, -5, 13);
	Lajny.Aktivni(false);
	
	SDL_WM_SetCaption( "Notář", NULL );
	
	Statistika.Reset();
	Timer.Reset();
	printf("hotovo\n");
	
	return true;
}

void final()
{
	printf("Ukoncuji ...");
	Pauza.DeInit();
	Otext.DeInit();
	Klaviatura.Zrus();
	
	DurMol.DeInit();
	p_Stup.DeInit();
	p_Pocet.DeInit();
	
	TTF_CloseFont(font);
	SDL_FreeSurface(tl_pause);
	SDL_FreeSurface(tl_play);
	SDL_FreeSurface(text_stat_noty);
	SDL_FreeSurface(text_stat_cas);
	SDL_FreeSurface(vp_housl_vyp);
	SDL_FreeSurface(vp_housl_zap);
	SDL_FreeSurface(vp_bas_vyp);
	SDL_FreeSurface(vp_bas_zap);
	SDL_FreeSurface(krbe_vyp);
	SDL_FreeSurface(krbe_zap);
	
	SDL_Quit();
	printf(" hotovo\n");
}

//-----Statistiky

bool ZapisStatistiku(int cyklus, int cislo, int celkem, const char* vystup)
{
	int i;
	char pomocny[255];
	FILE *out;
	
	out = fopen(vystup,"a");
	if (!out)
	{
		printf("Nastala chyba pri zapisu.\n");
		return false;
	}
	
	if (cyklus == 0) {
		struct tm *ltmcas;
		
		ltmcas = localtime(&cas);
		strftime(pomocny, 255, "#%d-%m-%Y	", ltmcas);
		fputs(pomocny,out);
		sprintf(pomocny,"statistika po %i notach\n", celkem);
		fputs(pomocny,out);
	}
	
	sprintf(pomocny,"%i	%i\n", cyklus, cislo);
	fputs(pomocny,out);
	
	fclose(out);
	return true;
}

//-----Statistiky-Vykresleni

void StatNotNaSurface(int a, int b, int c)
{
	char stat_noty[20];
	std::string d;
	sprintf(stat_noty, "%i - %i - %i", a, b, c);
	d = stat_noty;
	SDL_FreeSurface(text_stat_noty);
	text_stat_noty = TTF_RenderUTF8_Shaded(font, d.c_str(), {100,100,100}, {255,255,255});
}

void StatCasNaSurface(int a, int b)
{
	char stat_noty[20];
	std::string c;
	sprintf(stat_noty, "%i - %.1f", a/1000, b/1000.0);
	c = stat_noty;
	SDL_FreeSurface(text_stat_cas);
	text_stat_cas = TTF_RenderUTF8_Shaded(font, c.c_str(), {100,100,100}, {255,255,255});
}

//-----Hlavni cast

int main(int argc, char **argv)
{	
	if ( !init() ) return 1;
	
	bool horni, dolni;
	
	bool cycling = true;
	bool pauzuj = true;
	bool klic;
	bool losuj;
	bool znacit_kl = false;
	
	int stat_celkem = 0;
	#define stat_pocet 20
	int stat_spravnych = 0;
	int stat_cyklus = 0;
	int stat = 0;
	
	StatNotNaSurface(stat_celkem, stat_spravnych, stat_cyklus);
	StatCasNaSurface(0, stat);
	
	while ( cycling )	//Hlavní cyklus
	{
		Timer.Start();
		
		//-----Udalosti-----Zacatek
		while ( SDL_PollEvent(&event) )
		{
			switch ( event.type )
			{
				case SDL_KEYDOWN:
					switch ( event.key.keysym.sym )
					{
						case SDLK_ESCAPE:
							cycling = false;
							break;
						//SDLK_RETURN, SDLK_SPACE, SDLK_BACKSPACE, SDLK_TAB
					}
					break;				
				case SDL_QUIT:
					cycling = false;
					break;
			}
		}
		
		Pauza.Udalost(&event);
		Otext.Udalost(&event);
		Klaviatura.Udalosti(&event);
		
		if (pauzuj) {
			KlicG.Udalost(&event);
			KlicF.Udalost(&event);
			KrBe.Udalost(&event);
			DurMol.Udalost(&event);
			p_Stup.Udalost(&event);
			p_Pocet.Udalost(&event);
		}
		//-----Udalosti-----Konec
		
		//-----Funkce
		if (Otext.Odstisknuto()) {
			znacit_kl = !znacit_kl;
			Klaviatura.SkrytPopis(!znacit_kl);
			
			if (znacit_kl) Otext.ZmenPopis("-");
			else Otext.ZmenPopis("abc");
		}
				
		if (Pauza.Odstisknuto()) {	//Stisknutí tlačítka pauza
			pauzuj = !pauzuj;	//Nastavi opacny stav
			
			if (!pauzuj)		//Pokud pri odpauzovani neni vybrany klic, zase zapauzuje, jinak vyber nastavi do osnovy
				if (KlicG.Zapnuto())
					if (KlicF.Zapnuto()) Lajny.NastavKlic(2);
					else Lajny.NastavKlic(0);
				else
					if (KlicF.Zapnuto()) Lajny.NastavKlic(1);
					else pauzuj = true;
			
			if (KrBe.Zapnuto()) Lajny.NastavPredznam(true); else Lajny.NastavPredznam(false);	//Nastavi posuvky
			
			Lajny.NastavStupnici(p_Stup.Vybrano(), DurMol.Vybrano()); 	//Nastavi stupnici
			Lajny.NastavPocet(p_Pocet.Vybrano());
					
			if (pauzuj) {	//natavi ovladaci prvky pri pauze
				Pauza.NastavObrazek(tl_play);
				Statistika.Stop();
				
				Lajny.Aktivni(false);
				
				KlicG.Aktivuj(true);
				KlicF.Aktivuj(true);
				KrBe.Aktivuj(true);
				DurMol.Aktivuj(true);
				p_Stup.Aktivuj(true);
				p_Pocet.Aktivuj(true);
			} else {	//natavi ovladaci prvky pri odpauzovani
				Pauza.NastavObrazek(tl_pause);
				Statistika.Continue();
				
				Lajny.Aktivni(true);
				Lajny.Generuj();
				
				KlicG.Aktivuj(false);
				KlicF.Aktivuj(false);
				KrBe.Aktivuj(false);
				DurMol.Aktivuj(false);
				p_Stup.Aktivuj(false);
				p_Pocet.Aktivuj(false);
				
				//losuj = true;
				
				horni = true;
				dolni = true;
			}
		}
		
		if (!pauzuj) {	//Pokud není pauza
			/*if (losuj) {	//Losování nové noty
				Lajny.Nova();
				losuj = false;
			}*/
			
			if (horni && dolni) {
				Lajny.Nova();
				if (p_Pocet.Vybrano()) horni = false; else horni = true;
				dolni = false;
			}
			
			if (!dolni && Klaviatura.OdstisknutaKlavesa() == Lajny.Aktualni() && Klaviatura.OdstisknutaLevaKlavesa()) {	//Stisknuti spravne klavesy
				stat_spravnych++;
				stat_celkem++;
				StatNotNaSurface(stat_celkem, stat_spravnych, stat_cyklus);
				//losuj = true;
				dolni = true;
			}
			
			if (!horni && Klaviatura.OdstisknutaKlavesa() == Lajny.Aktualni2() && p_Pocet.Vybrano() && !Klaviatura.OdstisknutaLevaKlavesa()) {
				stat_spravnych++;
				stat_celkem++;
				StatNotNaSurface(stat_celkem, stat_spravnych, stat_cyklus);
				horni = true;
			}
			
			if (stat_spravnych == stat_pocet) {	//Uložení statistiky po uhadnuti dostatecneho poctu not
				stat = Statistika.GetTicks()/stat_pocet;
				
				ZapisStatistiku(stat_cyklus, stat, stat_pocet, Lajny.GenerujText());
				
				stat_cyklus++;
				stat_spravnych = 0;
				
				StatNotNaSurface(stat_celkem, stat_spravnych, stat_cyklus);
				
				Statistika.Start();
			}
			
			StatCasNaSurface(Statistika.GetTicks(), stat);	//Vypise cas testu
		} else {	//Pokud je pauza
			if (KlicG.ZmenaStavu() || KlicF.ZmenaStavu() || KrBe.ZmenaStavu() || DurMol.ZmenaStavu() || p_Stup.ZmenaStavu() || p_Pocet.ZmenaStavu()) {	//Vynulovani score pri zmene podminek testu
				stat_celkem = 0;
				stat_cyklus = 0;
				stat_spravnych = 0;
				stat = 0;
				Statistika.Reset();
				
				StatNotNaSurface(stat_celkem, stat_spravnych, stat_cyklus);
				StatCasNaSurface(Statistika.GetTicks(), stat);
			}
			
			if (DurMol.ZmenaStavu()) switch (DurMol.Vybrano()) {	//Zmeni text prepinace typu stupnice
				case 0:	DurMol.NastavText("dur",font); break;
				case 1: DurMol.NastavText("mol",font); break;
			}
			
			if (p_Stup.ZmenaStavu()) {
				switch (p_Stup.Vybrano()) {	//Zmeni text prepinace stupnic
				case -7: p_Stup.NastavText("Ces",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,12);
					break;
				case -6: p_Stup.NastavText("Ges",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,7);
					break;
				case -5: p_Stup.NastavText("Des",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,1);
					break;
				case -4: p_Stup.NastavText("As",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,9);
					break;					
				case -3: p_Stup.NastavText("Es",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,3);
					break;
				case -2: p_Stup.NastavText("B",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,11);
					break;				
				case -1: p_Stup.NastavText("F",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,6);
					break;
				case 0:	p_Stup.NastavText("C",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,0);
					break;
				case 1: p_Stup.NastavText("G",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,8);
					break;
				case 2: p_Stup.NastavText("D",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,2);
					break;
				case 3: p_Stup.NastavText("A",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,10);
					break;
				case 4: p_Stup.NastavText("E",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,4);
					break;
				case 5: p_Stup.NastavText("H",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,12);
					break;
				case 6: p_Stup.NastavText("Fis",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,7);
					break;
				case 7: p_Stup.NastavText("Cis",font);
					Klaviatura.Nastav(klaviatura_x1,klaviatura_y1,klaviatura_x2,klaviatura_y2,font,1);
					break;
				}
				Klaviatura.SkrytPopis(!znacit_kl);
			}
			
			if (p_Pocet.ZmenaStavu()) switch (p_Pocet.Vybrano()) {	//Zmeni text prepinace stupnic
				case 0: p_Pocet.NastavText("1",font); break;
				case 1: p_Pocet.NastavText("2",font); break;
			}
		}
		//-----Funkce-----Konec
		
		//-----Vykreslovani
		SDL_FillRect(screen,NULL,bila);
		
		Lajny.Vykresli(screen);
		
		if (p_Pocet.Vybrano()) {
			if (!pauzuj && horni) filledCircleRGBA(screen,200,80,10,200,200,200,255);
			else circleRGBA(screen,200,80,10,200,200,200,255);
			if (!pauzuj && dolni) filledCircleRGBA(screen,200,180,10,200,200,200,255);
			else circleRGBA(screen,200,180,10,200,200,200,255);
		}
		
			//Ovladací prvky
		Pauza.Vykresli(screen);
		Otext.Vykresli(screen);
		KlicG.Vykresli(screen);
		KlicF.Vykresli(screen);
		KrBe.Vykresli(screen);
		DurMol.Vykresli(screen);
		p_Stup.Vykresli(screen);
		p_Pocet.Vykresli(screen);
		Klaviatura.Vykresli(screen);
		
			//Statistiky
		BlitScreen(text_stat_noty, screen, ovladani_x, ovladani_y+70);
		BlitScreen(text_stat_cas, screen, ovladani_x, ovladani_y+100);
		
		SDL_Flip(screen);
		//-----Vykreslovani-----Konec
		
		while (Timer.GetTicks() < Tick) {}
	}
	
	final();
	
	return 0;
}
