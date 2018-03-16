/* string.h library for large systems - small embedded systems use clibrary.c instead */
#include "../interpreter.h"
#ifdef LOXONE
    #include "LoxSim.h"
#endif

#ifndef BUILTIN_MINI_STDLIB

static int String_ZeroValue = 0;

static void StringStrcpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strcpy((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

static void StringStrncpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strncpy((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

static void StringStrcmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = strcmp((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

static void StringStrncmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = strncmp((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

static void StringStrcat(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strcat((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

static void StringStrncat(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strncat((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

static void StringStrlen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int) strlen((char *) Param[0]->Val->Pointer);
}

static void StringMemset(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = memset(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

static void StringMemcpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = memcpy(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

static void StringMemcmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = memcmp(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

static void StringMemmove(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = memmove(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

static void StringMemchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = memchr(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

static void StringStrchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strchr((char *) Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

static void StringStrrchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strrchr((char *) Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

static void StringStrcoll(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = strcoll((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

static void StringStrerror(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strerror(Param[0]->Val->Integer);
}

static void StringStrspn(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int) strspn((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

static void StringStrcspn(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int) strcspn((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

static void StringStrpbrk(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strpbrk((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

static void StringStrstr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strstr((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

static void StringStrtok(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strtok((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

#ifndef LOXONE
static void StringStrxfrm(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int) strxfrm((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}
#endif

static void StringStrdup(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
#ifndef _WIN32
    ReturnValue->Val->Pointer = strdup((char *) Param[0]->Val->Pointer);
#else
	ReturnValue->Val->Pointer = _strdup((char *)Param[0]->Val->Pointer);
#endif
#ifdef LOXONE
    LoxSim_addPointer(Parser, ReturnValue->Val->Pointer);
#endif
}

#ifndef LOXONE
#ifndef WIN32
static void StringStrtok_r(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strtok_r((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Pointer);
}
#endif
#endif

/* all string.h functions */
struct LibraryFunction StringFunctions[] =
{
	{ StringStrchr,         "char *index(char *,int);" },
    { StringStrrchr,        "char *rindex(char *,int);" },
    { StringMemcpy,        "void *memcpy(void *,void *,int);" },
    { StringMemmove,       "void *memmove(void *,void *,int);" },
    { StringMemchr,        "void *memchr(char *,int,int);" },
    { StringMemcmp,        "int memcmp(void *,void *,int);" },
    { StringMemset,        "void *memset(void *,int,int);" },
    { StringStrcat,        "char *strcat(char *,char *);" },
    { StringStrncat,       "char *strncat(char *,char *,int);" },
    { StringStrchr,        "char *strchr(char *,int);" },
    { StringStrrchr,       "char *strrchr(char *,int);" },
    { StringStrcmp,        "int strcmp(char *,char *);" },
    { StringStrncmp,       "int strncmp(char *,char *,int);" },
    { StringStrcoll,       "int strcoll(char *,char *);" },
    { StringStrcpy,        "char *strcpy(char *,char *);" },
    { StringStrncpy,       "char *strncpy(char *,char *,int);" },
    { StringStrerror,      "char *strerror(int);" },
    { StringStrlen,        "int strlen(char *);" },
    { StringStrspn,        "int strspn(char *,char *);" },
    { StringStrcspn,       "int strcspn(char *,char *);" },
    { StringStrpbrk,       "char *strpbrk(char *,char *);" },
    { StringStrstr,        "char *strstr(char *,char *);" },
    { StringStrtok,        "char *strtok(char *,char *);" },
#ifndef LOXONE
    { StringStrxfrm,       "int strxfrm(char *,char *,int);" },
#endif
	{ StringStrdup,        "char *strdup(char *);" },
#ifndef LOXONE
#ifndef WIN32
    { StringStrtok_r,      "char *strtok_r(char *,char *,char **);" },
#endif
#endif
    { NULL,             NULL }
};

/* creates various system-dependent definitions */
void StringSetupFunc(Picoc *pc)
{
    /* define NULL */
    if (!VariableDefined(pc, TableStrRegister(pc, "NULL")))
        VariableDefinePlatformVar(pc, NULL, "NULL", &pc->IntType, (union AnyValue *)&String_ZeroValue, FALSE);
}

#endif /* !BUILTIN_MINI_STDLIB */
