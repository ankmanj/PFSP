#ifndef __INCLUDE_XML_NODE__
#define __INCLUDE_XML_NODE__

#include <stdlib.h>

#ifdef _UNICODE
#define _XMLWIDECHAR
#endif

#if defined(WIN32) || defined(UNDER_CE) || defined(_WIN32) || defined(WIN64) || defined(__BORLANDC__)
#define _XMLWINDOWS
#endif

#ifdef XMLDLLENTRY
#undef XMLDLLENTRY
#endif
#ifdef _USE_XMLPARSER_DLL
#ifdef _DLL_EXPORTS_
#define XMLDLLENTRY __declspec(dllexport)
#else
#define XMLDLLENTRY __declspec(dllimport)
#endif
#else
#define XMLDLLENTRY
#endif


#if !defined(WIN32) && !defined(LINUX)
#define XML_NO_FILESYSTEM
#endif

#if !defined(XML_NO_WIDE_CHAR)
#define XML_NO_WIDE_CHAR
#endif

#ifdef XML_NO_WIDE_CHAR
#undef _XMLWINDOWS
#undef _XMLWIDECHAR
#endif

#ifdef _XMLWINDOWS
#include <tchar.h>
#else
#define XMLDLLENTRY
#ifndef XML_NO_WIDE_CHAR
#include <wchar.h> 
#endif
#endif

#ifdef _XMLWIDECHAR
    #define _CXML(c) L ## c
    #define XMLCSTR const wchar_t *
    #define XMLSTR  wchar_t *
    #define XMLCHAR wchar_t
#else
    #define _CXML(c) c
    #define XMLCSTR const char *
    #define XMLSTR  char *
    #define XMLCHAR char
#endif
#ifndef FALSE
    #define FALSE 0
#endif /* FALSE */
#ifndef TRUE
    #define TRUE 1
#endif /* TRUE */

typedef enum XMLError
{
    eXMLErrorNone = 0,
    eXMLErrorMissingEndTag,
    eXMLErrorNoXMLTagFound,
    eXMLErrorEmpty,
    eXMLErrorMissingTagName,
    eXMLErrorMissingEndTagName,
    eXMLErrorUnmatchedEndTag,
    eXMLErrorUnmatchedEndClearTag,
    eXMLErrorUnexpectedToken,
    eXMLErrorNoElements,
    eXMLErrorFileNotFound,
    eXMLErrorFirstTagNotFound,
    eXMLErrorUnknownCharacterEntity,
    eXMLErrorCharacterCodeAbove255,
    eXMLErrorCharConversionError,
    eXMLErrorCannotOpenWriteFile,
    eXMLErrorCannotWriteFile,

    eXMLErrorBase64DataSizeIsNotMultipleOf4,
    eXMLErrorBase64DecodeIllegalCharacter,
    eXMLErrorBase64DecodeTruncatedData,
    eXMLErrorBase64DecodeBufferTooSmall
} XMLError;

typedef enum XMLElementType
{
    eNodeChild=0,
    eNodeAttribute=1,
    eNodeText=2,
    eNodeClear=3,
    eNodeNULL=4
} XMLElementType;

typedef struct XMLResults
{
    enum XMLError error;
    int  nLine,nColumn;
} XMLResults;

typedef struct XMLClear {
    XMLCSTR lpszValue; XMLCSTR lpszOpenTag; XMLCSTR lpszCloseTag;
} XMLClear;

typedef struct XMLAttribute {
    XMLCSTR lpszName; XMLCSTR lpszValue;
} XMLAttribute;

typedef int XMLElementPosition;

struct XMLNodeContents;

typedef struct XMLDLLENTRY XMLNode
{
  private:

    struct XMLNodeDataTag;

    XMLNode(struct XMLNodeDataTag *pParent, XMLSTR lpszName, char isDeclaration);
    XMLNode(struct XMLNodeDataTag *p);

  public:
    static XMLCSTR getVersion();

    static XMLNode parseString   (XMLCSTR  lpXMLString, XMLCSTR tag=NULL, XMLResults *pResults=NULL);
    static XMLNode parseFile     (XMLCSTR     filename, XMLCSTR tag=NULL, XMLResults *pResults=NULL);
    static XMLNode openFileHelper(XMLCSTR     filename, XMLCSTR tag=NULL);
    static XMLSTR openFileToMemory(XMLCSTR filename, size_t* bufferSize, long* offset);

    static XMLCSTR getError(XMLError error);

    XMLSTR createXMLString(int nFormat=1, int *pnSize=NULL) const;
    XMLError writeToFile(XMLCSTR filename,
                         const char *encoding=NULL,
                         char nFormat=1) const;
    XMLCSTR getName() const;
    XMLCSTR getText(int i=0) const;
    int nText() const;
    XMLNode getParentNode() const;
    XMLNode getChildNode(int i=0) const;
    XMLNode getChildNode(XMLCSTR name, int i)  const;
    XMLNode getChildNode(XMLCSTR name, int *i=NULL) const;
    XMLNode getChildNodeWithAttribute(XMLCSTR tagName,
                                      XMLCSTR attributeName,
                                      XMLCSTR attributeValue=NULL,
                                      int *i=NULL)  const;
    XMLNode getChildNodeByPath(XMLCSTR path, char createNodeIfMissing=0, XMLCHAR sep='/');
                                                                  
    XMLNode getChildNodeByPathNonConst(XMLSTR  path, char createNodeIfMissing=0, XMLCHAR sep='/');
                                                                 

    int nChildNode(XMLCSTR name) const;
    int nChildNode() const;                                       
    XMLAttribute getAttribute(int i=0) const;              
    XMLCSTR      getAttributeName(int i=0) const;  
    XMLCSTR      getAttributeValue(int i=0) const; 
    char  isAttributeSet(XMLCSTR name) const;       
    XMLCSTR getAttribute(XMLCSTR name, int i) const;               
    XMLCSTR getAttribute(XMLCSTR name, int *i=NULL) const;   
    int nAttribute() const;                                
    XMLClear getClear(int i=0) const;                
    int nClear() const;                                        
    XMLNodeContents enumContents(XMLElementPosition i) const;     
    int nElement() const;                                        
    char isEmpty() const;                                         
    char isDeclaration() const;                            
    XMLNode deepCopy() const;                           
    static XMLNode emptyNode();                         

    ~XMLNode();
    XMLNode(const XMLNode &A);                       
    XMLNode& operator=( const XMLNode& A );   

    XMLNode(): d(NULL){};
    static XMLNode emptyXMLNode;
    static XMLClear emptyXMLClear;
    static XMLAttribute emptyXMLAttribute;

    static XMLNode createXMLTopNode(XMLCSTR lpszName, char isDeclaration=FALSE);                    
    XMLNode        addChild(XMLCSTR lpszName, char isDeclaration=FALSE, XMLElementPosition pos=-1); 
    XMLNode        addChild(XMLNode nodeToAdd, XMLElementPosition pos=-1);                 
    XMLAttribute  *addAttribute(XMLCSTR lpszName, XMLCSTR lpszValuev);                       
    XMLCSTR        addText(XMLCSTR lpszValue, XMLElementPosition pos=-1);                         
    XMLClear      *addClear(XMLCSTR lpszValue, XMLCSTR lpszOpen=NULL, XMLCSTR lpszClose=NULL, XMLElementPosition pos=-1);
    XMLCSTR       updateName(XMLCSTR lpszName); 
    XMLAttribute *updateAttribute(XMLAttribute *newAttribute, XMLAttribute *oldAttribute);      
    XMLAttribute *updateAttribute(XMLCSTR lpszNewValue, XMLCSTR lpszNewName=NULL,int i=0);  
    XMLAttribute *updateAttribute(XMLCSTR lpszNewValue, XMLCSTR lpszNewName,XMLCSTR lpszOldName);
    XMLCSTR       updateText(XMLCSTR lpszNewValue, int i=0);                                    
    XMLCSTR       updateText(XMLCSTR lpszNewValue, XMLCSTR lpszOldValue);          
    XMLClear     *updateClear(XMLCSTR lpszNewContent, int i=0);                               
    XMLClear     *updateClear(XMLClear *newP,XMLClear *oldP);                                 
    XMLClear     *updateClear(XMLCSTR lpszNewValue, XMLCSTR lpszOldValue);        
    void deleteNodeContent();
    void deleteAttribute(int i=0);
    void deleteAttribute(XMLCSTR lpszName);          
    void deleteAttribute(XMLAttribute *anAttribute); 
    void deleteText(int i=0);                       
    void deleteText(XMLCSTR lpszValue);  
    void deleteClear(int i=0);                     
    void deleteClear(XMLCSTR lpszValue);
    void deleteClear(XMLClear *p);            
    static XMLNode createXMLTopNode_WOSD(XMLSTR lpszName, char isDeclaration=FALSE);   
    XMLNode        addChild_WOSD(XMLSTR lpszName, char isDeclaration=FALSE, XMLElementPosition pos=-1); 
    XMLAttribute  *addAttribute_WOSD(XMLSTR lpszName, XMLSTR lpszValue);               
    XMLCSTR        addText_WOSD(XMLSTR lpszValue, XMLElementPosition pos=-1);            
    XMLClear      *addClear_WOSD(XMLSTR lpszValue, XMLCSTR lpszOpen=NULL, XMLCSTR lpszClose=NULL, XMLElementPosition pos=-1);
    XMLCSTR        updateName_WOSD(XMLSTR lpszName);                                            
    XMLAttribute  *updateAttribute_WOSD(XMLAttribute *newAttribute, XMLAttribute *oldAttribute);     
    XMLAttribute  *updateAttribute_WOSD(XMLSTR lpszNewValue, XMLSTR lpszNewName=NULL,int i=0);       
    XMLAttribute  *updateAttribute_WOSD(XMLSTR lpszNewValue, XMLSTR lpszNewName,XMLCSTR lpszOldName); 
    XMLCSTR        updateText_WOSD(XMLSTR lpszNewValue, int i=0);                                    
    XMLCSTR        updateText_WOSD(XMLSTR lpszNewValue, XMLCSTR lpszOldValue);          
    XMLClear      *updateClear_WOSD(XMLSTR lpszNewContent, int i=0);                              
    XMLClear      *updateClear_WOSD(XMLClear *newP,XMLClear *oldP);                               
    XMLClear      *updateClear_WOSD(XMLSTR lpszNewValue, XMLCSTR lpszOldValue);        
    XMLElementPosition positionOfText(int i=0) const;
    XMLElementPosition positionOfText(XMLCSTR lpszValue) const;
    XMLElementPosition positionOfClear(int i=0) const;
    XMLElementPosition positionOfClear(XMLCSTR lpszValue) const;
    XMLElementPosition positionOfClear(XMLClear *a) const;
    XMLElementPosition positionOfChildNode(int i=0) const;
    XMLElementPosition positionOfChildNode(XMLNode x) const;
    XMLElementPosition positionOfChildNode(XMLCSTR name, int i=0) const; 
    typedef enum XMLCharEncoding
    {
        char_encoding_error=0,
        char_encoding_UTF8=1,
        char_encoding_legacy=2,
        char_encoding_ShiftJIS=3,
        char_encoding_GB2312=4,
        char_encoding_Big5=5,
        char_encoding_GBK=6
    } XMLCharEncoding;

    static char setGlobalOptions(XMLCharEncoding characterEncoding=XMLNode::char_encoding_UTF8, char guessWideCharChars=1,
                                 char dropWhiteSpace=1, char removeCommentsInMiddleOfText=1);
    static XMLCharEncoding guessCharEncoding(void *buffer, int bufLen, char useXMLEncodingAttribute=1);

  private:

      typedef struct XMLNodeDataTag 
      {
          XMLCSTR                lpszName;     
          int                    nChild,          
                                 nText,           
                                 nClear,          
                                 nAttribute;   
          char                   isDeclaration;   
          struct XMLNodeDataTag  *pParent;
          XMLNode                *pChild;     
          XMLCSTR                *pText;       
          XMLClear               *pClear;       
          XMLAttribute           *pAttribute;
          int                    *pOrder;        
          int                    ref_count;      
      } XMLNodeData;
      XMLNodeData *d;

      char parseClearTag(void *px, void *pa);
      char maybeAddTxT(void *pa, XMLCSTR tokenPStr);
      int ParseXMLElement(void *pXML);
      void *addToOrder(int memInc, int *_pos, int nc, void *p, int size, XMLElementType xtype);
      int indexText(XMLCSTR lpszValue) const;
      int indexClear(XMLCSTR lpszValue) const;
      XMLNode addChild_priv(int,XMLSTR,char,int);
      XMLAttribute *addAttribute_priv(int,XMLSTR,XMLSTR);
      XMLCSTR addText_priv(int,XMLSTR,int);
      XMLClear *addClear_priv(int,XMLSTR,XMLCSTR,XMLCSTR,int);
      void emptyTheNode(char force);
      static inline XMLElementPosition findPosition(XMLNodeData *d, int index, XMLElementType xtype);
      static int CreateXMLStringR(XMLNodeData *pEntry, XMLSTR lpszMarker, int nFormat);
      static int removeOrderElement(XMLNodeData *d, XMLElementType t, int index);
      static void exactMemory(XMLNodeData *d);
      static int detachFromParent(XMLNodeData *d);
      static void setCharEncodingOption(XMLCSTR filename);
      static unsigned char* copyFileToMemory(XMLCSTR filename, XMLResults *pResults, size_t* bufferSize, long *offset);
} XMLNode;

typedef struct XMLNodeContents
{
    enum XMLElementType etype;
    XMLNode child;
    XMLAttribute attrib;
    XMLCSTR text;
    XMLClear clear;

} XMLNodeContents;

XMLDLLENTRY XMLSTR stringDup(XMLCSTR source, int cbData=-1);

XMLDLLENTRY void freeXMLString(XMLSTR t); // {free(t);}
XMLDLLENTRY char    xmltob(XMLCSTR xmlString,char   defautValue=0);
XMLDLLENTRY int     xmltoi(XMLCSTR xmlString,int    defautValue=0);
XMLDLLENTRY long    xmltol(XMLCSTR xmlString,long   defautValue=0);
XMLDLLENTRY double  xmltof(XMLCSTR xmlString,double defautValue=.0);
XMLDLLENTRY XMLCSTR xmltoa(XMLCSTR xmlString,XMLCSTR defautValue=_CXML(""));
XMLDLLENTRY XMLCHAR xmltoc(XMLCSTR xmlString,const XMLCHAR defautValue=_CXML('\0'));
typedef struct XMLDLLENTRY ToXMLStringTool
{
public:
    ToXMLStringTool(): buf(NULL),buflen(0){}
    ~ToXMLStringTool();
    void freeBuffer();

    XMLSTR toXML(XMLCSTR source);

    static XMLSTR toXMLUnSafe(XMLSTR dest,XMLCSTR source);
    static int lengthXMLString(XMLCSTR source);

private:
    XMLSTR buf;
    int buflen;
} ToXMLStringTool;

typedef struct XMLDLLENTRY XMLParserBase64Tool
{
public:
    XMLParserBase64Tool(): buf(NULL),buflen(0){}
    ~XMLParserBase64Tool();
    void freeBuffer();
    static int encodeLength(int inBufLen, char formatted=0); 

    XMLSTR encode(unsigned char *inByteBuf, unsigned int inByteLen, char formatted=0); 

    static unsigned int decodeSize(XMLCSTR inString, XMLError *xe=NULL);

    unsigned char* decode(XMLCSTR inString, int *outByteLen=NULL, XMLError *xe=NULL);

    static unsigned char decode(XMLCSTR inString, unsigned char *outByteBuf, int inMaxByteOutBuflen, XMLError *xe=NULL);

private:
    void *buf;
    int buflen;
    void alloc(int newsize);
}XMLParserBase64Tool;

#undef XMLDLLENTRY

#endif
