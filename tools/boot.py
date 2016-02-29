#!/usr/bin/env python

import os
import sys
import serial

#------------------------------------------------------------------------------
def error(text):
  print 'Error: %s' % text
  sys.exit(1)

#------------------------------------------------------------------------------
def usage():
  print 'Usage: boot <port> <input_file>'
  sys.exit(0)

#------------------------------------------------------------------------------
def main():
  if len(sys.argv) != 3:
    usage()

  if not os.path.exists(sys.argv[2]):
    error('input file does not exists')

  if not os.path.isfile(sys.argv[2]):
    error('invalid input file')

  try:
    port = serial.Serial(sys.argv[1], 38400, timeout=1)
    port.write(open(sys.argv[2], 'rb').read())
    port.close()
  except serial.serialutil.SerialException, msg:
    error(msg)

#------------------------------------------------------------------------------
main()

