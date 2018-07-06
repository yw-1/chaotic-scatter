#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

// Midpoint circle drawing algorithm
void draw_circle(SDL_Renderer *renderer, int x0, int y0, int r)
{
	int x = r;
	int y = 0;
	int error = -r;

	while (x > y) {
		SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
		SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
		SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
		SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
		SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
		SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
		SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
		SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);

		error += 2*y + 1;
		if (error >= 0) {
			x--;
			error -= 2*x;
		}
		y++;
	}
}

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	int sw = 640, sh = 480;
	int end = 0, redraw = 1;
	int mx, my;

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Gaspard-Rice system", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			sw, sh, SDL_WINDOW_RESIZABLE);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	int x0 = sw/2, y0 = sh/2;
	int t;
	int r = 150, rr = 100;

	double x, y, xx, yy, x1, y1;
	double angle;

	while (!end) {
		if (redraw) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			for (double i=0; i<=2*M_PI; i+=2*M_PI/3) {
				draw_circle(renderer, r*cos(i) + sw/2, r*sin(i) + sh/2, rr);
			}		

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

			xx = x0;
			yy = y0;

			angle = atan2(my - y0, mx - x0);
			t = 0;

			x = xx;
			y = yy;

			while (x >= 0 && x < sw && y >= 0 && y < sh) {
				t++;
				x = t*cos(angle) + xx;
				y = t*sin(angle) + yy;

				for (double i=0; i<=2*M_PI; i+=2*M_PI/3) {
					x1 = r*cos(i) + sw/2;
					y1 = r*sin(i) + sh/2;

					if ((x - x1)*(x - x1) + (y - y1)*(y - y1) < rr*rr) {
						SDL_RenderDrawLine(renderer, xx, yy, x, y);

						angle = 2*atan2(y - y1, x - x1) - angle - M_PI;
						xx = x;
						yy = y;
						t = 0;
						break;
					}
				}
			}

			SDL_RenderDrawLine(renderer, xx, yy, x, y);

			SDL_RenderPresent(renderer);
			redraw = 0;
		}

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				end = 1;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					sw = event.window.data1;
					sh = event.window.data2;

					x0 = sw/2;
					y0 = sh/2;

					redraw = 1;
					break;
				default:
					SDL_RenderPresent(renderer);
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					end = 1;
					break;
				}
				break;

			case SDL_MOUSEMOTION:
				mx = event.motion.x;
				my = event.motion.y;

				redraw = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mx = event.button.x;
				my = event.button.y;

				int incircle = 0;

				for (double i=0; i<=2*M_PI; i+=2*M_PI/3) {
					x1 = r*cos(i) + sw/2;
					y1 = r*sin(i) + sh/2;

					if ((mx - x1)*(mx - x1) + (my - y1)*(my - y1) < rr*rr) incircle = 1;
				}

				if (!incircle) {
					x0 = mx;
					y0 = my;
				}

				redraw = 1;
				break;
			}

			SDL_FlushEvent(event.type);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

