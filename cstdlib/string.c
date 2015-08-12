/* string.h library for large systems - small embedded systems use clibrary.c instead */
#include "../interpreter.h"

#ifndef BUILTIN_MINI_STDLIB

static int String_ZeroValue = 0;

void StringStrcpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strcpy((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

void StringStrncpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strncpy((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringStrcmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = strcmp((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

void StringStrncmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = strncmp((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringStrcat(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strcat((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

void StringStrncat(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strncat((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringStrlen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int) strlen((char *) Param[0]->Val->Pointer);
}

void StringMemset(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = memset(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

void StringMemcpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = memcpy(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringMemcmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = memcmp(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringMemmove(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = memmove(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringMemchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = memchr(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

void StringStrchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strchr((char *) Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

void StringStrrchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strrchr((char *) Param[0]->Val->Pointer, Param[1]->Val->Integer);
}

void StringStrcoll(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = strcoll((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

void StringStrerror(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strerror(Param[0]->Val->Integer);
}

void StringStrspn(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int) strspn((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

void StringStrcspn(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int) strcspn((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

void StringStrpbrk(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strpbrk((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

void StringStrstr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strstr((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

void StringStrtok(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strtok((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer);
}

void StringStrxfrm(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Integer = (int) strxfrm((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void StringStrdup(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
#ifndef _WIN32
    ReturnValue->Val->Pointer = strdup((char *) Param[0]->Val->Pointer);
#else
	ReturnValue->Val->Pointer = _strdup((char *)Param[0]->Val->Pointer);
#endif
}

#ifndef WIN32
void StringStrtok_r(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
	UNUSED(Parser); UNUSED(NumArgs); 
    ReturnValue->Val->Pointer = strtok_r((char *) Param[0]->Val->Pointer, (char *) Param[1]->Val->Pointer, Param[2]->Val->Pointer);
}
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
    { StringStrxfrm,       "int strxfrm(char *,char *,int);" },
	{ StringStrdup,        "char *strdup(char *);" },
#ifndef WIN32
    { StringStrtok_r,      "char *strtok_r(char *,char *,char **);" },
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
