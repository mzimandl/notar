class cPrepinac
{
	private:
		SDL_Rect rozmery;

		int min, max, x;
		
		int posx_txt, posy_txt;
		SDL_Surface *obrazek;	//Paměť se uvolní až v hlavním programu
		
		bool aktivni;
		
		bool udal;
		bool lklik, rklik;
		bool zmena;
				
		bool oobrazkovano;
		
		bool inicializovano;
	public:
		void Init(int x1, int y1, int x2, int y2, int nast_min, int nast_max, int nast_poc);
		void DeInit();
		
		void NastavText(const char* string, TTF_Font *p_font);
		
		void NastavRozmery(int x1, int y1, int x2, int y2);
		
		void Vykresli(SDL_Surface *plocha);

		bool Udalost(SDL_Event *stalose);
		
		void Aktivuj(bool a);
		bool ZmenaStavu();
		int Vybrano();
};

void cPrepinac::Init(int x1, int y1, int x2, int y2, int nast_min, int nast_max, int nast_poc)
{
	rozmery.x = x1;
	rozmery.y = y1;
	rozmery.w = x2;
	rozmery.h = y2;
	
	posx_txt = (rozmery.w - rozmery.x)/2;
	posy_txt = (rozmery.h - rozmery.y)/2;
	
	min = nast_min;
	max = nast_max;
	x = nast_poc;
	
	oobrazkovano = false;
	aktivni = true;
	zmena = false;
	inicializovano = true;
}

void cPrepinac::DeInit()
{
	SDL_FreeSurface(obrazek);
	inicializovano = false;
}

void cPrepinac::NastavRozmery(int x1, int y1, int x2, int y2)
{
	rozmery.x = x1;
	rozmery.y = y1;
	rozmery.w = x2;
	rozmery.h = y2;
	
	posx_txt = (rozmery.w - rozmery.x)/2;
	posy_txt = (rozmery.h - rozmery.y)/2;
}

bool cPrepinac::Udalost(SDL_Event *stalose)
{
	bool pomocny;
	
	if (aktivni) {
		if ((stalose->motion.x>rozmery.x) && (stalose->motion.x<rozmery.w) && (stalose->motion.y>rozmery.y) && (stalose->motion.y<rozmery.h))
			udal = true;
		else udal = false;
	
		pomocny = lklik;
		if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) lklik = true; else lklik = false;
		if (pomocny && !lklik && udal) {
			x++;
			zmena = true;
		} else zmena = false;
		
		pomocny = rklik;
		if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(3)) rklik = true; else rklik = false;
		if (pomocny && !rklik && udal) {
			x--;
			zmena = true;
		}
		
		if (x>max) x = min; else if (x<min) x = max;
		
		return udal;
	} else return false;
}

void cPrepinac::Vykresli(SDL_Surface *plocha)
{
	if (oobrazkovano) CenterBlitScreen(obrazek, plocha, rozmery.x + posx_txt, rozmery.y + posy_txt);
	
	if (aktivni)
		if (udal) if (lklik || rklik) rectangleRGBA(plocha,rozmery.x+2,rozmery.y+2,rozmery.w-2,rozmery.h-2,0,0,0,255);
			else rectangleRGBA(plocha,rozmery.x,rozmery.y,rozmery.w,rozmery.h,0,0,0,255);
		else rectangleRGBA(plocha,rozmery.x,rozmery.y,rozmery.w,rozmery.h,200,200,200,255);
}

void cPrepinac::Aktivuj(bool a)
{
	aktivni = a;
}

void cPrepinac::NastavText(const char* string, TTF_Font *p_font)
{
	SDL_FreeSurface(obrazek);
	obrazek = TTF_RenderUTF8_Shaded(p_font, string, {0,0,0}, {255,255,255});

	oobrazkovano = true;
}

bool cPrepinac::ZmenaStavu()
{
	return zmena;
}

int cPrepinac::Vybrano()
{
	return x;
}
