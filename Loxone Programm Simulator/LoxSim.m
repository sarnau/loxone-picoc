//
//  LoxSim.m
//  Loxone Programm Simulator
//
//  Created by Markus Fritze on 16.03.18.
//  Copyright © 2018 Markus Fritze. All rights reserved.
//

#import "LoxSim.h"
#include "picoc.h"
#include "interpreter.h"
#import "LoxPrgDocument.h"
#define PICOC_STACK_SIZE (128*1024)              // space for the the stack

// mark where to end the program for platforms which require this
jmp_buf PicocExitBuf;

#ifndef NO_DEBUGGER
#include <signal.h>

Picoc *break_pc = NULL;

static void BreakHandler(int Signal)
{
    break_pc->DebugManualBreak = TRUE;
}

void PlatformInit(Picoc *pc)
{
    // capture the break signal and pass it to the debugger
    break_pc = pc;
    signal(SIGINT, BreakHandler);
}
#else
void PlatformInit(Picoc *pc)
{
}
#endif

void PlatformCleanup(Picoc *pc)
{
}

// get a line of interactive input
char *PlatformGetLine(char *Buf, int MaxLen, const char *Prompt)
{
    return NULL;    // not available in the Cocoa app
}

// write a character to the console
void PlatformPutc(unsigned char OutCh, union OutputStreamInfo *Stream)
{
    putchar(OutCh);
}

// read and scan a file for definitions
void PicocPlatformScanFile(Picoc *pc, const char *FileName)
{
    NSLog(@"#import \"%s\"", FileName);
}

// exit the program
void PlatformExit(Picoc *pc, int RetVal)
{
    pc->PicocExitValue = RetVal;
    longjmp(pc->PicocExitBuf, 1);
}

#pragma mark -

void LexSim_updateLine(struct ParseState *state)
{
    LoxPrgDocument *doc = (__bridge LoxPrgDocument *)(state->pc->owner);
    int currentLine = state->Line; 
    dispatch_async(dispatch_get_main_queue(), ^{
        __block NSRange selectRange = NSMakeRange(0, 0);
        __block NSInteger lineCount = 1;
        __block NSString *theLine = nil;
        [doc.sourceCodeView.string enumerateSubstringsInRange:NSMakeRange(0, doc.sourceCodeView.string.length) options:NSStringEnumerationByLines usingBlock:^(NSString * _Nullable substring, NSRange substringRange, NSRange enclosingRange, BOOL *stop) {
            selectRange.length = substringRange.length;
            if(lineCount++ == currentLine) {
                theLine = substring;
                *stop = YES;
                return;
            }
            selectRange.location += enclosingRange.length;
        }];
        [doc.sourceCodeView setSelectedRange:selectRange affinity:NSSelectionAffinityUpstream stillSelecting:YES];
//        [doc.sourceCodeView scrollRangeToVisible:selectRange];
    });
}

void LoxSim_Launch(Picoc *pc, NSString *appName, NSString *sourceCode)
{
    LoxPrgDocument *doc = (__bridge LoxPrgDocument *)(pc->owner);
    [NSThread detachNewThreadWithBlock:^{
        PicocInitialise(pc, PICOC_STACK_SIZE);
        PicocIncludeAllSystemHeaders(pc);
        if (!PicocPlatformSetExitPoint(pc))
        {
            PicocParse(pc, appName.UTF8String, sourceCode.UTF8String, (int)strlen(sourceCode.UTF8String), TRUE, FALSE, TRUE, TRUE);
        }
        PicocCleanup(pc);
        dispatch_sync(dispatch_get_main_queue(), ^{
            doc.runStopToolbarItem.image = [NSImage imageNamed:@"NSGoForwardTemplate"];
        });
    }];
}


#pragma mark -
#pragma mark I/O Callbacks

void LoxSim_setlogtext(struct ParseState *Parser, const char *text, BOOL isErrorLog)
{
    LoxPrgDocument *doc = (__bridge LoxPrgDocument *)(Parser->pc->owner);
    NSString *strObj = [NSString stringWithFormat:@"%@ : %4d : %s\n", NSDate.date, Parser->Line, text];
    dispatch_async(dispatch_get_main_queue(), ^{
        NSDictionary<NSAttributedStringKey, id> *attr = nil;
        if(isErrorLog)
            attr = @{ NSForegroundColorAttributeName:NSColor.redColor };
        [doc.logOutputView.textStorage appendAttributedString:[[NSAttributedString alloc] initWithString:strObj attributes:attr]];
        [doc.logOutputView scrollRangeToVisible:NSMakeRange(doc.logOutputView.string.length, 0)];
    });
}

float LoxSim_getio(struct ParseState *Parser, const char *name)
{
    LoxPrgDocument *doc = (__bridge LoxPrgDocument *)(Parser->pc->owner);
    size_t  slen = strlen(name);
    if(slen > 2 && name[0]=='V' && name[1]=='I') {
        int input = atoi(name+2);
        __block float ret = 0;
        dispatch_sync(dispatch_get_main_queue(), ^{
            switch(input) {
            case 1: ret = [doc.analogInput1.objectValue floatValue]; break;
            case 2: ret = [doc.analogInput2.objectValue floatValue]; break;
            case 3: ret = [doc.analogInput3.objectValue floatValue]; break;
            case 4: ret = [doc.analogInput4.objectValue floatValue]; break;
            case 5: ret = [doc.analogInput5.objectValue floatValue]; break;
            case 6: ret = [doc.analogInput6.objectValue floatValue]; break;
            case 7: ret = [doc.analogInput7.objectValue floatValue]; break;
            case 8: ret = [doc.analogInput8.objectValue floatValue]; break;
            case 9: ret = [doc.analogInput9.objectValue floatValue]; break;
            case 10: ret = [doc.analogInput10.objectValue floatValue]; break;
            case 11: ret = [doc.analogInput11.objectValue floatValue]; break;
            case 12: ret = [doc.analogInput12.objectValue floatValue]; break;
            case 13: ret = [doc.analogInput13.objectValue floatValue]; break;
            }
        });
    }
    return 0.0;
}

int LoxSim_setio(struct ParseState *Parser, const char *name, float value)
{
    LoxPrgDocument *doc = (__bridge LoxPrgDocument *)(Parser->pc->owner);
    size_t  slen = strlen(name);
    if(slen > 2 && name[0]=='V' && name[1]=='I') {
        int input = atoi(name+2);
        dispatch_sync(dispatch_get_main_queue(), ^{
            switch(input) {
            case 1: doc.analogInput1.objectValue = @(value); break;
            case 2: doc.analogInput2.objectValue = @(value); break;
            case 3: doc.analogInput3.objectValue = @(value); break;
            case 4: doc.analogInput4.objectValue = @(value); break;
            case 5: doc.analogInput5.objectValue = @(value); break;
            case 6: doc.analogInput6.objectValue = @(value); break;
            case 7: doc.analogInput7.objectValue = @(value); break;
            case 8: doc.analogInput8.objectValue = @(value); break;
            case 9: doc.analogInput9.objectValue = @(value); break;
            case 10: doc.analogInput10.objectValue = @(value); break;
            case 11: doc.analogInput11.objectValue = @(value); break;
            case 12: doc.analogInput12.objectValue = @(value); break;
            }
        });
    }
    return LoxSim_getio(Parser, name);
}

void LoxSim_triggerInputEvent(Picoc *pc, int mask)
{
    pc->eventMask |= mask;
}

int LoxSim_getinputevent(struct ParseState *Parser)
{
    __block int mask;
    dispatch_sync(dispatch_get_main_queue(), ^{
        mask = Parser->pc->eventMask;
        Parser->pc->eventMask = 0;
    });
    return mask;
}

float LoxSim_getinput(struct ParseState *Parser, int input)
{
    LoxPrgDocument *doc = (__bridge LoxPrgDocument *)(Parser->pc->owner);
    __block float ret = 0;
    dispatch_sync(dispatch_get_main_queue(), ^{
        switch(input) {
        case 0: ret = [doc.analogInput1.objectValue floatValue]; break;
        case 1: ret = [doc.analogInput2.objectValue floatValue]; break;
        case 2: ret = [doc.analogInput3.objectValue floatValue]; break;
        case 3: ret = [doc.analogInput4.objectValue floatValue]; break;
        case 4: ret = [doc.analogInput5.objectValue floatValue]; break;
        case 5: ret = [doc.analogInput6.objectValue floatValue]; break;
        case 6: ret = [doc.analogInput7.objectValue floatValue]; break;
        case 7: ret = [doc.analogInput8.objectValue floatValue]; break;
        case 8: ret = [doc.analogInput9.objectValue floatValue]; break;
        case 9: ret = [doc.analogInput10.objectValue floatValue]; break;
        case 10: ret = [doc.analogInput11.objectValue floatValue]; break;
        case 11: ret = [doc.analogInput12.objectValue floatValue]; break;
        case 12: ret = [doc.analogInput13.objectValue floatValue]; break;
        }
    });
    return ret;
}

const char *LoxSim_getinputtext(struct ParseState *Parser, int input)
{
    LoxPrgDocument *doc = (__bridge LoxPrgDocument *)(Parser->pc->owner);
    __block const char *retStr = "";
    dispatch_sync(dispatch_get_main_queue(), ^{
        switch(input) {
        case 0: retStr = [doc.textInput1.objectValue UTF8String]; break;
        case 1: retStr = [doc.textInput2.objectValue UTF8String]; break;
        case 2: retStr = [doc.textInput3.objectValue UTF8String]; break;
        }
    });
    return retStr;
}

void LoxSim_setoutput(struct ParseState *Parser, int output, float value)
{
    LoxPrgDocument *doc = (__bridge LoxPrgDocument *)(Parser->pc->owner);
    dispatch_sync(dispatch_get_main_queue(), ^{
        switch(output) {
        case 0: doc.analogOutput1.objectValue = @(value); break;
        case 1: doc.analogOutput2.objectValue = @(value); break;
        case 2: doc.analogOutput3.objectValue = @(value); break;
        case 3: doc.analogOutput4.objectValue = @(value); break;
        case 4: doc.analogOutput5.objectValue = @(value); break;
        case 5: doc.analogOutput6.objectValue = @(value); break;
        case 6: doc.analogOutput7.objectValue = @(value); break;
        case 7: doc.analogOutput8.objectValue = @(value); break;
        case 8: doc.analogOutput9.objectValue = @(value); break;
        case 9: doc.analogOutput10.objectValue = @(value); break;
        case 10: doc.analogOutput11.objectValue = @(value); break;
        case 11: doc.analogOutput12.objectValue = @(value); break;
        case 12: doc.analogOutput13.objectValue = @(value); break;
        }
    });
}

void LoxSim_setoutputtext(struct ParseState *Parser, int output, const char *str)
{
    LoxPrgDocument *doc = (__bridge LoxPrgDocument *)(Parser->pc->owner);
    NSString *strObj = @(str);
    dispatch_sync(dispatch_get_main_queue(), ^{
        switch(output) {
        case 0: doc.textOutput1.objectValue = strObj; break;
        case 1: doc.textOutput2.objectValue = strObj; break;
        case 2: doc.textOutput3.objectValue = strObj; break;
        }
    });
}

