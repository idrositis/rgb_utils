#!/usr/bin/env python
#
# usage: rgb2cmyk.py [-h] [-v] InFile
#
# positional arguments:
#  InFile                Source image file
#
# optional arguments:
#  -h/--help             Show this help message and exit
#  -v/--verbose          Increase output verbosity
#
# Versions:
# - 1.0/2017.11: Initial happy version
#
# TODOs:
# - Support multiple output formats
#
#import argparse
from argparse import ArgumentParser
from argparse import RawTextHelpFormatter
import colortuple
from PIL import Image
from modules.commonlib import warning_msg
from modules.commonlib import error_msg
from modules.commonlib import debug_msg
from modules.commonlib import info_msg
from modules.commonlib import silence_stderr
import os

file_basename = os.path.basename(__file__)
file_version = '1.0|17/11'

def rgb_to_cmyk(r, g, b):
  """
  Convert an RGB color triplet to a CMYK quadruplet.
  """
#  debug_msg( "%d %d %d" % (r, g, b) )
  if r < 3 and g < 3 and b < 3:
    c4 = 0
    m4 = 0
    y4 = 0
    k4 = 100
  else:
    w = max( (r*100)/255, (g*100)/255, (b*100)/255 )
    c4 = ( (w - (r*100)/255)*100 ) / w
    m4 = ( (w - (g*100)/255)*100 ) / w
    y4 = ( (w - (b*100)/255)*100 ) / w
    k4 = 100 - w

  c3 = min(100, (c4*(100 - k4))/100 + k4);
  m3 = min(100, (m4*(100 - k4))/100 + k4);
  y3 = min(100, (y4*(100 - k4))/100 + k4);

  colorC = 255 - (c3*255)/100, 255, 255, 255
  colorM = 255, 255 - (m3*255)/100, 255, 255
  colorY = 255, 255, 255 - (y3*255)/100, 255
  colorK = 255 - (k4*255)/100, 255 - (k4*255)/100, 255 - (k4*255)/100, 255

  return colorC, colorM, colorY, colorK


# Main
def main():

  program_description = "\
 Utility to convert RGB image-files to their CMYK components - v{}\n\
 eg. {} brick-house.png\n\
     {} -h".format(file_version, file_basename, file_basename)

  parser = ArgumentParser(description=program_description, formatter_class=RawTextHelpFormatter)
  parser.add_argument('InFile', type=str, help='input file')
  parser.add_argument('-v', '--verbose', action='store_true', help='increase output verbosity')

  # Read arguments
  args = parser.parse_args()

  # No verbosity by default
  if (not args.verbose):
    silence_stderr()
  else:
    debug_msg("Verbose mode enabled")

  filename_input = args.InFile
  debug_msg( "Input file: '%s'" % (filename_input) )
  format_input = os.path.splitext(filename_input)[1][1:]

  # Main
  debug_msg( "Processing file '%s'..." % (filename_input) )

  img = Image.open(filename_input).convert('RGBA')

  width, height = img.size
  colortuples = img.getcolors()

  imgC = Image.new("RGB", (width, height))
  imgM = Image.new("RGB", (width, height))
  imgY = Image.new("RGB", (width, height))
  imgK = Image.new("RGB", (width, height))

  pix = img.load()
  for x in xrange(0, width):
    for y in xrange(0, height):

      colorC, colorM, colorY, colorK = rgb_to_cmyk(
        pix[x, y][0],
        pix[x, y][1],
        pix[x, y][2]
      )

      imgC.putpixel( (x, y), colorC )
      imgM.putpixel( (x, y), colorM )
      imgY.putpixel( (x, y), colorY )
      imgK.putpixel( (x, y), colorK )

  filename_outputC = filename_input.replace('.' + format_input, '-C.' + format_input)
  debug_msg( "Saving '%s'..." % (filename_outputC) )
  imgC.save( filename_outputC, format_input )

  filename_outputM = filename_input.replace('.' + format_input, '-M.' + format_input)
  debug_msg( "Saving '%s'..." % (filename_outputM) )
  imgM.save( filename_outputM, format_input )

  filename_outputY = filename_input.replace('.' + format_input, '-Y.' + format_input)
  debug_msg( "Saving '%s'..." % (filename_outputY) )
  imgY.save( filename_outputY, format_input )

  filename_outputK = filename_input.replace('.' + format_input, '-K.' + format_input)
  debug_msg( "Saving '%s'..." % (filename_outputK) )
  imgK.save( filename_outputK, format_input )


# Main
if __name__ == '__main__':
  main()
