#! /bin/bash

source ~/.profile

this=_wifi
icon_color="^c#000080^^b#3870560x88^"
text_color="^c#000080^^b#3870560x99^"
signal=$(echo "^s$this^" | sed 's/_//')

update() {
	# method 1
	# network_res="$(ping -c5 -i .1 www.baidu.com | grep packets | cut -d, -f3 | tr -d ' ' | cut -d% -f1)"
	network_res="$(ifconfig -v wlan0| grep inet | grep -v inet6 | tr -s ' ' | cut -d ' ' -f3)"
	# if the network_res is null ,then the network is bad

	[ $network_res ] && network_res=0
    if  [ "$network_res" -eq 0 ]; then wifi_icon="直";
    else wifi_icon=" 睊 ";fi
    wifi_text=$(nmcli device | grep -w "connected" | grep -v '(externally)' | tr -s ' '| cut -d' ' -f4)
    [ "$wifi_text" = "" ] && wifi_text="Error"
    icon=" $wifi_icon "
    text=" $wifi_text "
    sed -i '/^export '$this'=.*$/d' $DWM/statusbar/temp
    printf "export %s='%s%s%s%s%s'\n" $this "$signal" "$text_color" "$icon" "$text_color" "$text" >> $DWM/statusbar/temp
}

notify() {
    update
    connect=$(nmcli | grep connected | awk '{print $4}' | head -n 1)
    device=$(nmcli | grep connected | cut -d: -f1 | head -n 1)
    text="设备: $device\n连接: $connect"
    [ "$connect" = "" ] && text="未连接到网络"
    notify-send -r 9527 "$wifi_icon Wifi" "\n$wifi_text"
}

call_nm() {
    pid1=`ps aux | grep 'st -t statusutil' | grep -v grep | awk '{print $2}'`
    pid2=`ps aux | grep 'st -t statusutil_nm' | grep -v grep | awk '{print $2}'`
    mx=`xdotool getmouselocation --shell | grep X= | sed 's/X=//'`
    my=`xdotool getmouselocation --shell | grep Y= | sed 's/Y=//'`
    kill $pid1 && kill $pid2 || st -t statusutil_nm -g 60x25+$((mx - 240))+$((my + 20)) -c noborder -e 'nmtui-connect'
}

click() {
    case "$1" in
        L) notify ;;
        R) call_nm ;;
    esac
}

case "$1" in
    click) click $2 ;;
    notify) notify ;;
    *) update ;;
esac
