#!/usr/bin/env python
# https://www.sitepoint.com/manipulating-images-with-the-python-imaging-library/
#
import colortuple
from PIL import Image

def rgb_to_cmyk (r, g, b):
#  print r,g,b
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

  return c4, m4, y4, k4


img = Image.open('brick-house.png').convert('RGBA')

width, height = img.size
colortuples = img.getcolors()

imgC = Image.new("RGB", (width, height))
imgM = Image.new("RGB", (width, height))
imgY = Image.new("RGB", (width, height))
imgK = Image.new("RGB", (width, height))

pix = img.load()
for x in xrange(0, width):
  for y in xrange(0, height):

    c4, m4, y4, k4 = rgb_to_cmyk(
      pix[x, y][0],
      pix[x, y][1],
      pix[x, y][2]
    )

    c3 = min(100, (c4*(100 - k4))/100 + k4);
    m3 = min(100, (m4*(100 - k4))/100 + k4);
    y3 = min(100, (y4*(100 - k4))/100 + k4);

    colorC = 255 - (c3*255)/100, 255, 255, 255
    colorM = 255, 255 - (m3*255)/100, 255, 255
    colorY = 255, 255, 255 - (y3*255)/100, 255
    colorK = 255 - (k4*255)/100, 255 - (k4*255)/100, 255 - (k4*255)/100, 255

    imgC.putpixel( (x, y), colorC )
    imgM.putpixel( (x, y), colorM )
    imgY.putpixel( (x, y), colorY )
    imgK.putpixel( (x, y), colorK )

imgC.save('brick-house-C.png','png')
imgM.save('brick-house-M.png','png')
imgY.save('brick-house-Y.png','png')
imgK.save('brick-house-K.png','png')
