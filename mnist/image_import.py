#
# Copyright (C) 2017 ARM Limited. All rights reserved.
#
from arm_ds.debugger_v1 import Debugger
from arm_ds.debugger_v1 import DebugException
from javax.imageio import ImageIO


def parseDScmdRetStr(retval):
    # extract the path from '$10 = "path"' like format
    s = retval.find("\"")    # detect first character of "
    e = retval.rfind("\"")    # detect char " from the end
    return retval[s+1:e]

def getImageDir(ec):
    # get image folder from CurrentExecutionContext
    idir = ec.executeDSCommand('print $idir')
    dir = parseDScmdRetStr(idir)
    # replace \ to /
    dir = dir.replace('\\', '/')
    return dir

def loadImage(filepath):

    fp = open(filepath, 'r')
    image = ImageIO.read(fp)
    fp.close()
    
    return image

def storeImage(ec, store_adr, image, result):

    adr = store_adr

    # grayscale color image
    start_adr = adr
    for x in range(0, image.getWidth()):
        for y in range(0, image.getHeight()):
            pixel = image.getRGB(y, x) & 0xFF   # grayscale and transform to [row][col]
            dscmd = 'memory set_typed S:0x%08x (unsigned int) %d' % (adr, pixel)
            ec.executeDSCommand(dscmd)
            adr += 0x4

    print ' image  S:0x%08x - S:0x%08x' % (start_adr, adr)

    # Set the result at end of area (0x17FFF)
    adr = start_adr + 0xFFC
    pixel = result & 0xFF
    dscmd = 'memory set_typed S:0x%08x (unsigned int) %d' % (adr, pixel)
    ec.executeDSCommand(dscmd)

    # Force "selected" test mode
#    adr = 0x801FFFFC
    adr = 0x800FFFFC
    pixel = 0xff
    dscmd = 'memory set_typed S:0x%08x (unsigned int) %d' % (adr, pixel)
    ec.executeDSCommand(dscmd)

    return adr


def main():

    # Obtain the first execution context
    debugger = Debugger()
    ec = debugger.getCurrentExecutionContext()

    # Assumed the image file is located on the project folder
    imgDir = getImageDir(ec)
    image = loadImage(imgDir + '/mnist/test_8.jpg')    
    #--- store image ---
    adr = 0x80150000     # TESTDATA 0x80370000 to 0x80370C40 (size 0xC40)
    adr = storeImage(ec, adr, image, 8)

'''
    imgDir = getImageDir(ec)
    image = loadImage(imgDir + '/mnist/test_3.jpg')
    adr = 0x80151000
    adr = storeImage(ec, adr, image, 3)

    imgDir = getImageDir(ec)
    image = loadImage(imgDir + '/mnist/test_4.jpg')
    adr = 0x80152000
    adr = storeImage(ec, adr, image, 4)

    imgDir = getImageDir(ec)
    image = loadImage(imgDir + '/mnist/test_7.jpg')
    adr = 0x80153000
    adr = storeImage(ec, adr, image, 7)

    imgDir = getImageDir(ec)
    image = loadImage(imgDir + '/mnist/test_1.jpg')
    adr = 0x80154000
    adr = storeImage(ec, adr, image, 1)

    imgDir = getImageDir(ec)
    image = loadImage(imgDir + '/mnist/test_6.jpg')
    adr = 0x80155000
    adr = storeImage(ec, adr, image, 6)
'''

if __name__ == '__main__':
    main() 
