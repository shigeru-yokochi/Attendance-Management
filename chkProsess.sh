#! /bin/bash
line=`/bin/ps -elf | /bin/grep  Attendance-Management | /bin/grep -v grep | /bin/grep -v chkProsess.sh | /usr/bin/wc -l`
if [ $line -ne 2 ]; then
    echo 2 > /tmp/alm.tmp
fi
