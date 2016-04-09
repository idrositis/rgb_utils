////////////////////////////////////////////////////////////////////////////////
// RGB-split
// Split RGB image to _R/G/B.bmp images.
//
// v1.0 | Oct 2011
//
#include "EasyBMP.h"	// EasyBMP classes

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char* argv[]) {

 // Argument check
 if (argc != 2) {
    cout << "\n== rgp-split | v1.0 - Oct2011 =======================================\n" 
         << " Split an RGB image to R|G|B channels.\n"
         << "   Usage: rgp-split <bmp_filename>\n"
         << "  Output: <bmp_filename_R.bmp> RED channel\n"
         << "          <bmp_filename_G.bmp> GREEN channel\n"
         << "          <bmp_filename_B.bmp> BLUE channel\n\n"
         << " [using EasyBMP library |  http://easybmp.sourceforge.net]\n"
         << "=====================================================================\n";
   return 1;
 }

 // Read input file
 BMP Input;
 Input.ReadFromFile(argv[1]);
 
 // Initialize output BMP classes 
 BMP OutputR, OutputG, OutputB;
 OutputR.SetSize(Input.TellWidth(), Input.TellHeight());
 OutputG.SetSize(Input.TellWidth(), Input.TellHeight());
 OutputB.SetSize(Input.TellWidth(), Input.TellHeight());

 // Decompose each pixel of input image
 double Temp;
 ebmpBYTE tmpByte;
 int i, j;
 for (i=0; i < Input.TellWidth(); i++) {
    for (j=0; j < Input.TellHeight(); j++) {
       // Channel decomposition
       OutputR(i, j)->Red   = Input(i, j)->Red;
       OutputR(i, j)->Green = (ebmpBYTE)0.0;
       OutputR(i, j)->Blue  = (ebmpBYTE)0.0;

       OutputG(i, j)->Red   = (ebmpBYTE)0.0;
       OutputG(i, j)->Green = Input(i, j)->Green;
       OutputG(i, j)->Blue  = (ebmpBYTE)0.0;

       OutputB(i, j)->Red   = (ebmpBYTE)0.0;
       OutputB(i, j)->Green = (ebmpBYTE)0.0;
       OutputB(i, j)->Blue  = Input(i, j)->Blue;
    }

    // Some debug printing...
//    if (i % 100 == 0) {
//       printf("(%d, %d): %d | %f \n", i, j, tmpByte, Temp);
//    }
 }

 OutputR.SetBitDepth(Input.TellBitDepth());
 OutputG.SetBitDepth(Input.TellBitDepth());
 OutputB.SetBitDepth(Input.TellBitDepth());
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
    strcpy(strBMPextChar, "_R.bmp");
    cout << " R: " << strOutputFilename << "... ";
    OutputR.WriteToFile(strOutputFilename);
    cout << " OK\n";

    *(strBMPextChar+1) = 'G';
    cout << " G: " << strOutputFilename << "... ";
    OutputG.WriteToFile(strOutputFilename);
    cout << " OK\n";

    *(strBMPextChar+1) = 'B';
    cout << " B: " << strOutputFilename << "... ";
    OutputB.WriteToFile(strOutputFilename);
    cout << " OK\n";
 }
 else {
    char strOutputFilename2[ 50 ];
    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_R.bmp");
    cout << " R: " << strOutputFilename2 << "... ";
    OutputR.WriteToFile(strOutputFilename);
    cout << " OK\n";

    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_G.bmp");
    cout << " G: " << strOutputFilename2 << "... ";
    OutputG.WriteToFile(strOutputFilename);
    cout << " OK\n";

    strcpy(strOutputFilename2, strOutputFilename);
    strcat(strOutputFilename2, "_B.bmp");
    cout << " B: " << strOutputFilename2 << "... ";
    OutputB.WriteToFile(strOutputFilename);
    cout << " OK\n";
 }

 // Create a greyscale color table
// Output.CreateStandardColorTable();
/*
 // Convert each pixel to greyscale
 double Temp;
 ebmpBYTE tmpByte;
 int i, j;
 for (i=0; i < Input.TellWidth(); i++) {
    for (j=0; j < Input.TellHeight(); j++) {
       // Greyscale component
       Temp = 0.30*( Input(i, j)->Red   ) +
              0.59*( Input(i, j)->Green ) +
              0.11*( Input(i, j)->Blue  );
       // Byte equivalent
       tmpByte = (ebmpBYTE) Temp;

       Output(i, j)->Red   = tmpByte;
       Output(i, j)->Green = tmpByte;
       Output(i, j)->Blue  = tmpByte;
    }

    if (i % 100 == 0) {
       printf("(%d, %d): %d | %f \n", i, j, tmpByte, Temp);
    }
 }

// Output.SetBitDepth( Input.TellBitDepth() );
 Output.SetBitDepth( 8 );

 // Create a greyscale color table
 CreateGrayscaleColorTable( Output );

 // Create a greyscale color table
// Output.CreateStandardColorTable();
 
 Output.WriteToFile( "EasyBMPbackground_CYV.bmp" );
*/

 return 0;
}
