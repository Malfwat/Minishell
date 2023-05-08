#!/bin/bash

# Clear the screen and hide the cursor
echo -e "\033[2J\033[?25l"

# Get the terminal size
R=`tput lines`
C=`tput cols`
REVERSED_BANNER=`tac "minishell_banner.txt"`

# Decrease the number of rows by 1
: $[R--]
echo -e -n "\033[$R;0H\033[38;5;110mPrompt de notre Minishell ici \033[38;5;9m>\033[0m"

: $[R--]

while true; do
	(
	e=echo\ -e
	s=sleep
	j=$[RANDOM % C]
	d=$[RANDOM % R]
	random_chance=$((RANDOM % 100 + 1))

	for i in `eval $e {1..$R}`; do
		c=`printf '%c' $(shuf -n1 -e m i n i s h e l l M I N I S H E L L ═ ║ ╔ ╗ ╚ ╝ █ ░)`
		p_char="\033[$[i-1];${j}H\033[32m$c"
		curr_char="\033[$i;${j}H\033[37m$c"
		char_in_banner=$(awk -v row=$(($R - $i + 1)) -v col=$j 'BEGIN{FS=""; OFS="";} NR==row {printf "%s", $col; exit}' <<< "$REVERSED_BANNER")

		if [[ -n "$char_in_banner" && "$char_in_banner" != " " ]] && [ $random_chance -le 100 ]; then
			p_char="\033[$[i-1];${j}H\033[32m$char_in_banner"
			curr_char="\033[$i;${j}H\033[37m$char_in_banner"
			$e $p_char$curr_char
		else
			char_in_banner=$(tac "minishell_banner.txt" | awk -v row=$(($R - $i + 1 - 1)) -v col=$j 'BEGIN{FS=""; OFS="";} NR==row {printf "%s", $col; exit}')
			if [[ -n "$char_in_banner" && "$char_in_banner" != " " ]] && [ $random_chance -le 100 ]; then
				$e $curr_char
			else
				$e $p_char$curr_char
			fi
		fi

		if [ $i -ge $d ]; then
			char_in_banner=$(tac "minishell_banner.txt" | awk -v row=$(($R - $i - $d + 1)) -v col=$j 'BEGIN{FS=""; OFS="";} NR==row {printf "%s", $col; exit}')
			if [[ -n "$char_in_banner" && "$char_in_banner" != " " ]] && [ $random_chance -le 100 ]; then
				:
			else
				$e "\033[$[i-d];${j}H "
			fi
		fi
		$s 0.06
	done

	for i in `eval $e {$[i-d]..$R}`; do
		char_in_banner=$(tac "minishell_banner.txt" | awk -v row=$(($R - $i + 1)) -v col=$j 'BEGIN{FS=""; OFS="";} NR==row {printf "%s", $col; exit}')
		if [[ -n "$char_in_banner" && "$char_in_banner" != " " ]] && [ $random_chance -le 100 ]; then
			:
		else
			echo -e "\033[$i;${j}f "
			$s 0.06
		fi
	done
	) & sleep 0.05
done

# ███╗░░░███╗██╗███╗░░██╗██╗░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░
# ████╗░████║██║████╗░██║██║██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░
# ██╔████╔██║██║██╔██╗██║██║╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░
# ██║╚██╔╝██║██║██║╚████║██║░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░
# ██║░╚═╝░██║██║██║░╚███║██║██████╔╝██║░░██║███████╗███████╗███████╗
# ╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝