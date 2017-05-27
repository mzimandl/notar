#define st_vyp 0
#define st_zap 1

class cVypinac
{
	private:
		SDL_Rect rozmery;

		int posx_txt, posy_txt;
		SDL_Surface *obrazek[2];	//Paměť se uvolní až v hlavním programu
		
		bool zapnuto;
		bool aktivni;
		
		bool udal;
		bool klik;
		bool zmena;
				
		bool oobrazkovano;
		
		bool inicializovano;
	public:
		void Init(int x1, int y1, int x2, int y2, bool poc);
		void DeInit();
		
		void NastavObrazky(SDL_Surface *sf_vyp, SDL_Surface *sf_zap);
		
		void NastavRozmery(int x1, int y1, int x2, int y2);
		
		void Vykresli(SDL_Surface *plocha);

		bool Udalost(SDL_Event *stalose);
		
		void Aktivuj(bool a);
		bool Zapnuto();
		bool ZmenaStavu();
};

void cVypinac::Init(int x1, int y1, int x2, int y2, bool poc)
{
	rozmery.x = x1;
	rozmery.y = y1;
	rozmery.w = x2;
	rozmery.h = y2;
	
	posx_txt = (rozmery.w - rozmery.x)/2;
	posy_txt = (rozmery.h - rozmery.y)/2;
	
	zapnuto = poc;
	
	oobrazkovano = false;
	aktivni = true;
	zmena = false;
	inicializovano = true;
}

void cVypinac::DeInit()
{
	inicializovano = false;
}

void cVypinac::NastavRozmery(int x1, int y1, int x2, int y2)
{
	rozmery.x = x1;
	rozmery.y = y1;
	rozmery.w = x2;
	rozmery.h = y2;
	
	posx_txt = (rozmery.w - rozmery.x)/2;
	posy_txt = (rozmery.h - rozmery.y)/2;
}

bool cVypinac::Udalost(SDL_Event *stalose)
{
	bool pomocny;
	
	if (aktivni) {
		if ((stalose->motion.x>rozmery.x) && (stalose->motion.x<rozmery.w) && (stalose->motion.y>rozmery.y) && (stalose->motion.y<rozmery.h))
			udal = true;
		else udal = false;
	
		pomocny = klik;
		if ((SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) || (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(3))) klik = true; else klik = false;
	
		if (pomocny && !klik && udal) {
			zapnuto = !zapnuto; //odkliknuti
			zmena = true;
		} else zmena = false;
	
		return udal;
	} else return false;
}

void cVypinac::Vykresli(SDL_Surface *plocha)
{
	if (zapnuto) {
		if (oobrazkovano) CenterBlitScreen(obrazek[st_zap], plocha, rozmery.x + posx_txt, rozmery.y + posy_txt);
	} else {
		if (oobrazkovano) CenterBlitScreen(obrazek[st_vyp], plocha, rozmery.x + posx_txt, rozmery.y + posy_txt);
	}
	
	if (aktivni)
		if (udal) if (klik) rectangleRGBA(plocha,rozmery.x+2,rozmery.y+2,rozmery.w-2,rozmery.h-2,0,0,0,255);
			else rectangleRGBA(plocha,rozmery.x,rozmery.y,rozmery.w,rozmery.h,0,0,0,255);
		else rectangleRGBA(plocha,rozmery.x,rozmery.y,rozmery.w,rozmery.h,200,200,200,255);
}

bool cVypinac::Zapnuto()
{
	return zapnuto;
}

void cVypinac::Aktivuj(bool a)
{
	aktivni = a;
}

void cVypinac::NastavObrazky(SDL_Surface *sf_vyp, SDL_Surface *sf_zap)
{
	obrazek[st_zap] = sf_zap;
	obrazek[st_vyp] = sf_vyp;

	oobrazkovano = true;
}

bool cVypinac::ZmenaStavu()
{
	return zmena;
}
