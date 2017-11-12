#!/usr/bin/env python
#
# Common Library Module
#
from __future__ import print_function  # msg_to_stderr, warning, error, debug, info
import sys                             # msg_to_stderr

import time                    # get_utc_time_in_ms, get_epoch_time
from datetime import datetime  # get_utc_time_in_ms

import os  # silence_stderr


# UTC time in milli-seconds
def get_utc_time_in_ms():
  # http://stackoverflow.com/questions/5998245/get-current-time-in-milliseconds-in-python
#  now = datetime.now()
#  mseconds = time.mktime(now.timetuple()) + now.microsecond * 1e-6
  mseconds = int(round(time.time() * 1000))
  return mseconds


# Get epoch time (seconds)
def get_epoch_time():
  # NOTE: this is in UTC
  # http://stackoverflow.com/questions/4548684/getting-the-time-since-the-epoch
  epoch_time = int(time.time())
  return epoch_time


# Function to print warning & debug MSGs
def warning_msg(*objs):
  print("[WARN]", *objs, file=sys.stderr)

def error_msg(*objs):
  print("[ERROR]", *objs, file=sys.stdout)

def debug_msg(*objs):
  print("[DEBUG]", *objs, file=sys.stderr)

def info_msg(*objs):
  print("[INFO]", *objs, file=sys.stderr)


# http://stackoverflow.com/questions/977840/redirecting-fortran-called-via-f2py-output-in-python/978264
def silence_stderr():
  null_fd = os.open(os.devnull, os.O_RDWR)
  # save old
  save = os.dup(2)
  # put /dev/null fds on 1 and 2
  os.dup2(null_fd, 2)

  # close the temporary fd
#  os.close(null_fd)
