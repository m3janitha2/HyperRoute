./${{github.workspace}}/build/examples/trading_system/application/sim_server_b "127.0.0.1" "8086" "" "0" "tcp" "server" "serverB" "10" > ${{github.workspace}}/sim_server_b.log 2>&1 &
sleep 1
./${{github.workspace}}/build/examples/trading_system/application/app_trading_system "" > ${{github.workspace}}/app_trading_system.log 2>&1 &
sleep 1
./${{github.workspace}}/build/examples/trading_system/application/sim_client_a "" "0" "127.0.0.1" "8081" "tcp" "client" "clientA" "100" > ${{github.workspace}}/sim_client_a.log 2>&1 &
sleep 15
pgrep -f sim_server_b | xargs kill -9
pgrep -f app_trading_system | xargs kill -9
pgrep -f sim_client_b | xargs kill -9
cat ${{github.workspace}}/app_trading_system.log