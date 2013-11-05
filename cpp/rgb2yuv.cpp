////////////////////////////////////////////////////////////////////////////////
// RGB2YUV
// Convert an RGB image to YUV channels.
//
// v1.0 | Oct 2011
//

// RGB -> YUV definitions
#define		W_R	0.299
#define		W_B	0.114
#define		W_G	0.587		// 1 - W_R - W_B
#define		U_MAX	0.436
#define		V_MAX	0.615

#include "EasyBMP.h"	// EasyBMP classes

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char* argv[]) {

 // Argument check
 if (argc != 2) {
    cout << "\n== rgp2yuv | v1.0 - Oct2011 =======================================\n" 
         << " Convert an RGB image to Y|U|V channels.\n"
         << "   Usage: rgp2yuv <bmp_filename>\n"
         << "  Output: <bmp_filename_Y.bmp> Brightness/Luminance\n"
         << "          <bmp_filename_U.bmp> BLUE scaled difference\n"
         << "          <bmp_filename_V.bmp> RED scaled difference\n\n"
         << " [using EasyBMP library |  http://easybmp.sourceforge.net]\n"
         << "====================================================================\n";
   return 1;
 }

 // Read input file
 BMP Input;
 Input.ReadFromFile(argv[1]);
 
 // Initialize output BMP classes 
 BMP OutputY, OutputU, OutputV;
 OutputY.SetSize(Input.TellWidth(), Input.TellHeight());
 OutputU.SetSize(Input.TellWidth(), Input.TellHeight());
 OutputV.SetSize(Input.TellWidth(), Input.TellHeight());

 // Decompose each pixel of input image
 double dR, dG, dB, dY, dU, dV;		// RGB & YUV channel values in double preccision
 ebmpBYTE tmpByte, Y, U, V;
 int i, j;
 for (i=0; i < Input.TellWidth(); i++) {
    for (j=0; j < Input.TellHeight(); j++) {
       // Compute YUV channels
       dR = Input(i, j)->Red/256.0;
       dG = Input(i, j)->Green/256.0;
       dB = Input(i, j)->Blue/256.0;

       dY = W_R*dR + W_G*dG + W_B*dB;
       dU = (U_MAX * (dB - dY)) / (1.0 - W_B);
       dV = (V_MAX * (dR - dY)) / (1.0 - W_R);
       dY *= 256;
       dU += .436; dU *= (256/.872);
       dV += .615; dV *= (256/1.23);

       Y = (ebmpBYTE)(dY + 0.5);
       U = (ebmpBYTE)(dU + 0.5);
       V = (ebmpBYTE)(dV + 0.5);

       OutputY(i, j)->Red   = Y; 
       OutputY(i, j)->Green = Y;
       OutputY(i, j)->Blue  = Y;

       OutputU(i, j)->Red   = 0;
       OutputU(i, j)->Green = 255-U;
       OutputU(i, j)->Blue  = U;

       OutputV(i, j)->Red   = V;
       OutputV(i, j)->Green = 255-V;
       OutputV(i, j)->Blue  = 0;

       // Some debug printing...
//       if (j % 100 == 0 && i % 100 == 0) {
//          printf("(%d, %d):\tR=%d | G=%d | B=%d \n", i, j, Input(i, j)->Red, Input(i, j)->Green, Input(i, j)->Blue);
//          printf("         \tY=%d | U=%d | V=%d \n", Y, U, V);
//       }
    }

 }

 OutputY.SetBitDepth(Input.TellBitDepth());
 OutputU.SetBitDepth(Input.TellBitDepth());
 OutputV.SetBitDepth(Input.TellBitDepth());
// OutputR.SetBitDepth(8);
// OutputG.SetBitDepth(8);
// OutputB.SetBitDepth(8);

 // Create color table
// CreateGrayscaleColorTable( OutputR );
// CreateGrayscaleColorTable( OutputG );
// CreateGrayscaleColorTable( OutputB );
// OutputR.CreateStandardColorTable();
// OutputG.CreateStandardColorTable();
// OutputB.CreateStandardColorTable();

 // Compose output filename
 char strOutputFilename[ 50 ];
 strcpy(strOutputFilename, argv[1]);
 char * strBMPextChar = strrchr(strOutputFilename, '.');
 if (strBMPextChar != NULL) {
    // Compose output filename and write files
    strcpy(strBMPextChar, "_Y.bmp");
    cout << " Y: " << strOutputFilename << "... ";
    OutputY.WriteToFile(strOutputFilename);
    cout << " OK\n";

    *(strBMPextChar+1) = 'U';
    cout << " U: " << strOutputFilename << "... ";
    OutputU.WriteToFile(strOutputFilename);
    cout << " OK\n";

    *(strBMPextChar+1) = 'V';
    cout << " V: " << strOutputFilename << "... ";
    OutputV.WriteToFile(strOutputFilename);
    cout << " OK\n";
 }
 else {
    char strOutputFilename2[ 50 ];
    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_Y.bmp");
    cout << " Y: " << strOutputFilename2 << "... ";
    OutputY.WriteToFile(strOutputFilename);
    cout << " OK\n";

    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_U.bmp");
    cout << " U: " << strOutputFilename2 << "... ";
    OutputU.WriteToFile(strOutputFilename);
    cout << " OK\n";

    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_V.bmp");
    cout << " V: " << strOutputFilename2 << "... ";
    OutputV.WriteToFile(strOutputFilename);
    cout << " OK\n";
 }

 return 0;
}
