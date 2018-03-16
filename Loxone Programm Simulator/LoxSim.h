//
//  LoxSim.h
//  Loxone Programm Simulator
//
//  Created by Markus Fritze on 16.03.18.
//  Copyright © 2018 Markus Fritze. All rights reserved.
//

#include "interpreter.h"
#import <Foundation/Foundation.h>

void LoxSim_updateLine(struct ParseState *state);
void LoxSim_addPointer(struct ParseState *state, void *pointer);
void LoxSim_removePointer(struct ParseState *state, void *pointer);
void LoxSim_checkLeaks(struct ParseState *state);

void LoxSim_Launch(Picoc *pcPtr, NSString *appName, NSString *sourceCode);

void LoxSim_setlogtext(struct ParseState *state, const char *text, BOOL isErrorLog);
float LoxSim_getio(struct ParseState *state, const char *name);
int LoxSim_setio(struct ParseState *state, const char *name, float value);
int LoxSim_getinputevent(struct ParseState *state);
float LoxSim_getinput(struct ParseState *state, int input);
const char *LoxSim_getinputtext(struct ParseState *state, int input);
void LoxSim_setoutput(struct ParseState *state, int output, float value);
void LoxSim_setoutputtext(struct ParseState *state, int output, const char *str);

void LoxSim_triggerInputEvent(Picoc *pc, int mask);
