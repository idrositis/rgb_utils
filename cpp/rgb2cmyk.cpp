/////////////////////////////////////////////////////////////////////////////////////////
// RGB2CMYK
// Convert an RGB image to CMYK channels.
//
// v1.1 | Oct 2016
//

// RGB <-> CMYK definitions
//
// CMY2RGB
// =================
// - RED   = 1-C3
// - GREEN = 1-M3
// - BLUE  = 1-Y3
//
// Since the RGB ans CMY color spaces are complements. Before doing that though, you
// have to convert CMYK to CMY by adding the "black" (K) value to the CMY values.
// The black value is the minimum of the other 3 values:
//
// CMYK2CMY
// =================
// C3 = minimum(1, C4*(1-K4)+K4)
// M3 = minimum(1, M4*(1-K4)+K4)
// Y3 = minimum(1, Y4*(1-K4)+K4)
//

#include "EasyBMP.h"			// EasyBMP classes

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////
// Functions

// Max for 3 integers
int max(int iA, int iB, int iC)
{
	int iMax = iA;

	if (iB > iMax)
		iMax = iB;
	else if (iC > iMax)
		iMax = iC;

	return iMax;
}

// Min for 2 integers
int min(int iA, int iB)
{
   int iMin = iA;

   if (iB < iMin)
      iMin = iB;

   return iMin;
}


// RGB to CMYK conversion
bool RGBtoCMYK (int iR, int iG, int iB, int *piC, int *piM, int *piY, int *piK)
{
	int iW;

	// Thorough error-checking
	if (piC == NULL || piM == NULL || piY == NULL || piK == NULL) {
		printf(" RGBtoCMYK(): null argument(s)!\n");
		return 0;
	}

	if (iR < 3 && iG < 3 && iB < 3)  {
   	*piC = 0;
		*piM = 0;
		*piY = 0;
		*piK = 100;
	}
	else {
		iW = max((iR*100)/255, (iG*100)/255, (iB*100)/255);
		*piC = ((iW - (iR*100)/255)*100) / iW;
		*piM = ((iW - (iG*100)/255)*100) / iW;
		*piY = ((iW - (iB*100)/255)*100) / iW;
		*piK = 100 - iW;
	}

	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char* argv[]) {

 // Argument check
 if (argc != 2) {
    cout << "\n== rgp2cmyk | v1.0 - Apr2011 ======================================\n"
         << " Convert an RGB image to C|M|Y|K channels.\n"
         << "   Usage: rgp2cmyk <bmp_filename>\n"
         << "  Output: <bmp_filename_C.bmp> Cyan component\n"
         << "          <bmp_filename_M.bmp> Magenta component\n"
         << "          <bmp_filename_Y.bmp> Yellow component\n"
         << "          <bmp_filename_K.bmp> Black component\n\n"
         << " [using EasyBMP library | http://easybmp.sourceforge.net]\n"
         << "====================================================================\n";
   return 1;
 }

 // Read input file
 BMP Input;
 Input.ReadFromFile(argv[1]);

 // Initialize output BMP classes
 BMP OutputC, OutputM, OutputY, OutputK;
 OutputC.SetSize(Input.TellWidth(), Input.TellHeight());
 OutputM.SetSize(Input.TellWidth(), Input.TellHeight());
 OutputY.SetSize(Input.TellWidth(), Input.TellHeight());
 OutputK.SetSize(Input.TellWidth(), Input.TellHeight());

 // Decompose each pixel of input image
 double dR, dG, dB,				// RGB channel values in percentage
        dC3, dM3, dY3,			// CMYK channel values in percentage
        dC4, dM4, dY4, dK4;	// CMYK channel values in percentage
 double dY, dU, dV;				// RGB & YUV channel values in double preccision
 ebmpBYTE R, G, B,
          tmpByte;
 int      C3, M3, Y3,
          C4, M4, Y4, K4;
 int i, j;
 bool bRetVal = 0;

 for (i=0; i < Input.TellWidth(); i++) {
    for (j=0; j < Input.TellHeight(); j++) {
       // Retrieve RGB components
       R = Input(i, j)->Red;
       G = Input(i, j)->Green;
       B = Input(i, j)->Blue;

       // Convert to CMYK (0-100)
       bRetVal = RGBtoCMYK(R, G, B, &C4, &M4, &Y4, &K4);

       C3 = min(100, (C4*(100 - K4))/100 + K4);
       M3 = min(100, (M4*(100 - K4))/100 + K4);
       Y3 = min(100, (Y4*(100 - K4))/100 + K4);

       OutputC(i, j)->Red   = (ebmpBYTE)(255 - (C3*255)/100);
       OutputC(i, j)->Green = 255;
       OutputC(i, j)->Blue  = 255;

       OutputM(i, j)->Red   = 255;
       OutputM(i, j)->Green = (ebmpBYTE)(255 - (M3*255)/100);
       OutputM(i, j)->Blue  = 255;

       OutputY(i, j)->Red   = 255;
       OutputY(i, j)->Green = 255;
       OutputY(i, j)->Blue  = (ebmpBYTE)(255 - (Y3*255)/100);

       OutputK(i, j)->Red   = (ebmpBYTE)(255 - (K4*255)/100);
       OutputK(i, j)->Green = (ebmpBYTE)(255 - (K4*255)/100);
       OutputK(i, j)->Blue  = (ebmpBYTE)(255 - (K4*255)/100);
/*
       // Some debug printing...
       if (j % 100 == 0 && i % 100 == 0) {
          printf("(%d, %d):\tR=%d | G=%d | B=%d \n", i, j, Input(i, j)->Red, Input(i, j)->Green, Input(i, j)->Blue);
          printf("         \tC=%d | M=%d | Y=%d | K=%d \n", C4, M4, Y4, K4);
          printf("         \tC=%d | M=%d | Y=%d \n", C3, M3, Y3);
       }
*/
    }
 }

 // Set output bitmap depth
 OutputC.SetBitDepth(Input.TellBitDepth());
 OutputM.SetBitDepth(Input.TellBitDepth());
 OutputY.SetBitDepth(Input.TellBitDepth());
 OutputK.SetBitDepth(Input.TellBitDepth());

 // Compose output filename
 char strOutputFilename[ 50 ];
 strcpy(strOutputFilename, argv[1]);
 char * strBMPextChar = strrchr(strOutputFilename, '.');
 if (strBMPextChar != NULL) {
    // Compose output filename and write files
    strcpy(strBMPextChar, "_C.bmp");
    cout << " C: " << strOutputFilename << "... ";
    OutputC.WriteToFile(strOutputFilename);
    cout << " OK\n";

    *(strBMPextChar+1) = 'M';
    cout << " M: " << strOutputFilename << "... ";
    OutputM.WriteToFile(strOutputFilename);
    cout << " OK\n";

    *(strBMPextChar+1) = 'Y';
    cout << " Y: " << strOutputFilename << "... ";
    OutputY.WriteToFile(strOutputFilename);
    cout << " OK\n";

    *(strBMPextChar+1) = 'K';
    cout << " K: " << strOutputFilename << "... ";
    OutputK.WriteToFile(strOutputFilename);
    cout << " OK\n";
 }
 else {
    char strOutputFilename2[ 50 ];
    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_C.bmp");
    cout << " C: " << strOutputFilename2 << "... ";
    OutputC.WriteToFile(strOutputFilename);
    cout << " OK\n";

    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_M.bmp");
    cout << " M: " << strOutputFilename2 << "... ";
    OutputM.WriteToFile(strOutputFilename);
    cout << " OK\n";

    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_Y.bmp");
    cout << " Y: " << strOutputFilename2 << "... ";
    OutputY.WriteToFile(strOutputFilename);
    cout << " OK\n";

    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_K.bmp");
    cout << " K: " << strOutputFilename2 << "... ";
    OutputK.WriteToFile(strOutputFilename);
    cout << " OK\n";
 }

 return 0;
}
