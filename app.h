#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include "circuit.h"

#define MAX_UI_GATES MAX_GATES

typedef struct {
    int gateIndex;
    int x;
    int y;
} GateUI;

typedef struct {
    GateType type;
    SDL_Rect iconRect;
} GateIcon;

typedef struct {
    SDL_Texture *andGate;
    SDL_Texture *orGate;
    SDL_Texture *notGate;
    SDL_Texture *nandGate;
    SDL_Texture *norGate;
    SDL_Texture *xorGate;
    SDL_Texture *highGate;
    SDL_Texture *lowGate;
    SDL_Texture *output;
    SDL_Texture *outputOn;
    SDL_Texture *outputOff;
    SDL_Texture *switchGate;
    SDL_Texture *switchOn;
    SDL_Texture *switchOff;
    SDL_Texture *halfAdder;
    SDL_Texture *fullAdder;
    SDL_Texture *drawWires;
    SDL_Texture *drawWiresGlow;
    SDL_Texture *trash;
} Textures;

typedef struct {
    Circuit circuit;

    GateUI uiGates[MAX_UI_GATES];
    int uiGateCount;

    int gateWidth;
    int gateHeight;

    int isDraggingGate;
    int draggedGateIndex;
    int dragOffsetX;
    int dragOffsetY;

    int wireMode;
    int wireStartSignal;

    Textures textures;
    SDL_Rect trashRect;
    SDL_Rect wireButtonRect;
} App;

#endif