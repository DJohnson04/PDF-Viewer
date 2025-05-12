#pragma once


#include "../pdfium/public/fpdfview.h"
#include "renderPDF.h"
#include <stdio.h>
#include <iostream>
#include <string>
FPDF_DOCUMENT initPDFium(char* path);
void freePDFium();
FPDF_PAGE* getAllPages(FPDF_DOCUMENT doc);