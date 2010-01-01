#ifndef _FDXPARSER_H_
#define _FDXPARSER_H_

#ifndef __cplusplus
#ifndef FDXBOOL
#define FDXBOOL
typedef char bool;
#endif
#endif

#ifndef WIN32
#define WINAPI
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

void* WINAPI createParser();
void* WINAPI copyParser(void *parser);
void  WINAPI destroyParser(void *parser);
void  WINAPI build(void *parser, char *transBuffer, long bufferSize, long *transLength);
bool  WINAPI parse(void *parser, const char *transaction, long *errorOffset);
void  WINAPI clear(void *parser);
void  WINAPI setTransactionType(void *parser, const char *type);
void  WINAPI getTransactionType(void *parser, char *typeBuffer, long bufferSize, long *typeLength);
void  WINAPI setField(void *parser, const char *tag, const char *value);
void  WINAPI setFieldInstance(void *parser, const char *tag, const char *value, short offset);
void  WINAPI setFieldInArray(void *parser, const char *tag, const char *value, short offsets[], short offsetsSize);
void  WINAPI getField(void *parser, const char *tag, char *valueBuffer, long bufferSize, long *valueLength);
void  WINAPI getFieldInstance(void *parser, const char *tag, char *valueBuffer, long bufferSize, long *valueLength, short offset);
void  WINAPI getFieldFromArray(void *parser, const char *tag, char *valueBuffer, long bufferSize, long *valueLength, short offsets[], short offsetsSize);
void  WINAPI setBinaryField(void *parser, const char *tag, const char *value, long valueLength);
void  WINAPI setBinaryFieldInstance(void *parser, const char *tag, const char *value, long valueLength, short offset);
void  WINAPI setBinaryFieldInArray(void *parser, const char *tag, const char *value, long valueLength, short offsets[], short offsetsSize);
void  WINAPI getBinaryField(void *parser, const char *tag, char *valueBuffer, long bufferSize, long *valueLength);
void  WINAPI getBinaryFieldInstance(void *parser, const char *tag, char *valueBuffer, long bufferSize, long *valueLength, short offset);
void  WINAPI getBinaryFieldFromArray(void *parser, const char *tag, char *valueBuffer, long bufferSize, long *valueLength, short offsets[], short offsetsSize);
bool  WINAPI fieldExists(void *parser, const char *tag); // Deprecated. Replaced by elemExists.
bool  WINAPI elemExists(void *parser, const char *tag);
bool  WINAPI fieldInstanceExists(void *parser, const char *tag, short offset);
bool  WINAPI elemExistsInArray(void *parser, const char *tag, short offsets[], short offsetsSize);
short WINAPI getInstanceCount(void *parser, const char *tag); // Deprecated. Replaced by getSizeOfArray.
short WINAPI getSizeOfArray(void *parser, const char *tag);
short WINAPI getSizeOfArrayInArray(void *parser, const char *tag, short offsets[], short offsetsSize);
void  WINAPI enumerateTags(void *parser);
bool  WINAPI getNextTag(void *parser, char *tagBuffer, long bufferSize, long *tagLength);

#ifdef __cplusplus
}
#endif

#endif // _FDXPARSER_H_
