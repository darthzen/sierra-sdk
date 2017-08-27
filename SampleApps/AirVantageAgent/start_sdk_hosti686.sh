sudo ./bin/start_sdkhosti686 ../../build/bin/hosti686/slqssdk &>debug.log
#sudo bin/start_sdkhosti686 ../../build/bin/hosti686/slqssdk 


if pidof -x agent >/dev/null; then
echo "Agent has already started"
else
echo "No Agent is running, start Agent now" 
sudo ./build.default/runtime/start.sh &
fi
