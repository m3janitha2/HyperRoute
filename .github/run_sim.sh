$GITHUB_WORKSPACE/build/examples/trading_system/application/sim_server_b "127.0.0.1" "8086" "" "0" "tcp" "server" "serverB" "10" > $GITHUB_WORKSPACE/sim_server_b.log 2>&1 &
$GITHUB_WORKSPACE/build/examples/trading_system/application/sim_server_b "127.0.0.1" "8088" "" "0" "tcp" "server" "serverC" "10" > $GITHUB_WORKSPACE/sim_server_b.log 2>&1 &
sleep 1
$GITHUB_WORKSPACE/build/examples/trading_system/application/app_trading_system "$GITHUB_WORKSPACE/examples/trading_system/config/config.xml" > $GITHUB_WORKSPACE/app_trading_system.log 2>&1 &
sleep 1
$GITHUB_WORKSPACE/build/examples/trading_system/application/sim_client_a "" "0" "127.0.0.1" "8081" "tcp" "client" "clientA" "500" > $GITHUB_WORKSPACE/sim_client_a.log 2>&1 &
sleep 15
pgrep -f sim_server_b | xargs kill -9
pgrep -f app_trading_system | xargs kill -9
pgrep -f sim_client_b | xargs kill -9
cat $GITHUB_WORKSPACE/app_trading_system.log