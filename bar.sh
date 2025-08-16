#!/bin/bash
# Terminate already running bar instances
dwmbar 2>&1 | tee -a /tmp/dwmbar.log & disown
#killall -q dwmblocks
#dwmblocks 2>&1 | tee-a /tmp/dwmblocks.log & disown
#killall -q polybar
# If all your bars have ipc enabled, you can also use
# polybar-msg cmd quit

# Launch Polybar, using default config location ~/.config/polybar/config.ini
#polybar mybar 2>&1 | tee -a /tmp/polybar.log & disown
#3echo "Polybar launched..."

