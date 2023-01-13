#!/bin/bash
source ~/.profile

this=_temp
icon_color="^c#4B005B^^b#7E51680x88^"
text_color="^c#4B005B^^b#7E51680x99^"
signal=$(echo "^s$this^" | sed 's/_//')

update() {
	temp_icon="糖"
	# this is normal format temperature value * 1000
	tmp_temp_text=$(cat /sys/class/thermal/thermal_zone0/temp) 
	temperature_text=$(($tmp_temp_text / 1000 ))
    text=" $temperature_text$temp_icon "
	if   [ "$temperature_text" -ge 90 ];then prefix_icon="";
	elif [ "$temperature_text" -ge 75 ];then prefix_icon="";
	elif [ "$temperature_text" -ge 70 ];then prefix_icon="";
	elif [ "$temperature_text" -ge 60 ];then prefix_icon="";
	elif [ "$temperature_text" -ge 0  ];then prefix_icon="";
	else prefix_icon=""; fi
    sed -i '/^export '$this'=.*$/d' $DWM/statusbar/temp
    printf "export %s='%s%s%s%s%s'\n" "$this" "$signal" "$icon_color" " $prefix_icon " "$text_color" "$text" >> $DWM/statusbar/temp
}

notify() {
    texts=""
#    [ "$(sudo docker ps | grep 'v2raya')" ] && texts="$texts\n v2raya 已启动"
    [ "$(bluetoothctl info 88:C9:E8:14:2A:72 | grep 'Connected: yes')" ] && texts="$texts\n WH-1000XM4 已链接"
    [ "$texts" != "" ] && notify-send " Info" "$texts" -r 9527
}

click() {
    case "$1" in
        L) notify; feh --randomize --bg-fill ~/Pictures/wallpapers/*.png ;;
        R) call_menu ;;
    esac
}

case "$1" in
    click) click $2 ;;
    notify) notify ;;
    *) update ;;
esac
