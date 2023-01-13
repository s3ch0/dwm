# rofi -show 自定义 -modi "自定义:~/rofi.sh"
#   1: 上述命令可调用rofi.sh作为自定义脚本
#   2: 将打印的内容作为rofi的选项
#   3: 每次选中后 会用选中项作为入参再次调用脚本
#   4: 当没有输出时 整个过程结束

# !!! 确保所有item没有重复的 !!!
main_menu_item[1]="set wallpaper"        ; main_cmd[1]='~/scripts/wallpaper/wp-change_me.sh; main_menu;'
main_menu_item[2]="toggle some server"   ; main_cmd[2]='server_menu'
main_menu_item[3]="bluetooth"            ; main_cmd[3]='bluetooth_menu'
main_menu_item[4]="open last screenshot" ; main_cmd[4]='~/scripts/app-starter.sh open_last_screenshot'
main_menu_item[5]="set default wallpaper"; main_cmd[5]='~/scripts/wp-default.sh'
main_menu_item[6]="auto change the wallpaper"; main_cmd[6]='~/scripts/wallpaper/wp-autochange.sh >> /dev/null &'
main_menu_item[7]="close auto change the wallpaper"; main_cmd[7]='ps -axf| pgrep wp-autochange | xargs kill -9 >> /dev/null'

server_menu_item[1]="v2ray"             ; server_cmd[1]='killall qv2ray ||~/scripts/app-starter.sh qv2ray'

server_menu_item[2]="picom"              ; server_cmd[2]='killall picom || ~/scripts/app-starter.sh picom'
server_menu_item[3]="easyeffects"        ; server_cmd[3]='killall easyeffects || ~/scripts/app-starter.sh easyeffects'
server_menu_item[4]="bilibili"           ; server_cmd[4]='~/workspace/go/src/bilibili/bin/run.sh'
server_menu_item[5]="auto screen"        ; server_cmd[5]='~/scripts/set-screen.sh toggle_auto'

bluetooth_menu_item[1]="toggle A2DP/HSP/HFP" ; bluetooth_cmd[1]='~/scripts/app-starter.sh toggle_hp_sink'

bluetooth_menu_item[2]="connect keyboard"     ; bluetooth_cmd[2]='~/scripts/bluetooth.sh k1 >> /dev/null &'
bluetooth_menu_item[3]="connect hp2"     ; bluetooth_cmd[3]='~/scripts/bluetooth.sh hp2 >> /dev/null &'
bluetooth_menu_item[4]="disconnect keyboard"  ; bluetooth_cmd[4]='~/scripts/bluetooth.sh dk1 >> /dev/null &'
bluetooth_menu_item[5]="disconnect hp2"  ; bluetooth_cmd[5]='~/scripts/bluetooth.sh dhp2 >> /dev/null &'

main_menu() {
    echo -e "\0prompt\x1fmenu"
    for item in "${main_menu_item[@]}"; do
        echo "$item"
    done
}
server_menu() {
    echo -e "\0prompt\x1fserver"
    for item in "${server_menu_item[@]}"; do
        echo "$item"
    done
}
bluetooth_menu() {
    echo -e "\0prompt\x1fbluetooth"
    for item in "${bluetooth_menu_item[@]}"; do
        echo "$item"
    done
}

[ ! "$*" ] && main_menu
for i in "${!main_menu_item[@]}"; do
    [ "$*" = "${main_menu_item[$i]}" ] && eval "${main_cmd[$i]}"
done
for i in "${!server_menu_item[@]}"; do
    [ "$*" = "${server_menu_item[$i]}" ] && eval "${server_cmd[$i]}"
done
for i in "${!bluetooth_menu_item[@]}"; do
    [ "$*" = "${bluetooth_menu_item[$i]}" ] && eval "${bluetooth_cmd[$i]}"
done
