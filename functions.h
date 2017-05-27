SDL_Surface *LoadIMG(std::string soubor)
{
	SDL_Surface *loaded = NULL;
	SDL_Surface *enhanced = NULL;
	
	loaded = IMG_Load(soubor.c_str());
	
	if (loaded!=NULL)
	{
		enhanced = SDL_DisplayFormatAlpha(loaded);
		SDL_FreeSurface(loaded);
	}
	
	return enhanced;
}

void BlitScreen(SDL_Surface *source, SDL_Surface *cil, int x, int y)
{
	SDL_Rect rectangle;
	
	rectangle.x = x;
	rectangle.y = y;
	
	SDL_BlitSurface(source,NULL,cil,&rectangle);
}

void CenterBlitScreen(SDL_Surface *source, SDL_Surface *cil, int x, int y)
{
	SDL_Rect rectangle;
	
	rectangle.x = x - source->w / 2;
	rectangle.y = y - source->h / 2;
	
	SDL_BlitSurface(source,NULL,cil,&rectangle);
}

void BlitScreenRect(SDL_Surface *zdroj, SDL_Rect ctverec1, SDL_Surface *cil, int x2, int y2)
{
	SDL_Rect ctverec2;
	
	ctverec2.x = x2;
	ctverec2.y = y2;
	
	SDL_BlitSurface(zdroj,&ctverec1,cil,&ctverec2);
}
