#include "renderPDF.h"
/**
* initPDFium(char* path)
*
* @param path the absolute path to the pdf document.
* @return
*/
FPDF_DOCUMENT initPDFium(char* path)
{

    FPDF_LIBRARY_CONFIG config;
    config.version = 2;
    config.m_pUserFontPaths = NULL;
    config.m_pIsolate = NULL;
    config.m_pPlatform = 0;
    FPDF_InitLibraryWithConfig(&config);
    //Load Document
    FPDF_BYTESTRING pdfEncryptionKey = NULL;
    if (!path)
        return NULL;
    FPDF_STRING docPath{ path };
    FPDF_DOCUMENT document = FPDF_LoadDocument(docPath, pdfEncryptionKey); //needs to be closed with fPDF_CloseDocumednt
    if (!document) {
        unsigned long err = FPDF_GetLastError();
        fprintf(stderr, "Load pdf docs unsuccessful: ");
        switch (err) {
        case FPDF_ERR_SUCCESS:
            fprintf(stderr, "Success");
            break;
        case FPDF_ERR_UNKNOWN:
            fprintf(stderr, "Unknown error");
            break;
        case FPDF_ERR_FILE:
            fprintf(stderr, "File not found or could not be opened");
            break;
        case FPDF_ERR_FORMAT:
            fprintf(stderr, "File not in PDF format or corrupted");
            break;
        case FPDF_ERR_PASSWORD:
            fprintf(stderr, "Password required or incorrect password");
            break;
        case FPDF_ERR_SECURITY:
            fprintf(stderr, "Unsupported security scheme");
            break;
        case FPDF_ERR_PAGE:
            fprintf(stderr, "Page not found or content error");
            break;
        default:
            fprintf(stderr, "Unknown error %ld", err);
        }
        FPDF_DestroyLibrary();
        return NULL;
    }
    return document;
   
}
void freePDFium()
{
    FPDF_DestroyLibrary();
}
/**
* getAllPages(FPDF_DOCUMENT)
* 
* @param doc a object of TYPE FPDF_DOCUMENT, that is the document we are returning the pages of
* @return an array pointer 
*/
FPDF_PAGE* getAllPages(FPDF_DOCUMENT doc)
{
    FPDF_PAGE* pageArray = new FPDF_PAGE[FPDF_GetPageCount(doc) * sizeof(FPDF_PAGE)];
    int i = 0;
    while (i < FPDF_GetPageCount(doc)) {
        if ((pageArray[i] = FPDF_LoadPage(doc, i)) == NULL) {
            fprintf(stderr, "Error while loading page %d",  i);
            std::exit(-1);
        }
        i++;
    }
    return pageArray;
}



