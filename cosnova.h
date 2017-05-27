#define znam_nic 0
#define znam_kriz 1
#define znam_be 2
#define znam_zrus 3

struct nota {
  int hodn;
  int znam;
  bool bas;
};

#define vzdalenost 10
#define poc_bas 11
#define poc_hou 42

#define zasobnik_vel 4

class cOsnova {
	private:
		SDL_Surface *bklic;
		SDL_Surface *hklic;
		SDL_Surface *cnota;
		SDL_Surface *kriz;
		SDL_Surface *be;
		
		bool posun[7];
		
		int minh, maxh;
		int klic;
		bool predznam;
		
		int stupnice;
		bool mol;
		
		int x1,x2,y;
		nota zasobnik[zasobnik_vel];
		nota zasobnikh[zasobnik_vel];
		
		bool dvojite;
		
		bool aktivni;
	public:
		cOsnova();
		~cOsnova();
		
		void Init();
		void Nastaveni(int posx1, int posx2, int posy, int min, int max);
		void Vykresli(SDL_Surface *plocha);
		void Aktivni(bool a);
		
		void Generuj();
		void Nova();
		
		int Aktualni();
		int Aktualni2();
		
		void NastavKlic(int a);
		void NastavPredznam(bool a);
		void NastavStupnici(int a, bool b);
		void NastavPocet(int a);
		
		char* GenerujText();
};

cOsnova::cOsnova()
{
}

cOsnova::~cOsnova()
{
	SDL_FreeSurface(hklic);
	SDL_FreeSurface(bklic);
	SDL_FreeSurface(cnota);
	SDL_FreeSurface(kriz);
	SDL_FreeSurface(be);
}

void cOsnova::Init()
{
	hklic = LoadIMG("data/hklic.png");
	bklic = LoadIMG("data/bklic.png");
	cnota = LoadIMG("data/cnota.png");
	kriz = LoadIMG("data/kriz.png");
	be = LoadIMG("data/be.png");
}

void cOsnova::Nastaveni(int posx1, int posx2, int posy, int min, int max)
{	
	x1 = posx1;
	x2 = posx2;
	y = posy;
	minh = min;
	maxh = max;
}

void cOsnova::Vykresli(SDL_Surface *plocha)
{
	#define nulova 8
	#define poz_noty_poc 20
	#define mezera 20
	#define polosirka 13
	
	int i, j, odsazeni = mezera;
	int odsazeni2 = mezera;
	bool kresliklic, kreslibas;
	
	for (i=0;i<5;i++) {
		lineRGBA(plocha, x1, y+vzdalenost*i, x2, y+vzdalenost*i, 0, 0, 0, 255);
		lineRGBA(plocha, x1, y+vzdalenost*i+100, x2, y+vzdalenost*i+100, 0, 0, 0, 255);
	}
	lineRGBA(plocha, x1, y, x1, y+vzdalenost*4+100, 0, 0, 0, 255);
	lineRGBA(plocha, x2, y, x2, y+vzdalenost*4+100, 0, 0, 0, 255);
	
	if (aktivni) for (i=0;i<zasobnik_vel;i++) {
		if (dvojite) {
			kresliklic = false;
			if (i == 0) {
				if (zasobnikh[i].bas) {
					BlitScreen(bklic, plocha, x1+odsazeni2, y + vzdalenost - poc_bas);
					odsazeni2+=bklic->w+mezera/3;
					kreslibas = true;
					kresliklic = true;
				} else {
					BlitScreen(hklic, plocha, x1+odsazeni2, y + 3*vzdalenost - poc_hou);
					odsazeni2+=hklic->w+mezera/3;
					kreslibas = false;
					kresliklic = true;
				}
			} else if (i > 0) {
				if (zasobnikh[i].bas) {
					if (zasobnikh[i].bas != zasobnikh[i-1].bas) {
						BlitScreen(bklic, plocha, x1+odsazeni2, y + vzdalenost - poc_bas);
						odsazeni2+=bklic->w+mezera/3;
						kreslibas = true;
						kresliklic = true;
					}
				} else {
					if (zasobnikh[i].bas != zasobnikh[i-1].bas) {
						BlitScreen(hklic, plocha, x1+odsazeni2, y + 3*vzdalenost - poc_hou);
						odsazeni2+=hklic->w+mezera/3;
						kreslibas = false;
						kresliklic = true;
					}
				}
			}
			
			if (kresliklic) if (stupnice > 0) switch (stupnice) {
				case 7: CenterBlitScreen(kriz, plocha, x1+odsazeni2+6*kriz->w/2, y + (nulova-4+2*kreslibas)*vzdalenost/2);
				case 6: CenterBlitScreen(kriz, plocha, x1+odsazeni2+5*kriz->w/2, y + (nulova-7+2*kreslibas)*vzdalenost/2);
				case 5: CenterBlitScreen(kriz, plocha, x1+odsazeni2+4*kriz->w/2, y + (nulova-3+2*kreslibas)*vzdalenost/2);
				case 4: CenterBlitScreen(kriz, plocha, x1+odsazeni2+3*kriz->w/2, y + (nulova-6+2*kreslibas)*vzdalenost/2);
				case 3: CenterBlitScreen(kriz, plocha, x1+odsazeni2+2*kriz->w/2, y + (nulova-9+2*kreslibas)*vzdalenost/2);
				case 2: CenterBlitScreen(kriz, plocha, x1+odsazeni2+1*kriz->w/2, y + (nulova-5+2*kreslibas)*vzdalenost/2);
				case 1: CenterBlitScreen(kriz, plocha, x1+odsazeni2+0*kriz->w/2, y + (nulova-8+2*kreslibas)*vzdalenost/2);
					odsazeni2 += (stupnice+2)*kriz->w/2;
			} else if (stupnice < 0) switch (stupnice) {
				case -7: CenterBlitScreen(be, plocha, x1+odsazeni2+6*be->w/2, y + (nulova-1+2*kreslibas)*vzdalenost/2-be->h/4);
				case -6: CenterBlitScreen(be, plocha, x1+odsazeni2+5*be->w/2, y + (nulova-5+2*kreslibas)*vzdalenost/2-be->h/4);
				case -5: CenterBlitScreen(be, plocha, x1+odsazeni2+4*be->w/2, y + (nulova-2+2*kreslibas)*vzdalenost/2-be->h/4);
				case -4: CenterBlitScreen(be, plocha, x1+odsazeni2+3*be->w/2, y + (nulova-6+2*kreslibas)*vzdalenost/2-be->h/4);
				case -3: CenterBlitScreen(be, plocha, x1+odsazeni2+2*be->w/2, y + (nulova-3+2*kreslibas)*vzdalenost/2-be->h/4);
				case -2: CenterBlitScreen(be, plocha, x1+odsazeni2+1*be->w/2, y + (nulova-7+2*kreslibas)*vzdalenost/2-be->h/4);
				case -1: CenterBlitScreen(be, plocha, x1+odsazeni2+0*be->w/2, y + (nulova-4+2*kreslibas)*vzdalenost/2-be->h/4);
					odsazeni2 += (-stupnice+2)*be->w/2;
			}
		
			switch (zasobnikh[i].znam) {
				case -1: CenterBlitScreen(be, plocha, x1+odsazeni2, y + (nulova-zasobnikh[i].hodn)*vzdalenost/2-be->h/4);
					odsazeni2+=be->w/2+2;
					break;
				case 1: CenterBlitScreen(kriz, plocha, x1+odsazeni2, y + (nulova-zasobnikh[i].hodn)*vzdalenost/2);
					odsazeni2+=kriz->w/2+2;
					break;
			}
			
			odsazeni2+=cnota->w/2;

			if (zasobnikh[i].hodn < 0) for (j=0;j<=(-zasobnikh[i].hodn/2);j++)
				lineRGBA(plocha, x1+odsazeni2-polosirka, y+vzdalenost*(4+j), x1+odsazeni2+polosirka, y+vzdalenost*(4+j), 0, 0, 0, 255);
			else if (zasobnikh[i].hodn > nulova) for (j=0;j<=(zasobnikh[i].hodn-nulova)/2;j++)
				lineRGBA(plocha, x1+odsazeni2-polosirka, y-vzdalenost*j, x1+odsazeni2+polosirka, y-vzdalenost*j, 0, 0, 0, 255);
			
			CenterBlitScreen(cnota, plocha, x1+odsazeni2, y + (nulova-zasobnikh[i].hodn)*vzdalenost/2);
			odsazeni2+=cnota->w/2+mezera;
		}
		
		kresliklic = false;
		if (i == 0) {
			if (zasobnik[i].bas) {
				BlitScreen(bklic, plocha, x1+odsazeni, y + vzdalenost + 100 - poc_bas);
				odsazeni+=bklic->w+mezera/3;
				kreslibas = true;
				kresliklic = true;
			} else {
				BlitScreen(hklic, plocha, x1+odsazeni, y + 3*vzdalenost + 100 - poc_hou);
				odsazeni+=hklic->w+mezera/3;
				kreslibas = false;
				kresliklic = true;
			}
		} else if (i > 0) {
			if (zasobnik[i].bas) {
				if (zasobnik[i].bas != zasobnik[i-1].bas) {
					BlitScreen(bklic, plocha, x1+odsazeni, y + vzdalenost + 100 - poc_bas);
					odsazeni+=bklic->w+mezera/3;
					kreslibas = true;
					kresliklic = true;
				}
			} else {
				if (zasobnik[i].bas != zasobnik[i-1].bas) {
					BlitScreen(hklic, plocha, x1+odsazeni, y + 3*vzdalenost + 100 - poc_hou);
					odsazeni+=hklic->w+mezera/3;
					kreslibas = false;
					kresliklic = true;
				}
			}
		}
		
		if (kresliklic) if (stupnice > 0) switch (stupnice) {
				case 7: CenterBlitScreen(kriz, plocha, x1+odsazeni+6*kriz->w/2, y + 100 + (nulova-4+2*kreslibas)*vzdalenost/2);
				case 6: CenterBlitScreen(kriz, plocha, x1+odsazeni+5*kriz->w/2, y + 100 + (nulova-7+2*kreslibas)*vzdalenost/2);
				case 5: CenterBlitScreen(kriz, plocha, x1+odsazeni+4*kriz->w/2, y + 100 + (nulova-3+2*kreslibas)*vzdalenost/2);
				case 4: CenterBlitScreen(kriz, plocha, x1+odsazeni+3*kriz->w/2, y + 100 + (nulova-6+2*kreslibas)*vzdalenost/2);
				case 3: CenterBlitScreen(kriz, plocha, x1+odsazeni+2*kriz->w/2, y + 100 + (nulova-9+2*kreslibas)*vzdalenost/2);
				case 2: CenterBlitScreen(kriz, plocha, x1+odsazeni+1*kriz->w/2, y + 100 + (nulova-5+2*kreslibas)*vzdalenost/2);
				case 1: CenterBlitScreen(kriz, plocha, x1+odsazeni+0*kriz->w/2, y + 100 + (nulova-8+2*kreslibas)*vzdalenost/2);
					odsazeni += (stupnice+2)*kriz->w/2;
			} else if (stupnice < 0) switch (stupnice) {
				case -7: CenterBlitScreen(be, plocha, x1+odsazeni+6*be->w/2, y + 100 + (nulova-1+2*kreslibas)*vzdalenost/2-be->h/4);
				case -6: CenterBlitScreen(be, plocha, x1+odsazeni+5*be->w/2, y + 100 + (nulova-5+2*kreslibas)*vzdalenost/2-be->h/4);
				case -5: CenterBlitScreen(be, plocha, x1+odsazeni+4*be->w/2, y + 100 + (nulova-2+2*kreslibas)*vzdalenost/2-be->h/4);
				case -4: CenterBlitScreen(be, plocha, x1+odsazeni+3*be->w/2, y + 100 + (nulova-6+2*kreslibas)*vzdalenost/2-be->h/4);
				case -3: CenterBlitScreen(be, plocha, x1+odsazeni+2*be->w/2, y + 100 + (nulova-3+2*kreslibas)*vzdalenost/2-be->h/4);
				case -2: CenterBlitScreen(be, plocha, x1+odsazeni+1*be->w/2, y + 100 + (nulova-7+2*kreslibas)*vzdalenost/2-be->h/4);
				case -1: CenterBlitScreen(be, plocha, x1+odsazeni+0*be->w/2, y + 100 + (nulova-4+2*kreslibas)*vzdalenost/2-be->h/4);
					odsazeni += (-stupnice+2)*be->w/2;
			}
		
		switch (zasobnik[i].znam) {
			case -1: CenterBlitScreen(be, plocha, x1+odsazeni, y + 100 + (nulova-zasobnik[i].hodn)*vzdalenost/2-be->h/4);
				odsazeni+=be->w/2+2;
				break;
			case 1: CenterBlitScreen(kriz, plocha, x1+odsazeni, y + 100 + (nulova-zasobnik[i].hodn)*vzdalenost/2);
				odsazeni+=kriz->w/2+2;
				break;
		}
		
		odsazeni+=cnota->w/2;

		if (zasobnik[i].hodn < 0) for (j=0;j<=(-zasobnik[i].hodn/2);j++)
			lineRGBA(plocha, x1+odsazeni-polosirka, y+vzdalenost*(4+j)+100, x1+odsazeni+polosirka, y+vzdalenost*(4+j)+100, 0, 0, 0, 255);
		else if (zasobnik[i].hodn > nulova) for (j=0;j<=(zasobnik[i].hodn-nulova)/2;j++)
			lineRGBA(plocha, x1+odsazeni-polosirka, y-vzdalenost*j+100, x1+odsazeni+polosirka, y-vzdalenost*j+100, 0, 0, 0, 255);
			
		CenterBlitScreen(cnota, plocha, x1+odsazeni, y + 100 + (nulova-zasobnik[i].hodn)*vzdalenost/2);
		odsazeni+=cnota->w/2+mezera;
		
		if (odsazeni < odsazeni2) odsazeni = odsazeni2; else odsazeni2 = odsazeni;
		lineRGBA(plocha, x1+odsazeni, y, x1+odsazeni, y+100+vzdalenost*4, 0, 0, 0, 255);
		
		odsazeni+=mezera;
		odsazeni2+=mezera;
	}
}

void cOsnova::Aktivni(bool a)
{
	aktivni = a;
}

void cOsnova::Generuj()
{
	int i;
	
	for (i=0;i<zasobnik_vel;i++) {
		zasobnik[i].hodn = rand()%(maxh-minh+1)+minh;
		zasobnikh[i].hodn = rand()%(maxh-minh+1)+minh;
		switch (klic) {
			case 0: zasobnik[i].bas = false;
				zasobnikh[i].bas = false;
				break;
			case 1: zasobnik[i].bas = true;
				zasobnikh[i].bas = true;
				break;
			case 2: if (dvojite) {
					zasobnikh[i].bas = false;
					zasobnik[i].bas = true;
				} else {
					zasobnikh[i].bas = rand()%2;
					zasobnik[i].bas = rand()%2;
				}
				break;
		}
		
		if (predznam) {
			zasobnik[i].znam = rand()%3 - 1;
			zasobnikh[i].znam = rand()%3 - 1;
		} else {
			zasobnik[i].znam = 0;
			zasobnikh[i].znam = 0;
		}
	}
}

void cOsnova::Nova()
{
	int i;
	
	for (i=0;i<zasobnik_vel-1;i++) {
		zasobnik[i].hodn = zasobnik[i+1].hodn;
		zasobnik[i].bas = zasobnik[i+1].bas;
		zasobnik[i].znam = zasobnik[i+1].znam;
		zasobnikh[i].hodn = zasobnikh[i+1].hodn;
		zasobnikh[i].bas = zasobnikh[i+1].bas;
		zasobnikh[i].znam = zasobnikh[i+1].znam;
	}

	zasobnik[zasobnik_vel-1].hodn = rand()%(maxh-minh+1)+minh;
	zasobnikh[zasobnik_vel-1].hodn = rand()%(maxh-minh+1)+minh;
	switch (klic) {
		case 0: zasobnik[zasobnik_vel-1].bas = false;
			zasobnikh[zasobnik_vel-1].bas = false;
			break;
		case 1: zasobnik[zasobnik_vel-1].bas = true;
			zasobnikh[zasobnik_vel-1].bas = true;
			break;
		case 2: if (dvojite) {
				zasobnikh[zasobnik_vel-1].bas = false;
				zasobnik[zasobnik_vel-1].bas = true;
			} else {
				zasobnikh[i].bas = rand()%2;
				zasobnik[i].bas = rand()%2;
			}
			break;
	}
	
	if (predznam) {
			zasobnik[zasobnik_vel-1].znam = rand()%3 - 1;
			zasobnikh[zasobnik_vel-1].znam = rand()%3 - 1;
		} else {
			zasobnik[zasobnik_vel-1].znam = 0;
			zasobnikh[zasobnik_vel-1].znam = 0;
		}
}

int cOsnova::Aktualni()
{
	int pom, pom2;
	
	pom = zasobnik[0].hodn;
	while (pom<0) pom+=7;
	
	if (zasobnik[0].bas) pom += 4; else pom += 2;
	pom%=7;
	
	switch (pom) {
		case 0: pom2 = 1 ; break;
		case 1: pom2 = 3; break;
		case 2: pom2 = 5; break;
		case 3: pom2 = 6; break;
		case 4: pom2 = 8; break;
		case 5: pom2 = 10; break;
		case 6: pom2 = 12; break;
	}
	
	if (stupnice>0) pom2 += posun[pom]; else if (stupnice<0) pom2 -= posun[pom];
	
	pom2 += zasobnik[0].znam;
	if (pom2<1) pom2+=12; else if (pom2>12) pom2-=12;
	
	return pom2;	
}

int cOsnova::Aktualni2()
{
	int pom, pom2;
	
	pom = zasobnikh[0].hodn;
	while (pom<0) pom+=7;
	
	if (zasobnikh[0].bas) pom += 4; else pom += 2;
	pom%=7;
	
	switch (pom) {
		case 0: pom2 = 1; break;
		case 1: pom2 = 3; break;
		case 2: pom2 = 5; break;
		case 3: pom2 = 6; break;
		case 4: pom2 = 8; break;
		case 5: pom2 = 10; break;
		case 6: pom2 = 12; break;
	}
	
	if (stupnice>0) pom2 += posun[pom]; else if (stupnice<0) pom2 -= posun[pom];
	
	pom2+=zasobnikh[0].znam;
	if (pom2<1) pom2+=12; else if (pom2>12) pom2-=12;
	
	return pom2;	
}

void cOsnova::NastavKlic(int a)
{
	klic = a;
}

void cOsnova::NastavPredznam(bool a)
{
	predznam = a;
}

void cOsnova::NastavStupnici(int a, bool b)
{
	int i;
	
	stupnice = a;
	mol = b;
	
	for (i=0;i<7;i++) posun[i] = false;
	
	if (stupnice > 0) switch (stupnice) {
		case 7: posun[6]=true;
		case 6: posun[2]=true;
		case 5: posun[5]=true;
		case 4: posun[1]=true;
		case 3: posun[4]=true;
		case 2: posun[0]=true;
		case 1: posun[3]=true;
	} else if (stupnice < 0) switch (stupnice) {
		case -7: posun[3]=true;
		case -6: posun[0]=true;
		case -5: posun[4]=true;
		case -4: posun[1]=true;
		case -3: posun[5]=true;
		case -2: posun[2]=true;
		case -1: posun[6]=true;
	}

}

void cOsnova::NastavPocet(int a)
{
	dvojite = a;
}

char* cOsnova::GenerujText()
{
	char vysledny[50], stup[5], dm[5], pom[5], pom2[4];
	switch (stupnice) {
		case -7: sprintf(stup,"Ces"); break;
		case -6: sprintf(stup,"Ges"); break;
		case -5: sprintf(stup,"Des"); break;
		case -4: sprintf(stup,"As"); break;
		case -3: sprintf(stup,"Es"); break;
		case -2: sprintf(stup,"B"); break;
		case -1: sprintf(stup,"F"); break;
		case 0:	sprintf(stup,"C"); break;
		case 1: sprintf(stup,"G"); break;
		case 2: sprintf(stup,"D"); break;
		case 3: sprintf(stup,"A"); break;
		case 4: sprintf(stup,"E"); break;
		case 5: sprintf(stup,"H"); break;
		case 6: sprintf(stup,"Fis"); break;
		case 7: sprintf(stup,"Cis"); break;
	}
	
	if (mol) sprintf(dm,"mol"); else sprintf(dm,"dur");
	
	switch (klic) {
		case 0: sprintf(pom,"_hou"); break;
		case 1: sprintf(pom,"_bas"); break;
		case 2: sprintf(pom,""); break;
	}
	
	if (dvojite) sprintf(pom2,"_d"); else sprintf(pom2,"");
	
	if (!predznam) sprintf(vysledny,"statistiky/stat_%s_%s%s%s_clr", stup, dm, pom, pom2);
	else sprintf(vysledny,"statistiky/stat_%s_%s%s%s", stup, dm, pom, pom2);
	
	return vysledny;
}
