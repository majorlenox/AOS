#!/bin/bash
PID=$1
kill -USR1 $PID
sleep 1
kill -USR2 $PID
kill -USR2 $PID
kill -USR2 $PID
kill -TERM $PID
kill -TERM $PID
kill -USR2 $PID
kill -CONT $PID
sleep 1
kill -SIGINT $PID
