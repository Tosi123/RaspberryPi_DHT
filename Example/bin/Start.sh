#!/bin/bash
PRO_NM="Temperature"
pid_chk=`ps -ef |grep "${PRO_NM}" |grep -v grep |wc -l`

if [[ ${pid_chk} -ne  0 ]]; then
echo "이미 프로세스가 실행중 입니다."
exit
fi

nohup ./Temperature &

if [[ 0 -eq $? ]];then
sleep 3
pid=`ps -ef |grep "${PRO_NM}" |grep -v grep |awk '{print $2}'`
if [[ -n ${pid} ]];then
echo "프로세스 정상 실행 되었습니다!! (PID:${pid})"
else
echo "프로세스 실행 실패!!"
fi
else
echo "프로세스 실행 실패!!"
fi

