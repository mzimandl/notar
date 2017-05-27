// vyžaduje ctlacitko.h

class cKlaviatura
{
	private:
		cTlacitko pole[12];		
		bool cerna_klavesa;
		bool leva_klavesa;
		
		int i;
		
		int PermutaceKlaves(int cislo);
	public:
		void Nastav(int x1, int y1, int x2, int y2, TTF_Font *font, int prvni);
		void Zrus();
		
		void Vykresli(SDL_Surface *plocha);
		void Udalosti(SDL_Event *stalose);
		
		int StisknutaKlavesa();
		int OdstisknutaKlavesa();
		
		bool OdstisknutaLevaKlavesa();
		
		void SkrytPopis(bool a);
};

void cKlaviatura::Nastav(int x1, int y1, int x2, int y2, TTF_Font *font, int prvni)
{
	int sirka, vyska, odsazeni, pomocne;
	
	sirka = (x2 - x1)/7;
	vyska = y2 - y1;
	pomocne = 0;
	
	odsazeni = prvni*sirka/2;
	
	for (i=0;i<7;i++) {
		if (odsazeni>i*sirka) pole[i].Init(x1+i*sirka+(sirka*7-odsazeni),y1,x1+(i+1)*sirka+(sirka*7-odsazeni),y2);
		else pole[i].Init(x1+i*sirka-odsazeni,y1,x1+(i+1)*sirka-odsazeni,y2); // bile klavesy
			
	}
	for (i=0;i<2;i++) {
		if (odsazeni>sirka/2+i*sirka+sirka/4) pole[7+i].Init(x1+sirka/2+i*sirka+sirka/4+(sirka*7-odsazeni),y1,x1+sirka/2+(i+1)*sirka-sirka/4+(sirka*7-odsazeni),y1+2*vyska/3); // dvojice cernych klaves
		else pole[7+i].Init(x1+sirka/2+i*sirka+sirka/4-odsazeni,y1,x1+sirka/2+(i+1)*sirka-sirka/4-odsazeni,y1+2*vyska/3);
	}
	
	for (i=0;i<3;i++) {
		if (odsazeni>sirka/2+(i+3)*sirka+sirka/4) pole[9+i].Init(x1+sirka/2+(i+3)*sirka+sirka/4+(sirka*7-odsazeni),y1,x1+sirka/2+(i+4)*sirka-sirka/4+(sirka*7-odsazeni),y1+2*vyska/3); // trojice cernych klaves
		else pole[9+i].Init(x1+sirka/2+(i+3)*sirka+sirka/4-odsazeni,y1,x1+sirka/2+(i+4)*sirka-sirka/4-odsazeni,y1+2*vyska/3);
	}
	
	for (i=0;i<7;i++) {
		pole[i].NastavBarvuNic(255,255,255);
		pole[i].NastavBarvuNad(235,235,235);
		pole[i].NastavBarvuKlik(200,200,200);
		
	}
	
	for (i=7;i<12;i++) {
		pole[i].NastavBarvuNic(0,0,0);
		pole[i].NastavBarvuNad(70,70,70);
		pole[i].NastavBarvuKlik(50,50,50);
		pole[i].KresliBezOkraje();
	}
	
	pole[0].NastavPopis(sirka/2,vyska/3,0,0,0,"c", font);
	pole[1].NastavPopis(sirka/2,vyska/3,0,0,0,"d", font);
	pole[2].NastavPopis(sirka/2,vyska/3,0,0,0,"e", font);
	pole[3].NastavPopis(sirka/2,vyska/3,0,0,0,"f", font);
	pole[4].NastavPopis(sirka/2,vyska/3,0,0,0,"g", font);
	pole[5].NastavPopis(sirka/2,vyska/3,0,0,0,"a", font);
	pole[6].NastavPopis(sirka/2,vyska/3,0,0,0,"h", font);
	
	pole[7].NastavPopis(sirka/4,vyska/4,255,255,255,"#c", font);
	pole[8].NastavPopis(sirka/4,vyska/4,255,255,255,"#d", font);
	pole[9].NastavPopis(sirka/4,vyska/4,255,255,255,"#f", font);
	pole[10].NastavPopis(sirka/4,vyska/4,255,255,255,"#g", font);
	pole[11].NastavPopis(sirka/4,vyska/4,255,255,255,"#a", font);
	
}

void cKlaviatura::Zrus()
{
	for (i=0;i<12;i++) pole[i].DeInit();
}

void cKlaviatura::Udalosti(SDL_Event *stalose)
{
	int pomocne = -1;
	
	cerna_klavesa = false;
	leva_klavesa = false;
	
	for (i=0;i<12;i++) if (pole[i].Udalost(stalose)) pomocne = i;
	if (pomocne > 6) cerna_klavesa = true;
	if (pomocne > -1) if (pole[pomocne].LeveOdstisknuto()) leva_klavesa = true;
	if (cerna_klavesa) for (i=0;i<7;i++) pole[i].ZrusUdalost();
}

void cKlaviatura::Vykresli(SDL_Surface *plocha)
{
	for (i=0;i<12;i++) pole[i].Vykresli(plocha);
}

int cKlaviatura::PermutaceKlaves(int cislo)
{
	switch (cislo) {
		case 0: return 1;
		case 1: return 3;
		case 2: return 5;
		case 3: return 6;
		case 4: return 8;
		case 5: return 10;
		case 6: return 12;
		case 7: return 2;
		case 8: return 4;
		case 9: return 7;
		case 10: return 9;
		case 11: return 11;
		default: return 0; //NEPLATNA KLAVESA
	}
}

int cKlaviatura::StisknutaKlavesa()
{
	int poradi = 12;
	
	for (i=0;i<12;i++) if (pole[i].Stisknuto()) poradi = i;
	return PermutaceKlaves(poradi);
}

int cKlaviatura::OdstisknutaKlavesa()
{
	int poradi = 12;
	
	for (i=0;i<12;i++) if (pole[i].Odstisknuto()) poradi = i;
	return PermutaceKlaves(poradi);
}

bool cKlaviatura::OdstisknutaLevaKlavesa()
{
	return leva_klavesa;	
}

void cKlaviatura::SkrytPopis(bool a)
{
	for (i=0;i<12;i++) pole[i].SkrytOznaceni(a);
}
