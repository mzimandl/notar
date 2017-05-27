#define st_nic 0
#define st_nad 1
#define st_klik 2

class cTlacitko
{
	private:
		SDL_Rect rozmery;

		int posx_txt, posy_txt;
		SDL_Surface *text[3];
		SDL_Surface *obrazek;	//Paměť se uvolní až v hlavním programu
		
		SDL_Color c_stav[3];
		SDL_Color c_text;
		
		TTF_Font *tl_font;	//Paměť se uvolní až v hlavním programu
		
		bool udal;
		bool klik;
		bool odklik;
		bool lodklik;
		bool inicializovano;
		
		bool okraj;
		bool popsano;
		bool oobrazkovano;
		bool znacit;
		bool leve;
	public:
		void Init(int x1, int y1, int x2, int y2);
		void DeInit();
		
		void NastavRozmery(int x1, int y1, int x2, int y2);
		void NastavBarvuNic(int r, int g, int b);
		void NastavBarvuNad(int r, int g, int b);
		void NastavBarvuKlik(int r, int g, int b);
		
		void NastavPopis(int xrel, int yrel, int r, int g, int b, const char *txt, TTF_Font *font);
		void NastavPopisCenterBlack(const char *txt, TTF_Font *font);
		void ZmenPopis(const char *txt);
		
		void SkrytOznaceni(bool a);
		
		void NastavObrazek(SDL_Surface *plocha);
		
		void KresliBezOkraje();
				
		void Vykresli(SDL_Surface *plocha);

		bool Udalost(SDL_Event *stalose);
		void ZrusUdalost();
		
		bool Stisknuto();
		bool Odstisknuto();
		bool LeveOdstisknuto();
		bool LeveTlacitko();
};

void cTlacitko::Init(int x1, int y1, int x2, int y2)
{
	c_stav[st_nic] = {0,255,0};
	c_stav[st_nad] = {0,0,255};
	c_stav[st_klik] = {255,0,0};
	
	rozmery.x = x1;
	rozmery.y = y1;
	rozmery.w = x2;
	rozmery.h = y2;
	
	odklik = false;
	
	okraj = true;
	popsano = false;
	oobrazkovano = false;
	znacit = true;
	
	inicializovano = true;
}

void cTlacitko::DeInit()
{
	int i;
	
	for (i=0;i<3;i++) SDL_FreeSurface(text[i]);
	
	inicializovano = false;
}

void cTlacitko::NastavRozmery(int x1, int y1, int x2, int y2)
{
	rozmery.x = x1;
	rozmery.y = y1;
	rozmery.w = x2;
	rozmery.h = y2;
}

bool cTlacitko::Udalost(SDL_Event *stalose)
{
	bool pomocny, pomocny2;
	
	if ((stalose->motion.x>rozmery.x) && (stalose->motion.x<rozmery.w) && (stalose->motion.y>rozmery.y) && (stalose->motion.y<rozmery.h))
		udal = true;
	else udal = false;
	
	pomocny = klik;
	pomocny2 = leve;
	
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1)) {
		leve = true;
		klik = true;
	} else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3)) {
		leve = false;
		klik = true;
	} else {
		klik = false;
		leve = false;
	}
	
	if (pomocny && !klik && udal) {
		if (pomocny2 && !leve) lodklik = true; else lodklik = false;
		odklik = true;
	} else {
		odklik = false;
		lodklik = false;
	}
	
	return udal;
}

void cTlacitko::Vykresli(SDL_Surface *plocha)
{
	if (udal) {
		if (klik) {
			boxRGBA(plocha,rozmery.x,rozmery.y,rozmery.w,rozmery.h,c_stav[st_klik].r,c_stav[st_klik].g,c_stav[st_klik].b,255);
			if (popsano && znacit) CenterBlitScreen(text[st_klik], plocha, rozmery.x + posx_txt, rozmery.y + posy_txt);
		} else {
			boxRGBA(plocha,rozmery.x,rozmery.y,rozmery.w,rozmery.h,c_stav[st_nad].r,c_stav[st_nad].g,c_stav[st_nad].b,255);
			if (popsano && znacit) CenterBlitScreen(text[st_nad], plocha, rozmery.x + posx_txt, rozmery.y + posy_txt);
		}
	} else {
		boxRGBA(plocha,rozmery.x,rozmery.y,rozmery.w,rozmery.h,c_stav[st_nic].r,c_stav[st_nic].g,c_stav[st_nic].b,255);
		if (popsano && znacit) CenterBlitScreen(text[st_nic], plocha, rozmery.x + posx_txt, rozmery.y + posy_txt);
	}
	
	if (oobrazkovano && znacit) CenterBlitScreen(obrazek, plocha, rozmery.x + posx_txt, rozmery.y + posy_txt);
	if (okraj) rectangleRGBA(plocha,rozmery.x,rozmery.y,rozmery.w,rozmery.h,0,0,0,255);
}

bool cTlacitko::Stisknuto()
{
	if ((udal) && (klik)) return true;
	else return false;
}

bool cTlacitko::Odstisknuto()
{
	return odklik;
}

void cTlacitko::ZrusUdalost()
{
	udal = false;
}

void cTlacitko::NastavBarvuNic(int r, int g, int b)
{
	c_stav[st_nic] = {r,g,b};
}

void cTlacitko::NastavBarvuNad(int r, int g, int b)
{
	c_stav[st_nad] = {r,g,b};
}

void cTlacitko::NastavBarvuKlik(int r, int g, int b)
{
	c_stav[st_klik] = {r,g,b};
}

void cTlacitko::KresliBezOkraje()
{
	okraj = false;
}

void cTlacitko::NastavPopis(int xrel, int yrel, int r, int g, int b, const char *txt, TTF_Font *font)
{
	int i;
	
	posx_txt = xrel;
	posy_txt = yrel;
	c_text = {r,g,b};
	tl_font = font;
	
	for (i=0;i<3;i++) {
		SDL_FreeSurface(text[i]);
		text[i] = TTF_RenderUTF8_Shaded(font, txt, c_text, c_stav[i]);
	}
	
	popsano = true;
	oobrazkovano = false;
}

void cTlacitko::NastavPopisCenterBlack(const char *txt, TTF_Font *font)
{
	NastavPopis((rozmery.w - rozmery.x)/2, (rozmery.h - rozmery.y)/2, 0, 0, 0, txt, font);
}

void cTlacitko::ZmenPopis(const char *txt)
{
	int i;
	
	if (popsano) for (i=0;i<3;i++) {
		SDL_FreeSurface(text[i]);
		text[i] = TTF_RenderUTF8_Shaded(tl_font, txt, c_text, c_stav[i]);
	}
}

void cTlacitko::NastavObrazek(SDL_Surface *plocha)
{
	obrazek = plocha;
	
	posx_txt = (rozmery.w - rozmery.x)/2;
	posy_txt = (rozmery.h - rozmery.y)/2;
	
	oobrazkovano = true;
	popsano = false;
}

void cTlacitko::SkrytOznaceni(bool a)
{
	znacit = !a;
}

bool cTlacitko::LeveTlacitko()
{
	return leve;
}

bool cTlacitko::LeveOdstisknuto()
{
	return lodklik;
}
