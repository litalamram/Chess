#include "UI_Widget.h"

/** Creates a new widget with the given parameters specified
 *
 * @param parentWindow - the window contains the widget
 * @param loction - the location of the widget in the screen
 * @param draw - a pointer to function for drawing the widget
 * @param handleEvent - a pointer to function for handling events
 * @param destroy - a pointer to function destroys the widget
 * @param data - additional data the widget should have
 *
 * @return
 * NULL ,if an allocation error occured,
 * A new instance of the widget ,otherwise
 * */
Widget* createWidget(
		Widget *parentWindow,
		SDL_Rect location,
		int (*draw)(Widget*, SDL_Renderer*),
		void (*handleEvent)(Widget*, SDL_Event*),
		void (*destroy)(Widget*),
		void* data){
	// allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	if (res == NULL){
		return NULL;
	}

	// store widget details
	res->destroy = destroy;
	res->draw = draw;
	res->handleEvent = handleEvent;
	res->data = data;
	res->srcRect = (SDL_Rect){.x = 0, .y = 0, .w = location.w, .h = location.h};
	res->location = location;
	res->originalLocation = location;
	res->next = NULL;
	res->parent = NULL;
	res->parentWindow = parentWindow;
	return res;
}

/** Destroys the given widget src
 *  in case src == NULL, does nothing
 *
 *  @param src- the widget src
 *
 * */
void destroyWidget(Widget* src) {
	if (src != NULL)
		src->destroy(src);
}

/** Update the location of the widget in the screen
 *  according to its parent's boundaries
 *
 *  @param src - the widget src
 * */
void updateLocation(Widget *src){

	Widget *parent = src->parent;
	SDL_Rect *parentLoc = &parent->location;
	SDL_Rect *childLoc = &src->location;

	//sets child position according to his parent's position
	childLoc->x = (childLoc->x > parentLoc->x) ? childLoc->x : parentLoc->x;
	childLoc->y = (childLoc->y > parentLoc->y) ? childLoc->y : parentLoc->y;

	//sets child width according to his parent's boundaries
	int w1 = src->originalLocation.x + src->originalLocation.w - childLoc->x;
	int w2 = parentLoc->x + parentLoc->w - childLoc->x;
	childLoc->w = (w1 < w2) ? w1 : w2;
	if (childLoc->w < 0)
		childLoc->w = 0;

	//sets child height according to his parent's boundaries
	int h1 = src->originalLocation.y + src->originalLocation.h - childLoc->y;
	int h2 = parentLoc->y + parentLoc->h - childLoc->y;
	childLoc->h = (h1 < h2) ? h1 : h2;
	if (childLoc->h < 0)
		childLoc->h = 0;
}

/** Update which part of the widget should be drawn to the screen
 *  according to its location and parent's boundaries
 *
 *  @param src - the widget src
 *  @param w - the original width of the widget
 *  @param h - the original height of the widget
 * */
void updateSrcRect (Widget *src, int w, int h){
	SDL_Rect *srcRect = &src->srcRect;

	//sets the width of the widget should be drawn
	srcRect->w = ((float)src->location.w/src->originalLocation.w)*w;

	//sets the x position to start draw from
	if ((src->location.x) != (src->originalLocation.x)){
		srcRect->x = w - srcRect->w;
	}
	else {
		srcRect->x = 0;
	}

	//sets the width of the height should be drawn
	srcRect->h = ((float)src->location.h/src->originalLocation.h)*h;

	//sets the y position to start draw from
	if ((src->location.y) != (src->originalLocation.y)){
		srcRect->y = h - srcRect->h;
	}
	else {
		srcRect->y = 0;
	}
}

/** Creates a texture from the given image path
 *
 * @src renderer - the renderer
 * @src img - a path to bmp image
 *
 * @return
 * NULL, if an error occurred
 * The texture, otherwise
 * */
SDL_Texture* getTexture (SDL_Renderer* renderer, const char* image){
	// we use the surface as a temp var
	SDL_Surface* surface = SDL_LoadBMP(image);
	if (surface == NULL) {
		return NULL;
	}
	// set a specific color (red: 255,0,0) to transparent
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 0));

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {
		SDL_FreeSurface(surface);
		return NULL ;
	}

	// surface is not needed after texture is created
	SDL_FreeSurface(surface);

	return texture;

}

/**Returns if the the point (x,y) is inside the specified rectangle*/
bool isInRect (int x, int y, SDL_Rect r){
	return ((x > r.x) && (y > r.y) && (x < r.x+r.w) && (y < r.y+r.h));

}
