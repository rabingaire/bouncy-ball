#include <stdio.h>
#include "SDL.h"
#include "SDL_Image.h"
#include "BouncyBall.h"
#include "math.h"

static const int nBalls = 10;
static struct BouncyBall  balls[nBalls];
static int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480;
static int BALL_SIZE = 32;

void doRender(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 240, 240, 255,255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  for(int i = 0; i < nBalls; i++)
  {
    draw(&balls[i], renderer, BALL_SIZE);
  }
  SDL_RenderPresent(renderer);
}

int main()
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  SDL_Event event;

  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow(
      "Test",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_OPENGL
  );

  if(window == NULL)
  {
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Surface *image;

  image = IMG_Load("../ball.png");
  if(!image)
  {
    printf("IMG_Load: %s\n", IMG_GetError());
    return 1;
  }
  texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  for(int i = 0; i < nBalls; i++)
  {
    init(&balls[i]);
    setTexture(&balls[i], texture);
    setPosition(&balls[i], 50 + i * BALL_SIZE, 100);
    setElasticity(&balls[i], (float)i/nBalls);
  }

  while(1)
  {
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT)
    {
      break;
    }

    for(int i = 0; i < nBalls; i++)
    {
      update(&balls[i], WINDOW_HEIGHT, BALL_SIZE);
    }

    doRender(renderer);
    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(texture);
  SDL_Quit();
  return 0;
}