/* stdlib.h library for large systems - small embedded systems use clibrary.c instead */
#include "../interpreter.h"
#ifdef LOXONE
    #include "LoxSim.h"
#endif

#ifndef BUILTIN_MINI_STDLIB

static int Stdlib_ZeroValue = 0;

#ifndef NO_FP
static void StdlibAtof(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->FP = atof((char *) Param[0]->Val->Pointer);
}
#endif

static void StdlibAtoi(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = atoi((const char *) Param[0]->Val->Pointer);
}

#ifndef LOXONE
static void StdlibAtol(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int)atol((char *) Param[0]->Val->Pointer);
}

#ifndef NO_FP
static void StdlibStrtod(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->FP = strtod((char *) Param[0]->Val->Pointer, (char **) Param[1]->Val->Pointer);
}
#endif

static void StdlibStrtol(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int)strtol((char *) Param[0]->Val->Pointer, (char **) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

static void StdlibStrtoul(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int)strtoul((char *) Param[0]->Val->Pointer, (char **) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}
#endif

static void StdlibMalloc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = malloc(Param[0]->Val->Integer);
#ifdef LOXONE
    LoxSim_addPointer(Parser, ReturnValue->Val->Pointer);
#endif
}

static void StdlibCalloc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = calloc(Param[0]->Val->Integer, Param[1]->Val->Integer);
#ifdef LOXONE
    LoxSim_addPointer(Parser, ReturnValue->Val->Pointer);
#endif
}

static void StdlibRealloc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = realloc(Param[0]->Val->Pointer, Param[1]->Val->Integer);
#ifdef LOXONE
    LoxSim_removePointer(Parser, Param[0]->Val->Pointer);
    LoxSim_addPointer(Parser, ReturnValue->Val->Pointer);
#endif
}

static void StdlibFree(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(ReturnValue); UNUSED(NumArgs);     
    free(Param[0]->Val->Pointer);
#ifdef LOXONE
    LoxSim_removePointer(Parser, Param[0]->Val->Pointer);
#endif
}

#ifndef LOXONE
static void StdlibRand(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(Param); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = rand();
}

static void StdlibSrand(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(ReturnValue); UNUSED(NumArgs);
    srand(Param[0]->Val->Integer);
}

static void StdlibAbort(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Param); UNUSED(ReturnValue); UNUSED(NumArgs);
    ProgramFail(Parser, "abort");
}

static void StdlibExit(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(ReturnValue); UNUSED(NumArgs);
    PlatformExit(Parser->pc, Param[0]->Val->Integer);
}

static void StdlibGetenv(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs);
    ReturnValue->Val->Pointer = getenv((const char *) Param[0]->Val->Pointer);
}

static void StdlibSystem(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs);
    ReturnValue->Val->Integer = system((const char *) Param[0]->Val->Pointer);
}

#if 0
static void StdlibBsearch(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs);
    ReturnValue->Val->Pointer = bsearch(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer, Param[3]->Val->Integer, (int (*)())Param[4]->Val->Pointer);
}
#endif

static void StdlibAbs(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs);
    ReturnValue->Val->Integer = abs(Param[0]->Val->Integer);
}

static void StdlibLabs(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs);
    ReturnValue->Val->Integer = (int)labs(Param[0]->Val->Integer);
}

#if 0
static void StdlibDiv(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs);
    ReturnValue->Val->Integer = div(Param[0]->Val->Integer, Param[1]->Val->Integer);
}

static void StdlibLdiv(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs);
    ReturnValue->Val->Integer = ldiv(Param[0]->Val->Integer, Param[1]->Val->Integer);
}
#endif
#endif

#if 0
/* handy structure definitions */
const char StdlibDefs[] = "\
typedef struct { \
    int quot, rem; \
} div_t; \
\
typedef struct { \
    int quot, rem; \
} ldiv_t; \
";
#endif

/* all stdlib.h functions */
struct LibraryFunction StdlibFunctions[] =
{
#ifndef NO_FP
    { StdlibAtof,           "float atof(char *);" },
#ifndef LOXONE
    { StdlibStrtod,         "float strtod(char *,char **);" },
#endif
#endif
    { StdlibAtoi,           "int atoi(char *);" },
#ifndef LOXONE
    { StdlibAtol,           "int atol(char *);" },
    { StdlibStrtol,         "int strtol(char *,char **,int);" },
    { StdlibStrtoul,        "int strtoul(char *,char **,int);" },
#endif
    { StdlibMalloc,         "void *malloc(int);" },
    { StdlibCalloc,         "void *calloc(int,int);" },
    { StdlibRealloc,        "void *realloc(void *,int);" },
    { StdlibFree,           "void free(void *);" },
#ifndef LOXONE
    { StdlibRand,           "int rand();" },
    { StdlibSrand,          "void srand(int);" },
    { StdlibAbort,          "void abort();" },
#endif
#ifndef LOXONE
    { StdlibExit,           "void exit(int);" },
    { StdlibGetenv,         "char *getenv(char *);" },
    { StdlibSystem,         "int system(char *);" },
/*    { StdlibBsearch,        "void *bsearch(void *,void *,int,int,int (*)());" }, */
/*    { StdlibQsort,          "void *qsort(void *,int,int,int (*)());" }, */
    { StdlibAbs,            "int abs(int);" },
    { StdlibLabs,           "int labs(int);" },
#if 0
    { StdlibDiv,            "div_t div(int);" },
    { StdlibLdiv,           "ldiv_t ldiv(int);" },
#endif
#endif
    { NULL,                 NULL }
};

/* creates various system-dependent definitions */
void StdlibSetupFunc(Picoc *pc)
{
    /* define NULL, TRUE and FALSE */
    if (!VariableDefined(pc, TableStrRegister(pc, "NULL")))
        VariableDefinePlatformVar(pc, NULL, "NULL", &pc->IntType, (union AnyValue *)&Stdlib_ZeroValue, FALSE);
}

#endif /* !BUILTIN_MINI_STDLIB */
