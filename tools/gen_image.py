#!/usr/bin/env python

import os
import sys

#------------------------------------------------------------------------------
def error(text):
  print 'Error: %s' % text
  sys.exit(1)

#------------------------------------------------------------------------------
def usage():
  print 'Usage: gen_image <input_file> <output_file>'
  sys.exit(0)

#------------------------------------------------------------------------------
def crc_ccitt(data):
  crc = 0
  for d in data:
    d ^= crc & 0xff
    d ^= (d << 4) & 0xff
    crc = (((d << 8) | ((crc & 0xff00) >> 8)) ^ ((d >> 4) & 0xff) ^ (d << 3)) & 0xffff
  return crc

#------------------------------------------------------------------------------
def main():
  if len(sys.argv) != 3:
    usage()

  if not os.path.exists(sys.argv[1]):
    error('input file does not exists')

  if not os.path.isfile(sys.argv[1]):
    error('invalid input file')

  data = [ord(d) for d in open(sys.argv[1], 'rb').read()]

  if len(data) > 3072:
    error('input file is too big')

  while len(data) % 64:
    data += [0xff]

  try:
    f = open(sys.argv[2], 'w')
    f.write('%s\r\n' % ('?' * 16))
    f.write('$;\r\n')

    for chunk in [data[i:i+64] for i in range(0, len(data), 64)]:
      f.write('#%s;\r\n' % ''.join('%02x' % d for d in chunk))
      f.write('%s\r\n' % ('?' * 130))

    f.write('%%%04x;\r\n' % crc_ccitt(data))

  except Exception, msg:
    error(msg)

#------------------------------------------------------------------------------
main()

