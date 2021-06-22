function rsh_cli(){
	COMMAND=$(echo $* | sed 's/\(.-g[ ]\([^ ]*\)\)//g')
    echo $COMMAND
$cli_cmd << EOF
    ${COMMAND} 
    quit
EOF
}

function run_cmd() {
    echo $* | grep -q "[ ]-g[ ]"
    if [ $? -eq 0 ]; then
        cmd1=$(echo "$* " | sed "s/.*-g \([^ ]*\) .*/\1/")
        rsh_cli $* | egrep "${cmd1}| data |^$"
    else
        rsh_cli $* | grep -v '<L3-apollo>'
    fi
}

run_cmd $*