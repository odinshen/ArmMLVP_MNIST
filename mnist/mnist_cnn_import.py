#
# Copyright (C) 2017 ARM Limited. All rights reserved.
#
from arm_ds.debugger_v1 import Debugger
from arm_ds.debugger_v1 import DebugException
import json

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

def loadParams(filepath):

    fp = open(filepath, 'r')
    jsonObj = json.load(fp)
    fp.close()

    return jsonObj


def storeParams(ec, store_adr, params, biases_max, weights_max0, weights_max1, weights_max2, weights_max3):

    adr = store_adr

    # biases
    start_adr = adr
    for i in range(0, biases_max):
        dscmd = 'memory set_typed S:0x%08x (float)  (float)(%.18f)' % (adr, params['biases'][i])
        ec.executeDSCommand(dscmd)
        adr += 0x4
    print ' biases  S:0x%08x - S:0x%08x' % (start_adr, adr)

    # weights
    start_adr = adr
    if weights_max2 != 0:  # biases[], weights[][][][]
        for i in range(0, weights_max0):
            for j in range(0, weights_max1):
                for k in range(0, weights_max2):
                    for l in range(0, weights_max3):
                        dscmd = 'memory set_typed S:0x%08x (float)  (float)(%.18f)' % (adr, params['weights'][i][j][k][l])
                        ec.executeDSCommand(dscmd)
                        adr += 0x4
    else:                 # biases[], weights[][]
        for i in range(0, weights_max0):
            for j in range(0, weights_max1):
                dscmd = 'memory set_typed S:0x%08x (float)  (float)(%.18f)' % (adr, params['weights'][i][j])
                ec.executeDSCommand(dscmd)
                adr += 0x4
    print ' weights S:0x%08x - S:0x%08x' % (start_adr, adr)

    return adr


def main():

    # Obtain the first execution context
    debugger = Debugger()
    ec = debugger.getCurrentExecutionContext()

    # Assumed the image file is located on the project folder
    imgDir = getImageDir(ec)

    #--- load parameters ---
    lay0_params = loadParams(imgDir + 'mnist_cnn_train121_params_layer0.json')
    lay2_params = loadParams(imgDir + 'mnist_cnn_train121_params_layer2.json')
    lay6_params = loadParams(imgDir + 'mnist_cnn_train121_params_layer6.json')
    lay8_params = loadParams(imgDir + 'mnist_cnn_train121_params_layer8.json')

    #--- store parameters ---
    s_adr = 0x80100000

    print 'Keras_lay[0]'
    print 'biases{Array[16]}, weights{Array[5][5][1][16]}'
    adr = storeParams(ec, s_adr, lay0_params, 16, 5, 5, 1, 16)

    print 'Keras_lay[2]'
    print 'biases{Array[32]}, weights{Array[5][5][16][32]}'
    adr = storeParams(ec, adr, lay2_params, 32, 5, 5, 16, 32)

    print 'Keras_lay[6]'
    print 'biases{Array[128]}, weights{Array[512][128]}'
    adr = storeParams(ec, adr, lay6_params, 128, 512, 128, 0, 0)

    print 'Keras_lay[8]'
    print 'biases{Array[10]}, weights{Array[128][10]}'
    e_adr = storeParams(ec, adr, lay8_params, 10, 128, 10, 0, 0)

    #--- save stored parameters to binary file ---
    dscmd = 'dump binary memory "ds5_mnist_params_v2.bin" S:0x%08x S:0x%08x' % (s_adr, e_adr)
    ec.executeDSCommand(dscmd)

if __name__ == '__main__':
    main() 
