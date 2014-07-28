#!/bin/bash
#===============================================================================
#
#          FILE:  getStat.sh
#
#         USAGE:  ./getStat.sh nmbOfRuns [dataFile]
#
#   DESCRIPTION:  get statistics of bad/good suduku launches
#
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR:  Alexey Radkov (lyokha)
#       COMPANY:  
#       VERSION:  1.0
#       CREATED:  17.02.2008 21:38:13 MSK
#      REVISION:  ---
#===============================================================================

usage()
{
    echo "usage: `basename $0` nmbOfLaunches [datafile]"
}

logfile=./getStat.log
good=0
bad=0
sudokubin=./sudokuRapidConsole

if [ -z "$1" ] ; then
    usage
    exit
else
    nmbOfLaunches=$1
    if [ -n "$2" ] ; then
        dataFile=$2
    fi
fi

: > $logfile
for (( i = 0; i < $nmbOfLaunches; i++ )) ; do
    $sudokubin $dataFile >> $logfile 2>&1
    if [ $? -eq 0 ] ; then
        (( good++ ))
    else
        (( bad++ ))
    fi
    #sleep 1
done

echo "$nmbOfLaunches instances were launched" | tee -a $logfile
echo "$good good, $bad bad" | tee -a $logfile

