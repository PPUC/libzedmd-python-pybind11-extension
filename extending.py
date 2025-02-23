#! /usr/bin/env python

import ctypes
import pathlib
import time

from PIL import Image, ImageSequence


# Load ZeDMD library using ctypes
libzedmd = ctypes.CDLL(str(pathlib.Path(__file__).parent.resolve()) + '/extern/libzedmd/build/libzedmd.so')


from build.extending import ZeDMD_ext

ZeDMD = ZeDMD_ext()
# ZeDMD.SetBrightness(7)
# ZeDMD.SetRGBOrder(3) 
# ZeDMD.SetUsbPackageSize(1024)
# ZeDMD.SetPanelMinRefreshRate(60)
# ZeDMD.SaveSettings()

image = Image.open("test.jpg")
image = image.convert('RGB')
image_data = [x for RGB in image.getdata() for x in RGB] # flatten the list using a listcomp with two 'for'
ZeDMD.RenderRgb888(image_data)
time.sleep(3)

imageGif = Image.open("animation.gif")
for frame in ImageSequence.Iterator(imageGif):
  frame = frame.convert('RGB')
  frame_data = [x for RGB in frame.getdata() for x in RGB]
  ZeDMD.RenderRgb888(frame_data)
  time.sleep(0.016)

# ZeDMD.WhiteTest()
# ZeDMD.RenderTest()
# ZeDMD.LedTest() 

ZeDMD.Close()
