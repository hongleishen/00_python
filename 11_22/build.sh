#!/bin/bash
##export PATH=$PATH:/home/tool/gcc-arm-none-eabi-9-2019-q4-major/bin:/home/tool/gcc-arm-none-eabi-9-2019-q4-major/arm-none-eabi/bin
export PATH=$PATH:/opt/arm-hjimi-linux-gnueabi/bin


#Setup board target if specified by cmdline
unset BOARD_TARGET
BOARD_TARGET=(CLEAN IMI2280_DDR IMI1680_FPGA)
function print_target_nemu()
{
	echo "Board Target Supported:"
	local i=1
	local choice
	for choice in ${BOARD_TARGET[@]}; do
		echo "    $i. $choice"
		i=$(($i+1))
	done
}

if [ "$1" != "" ]; then
	if echo $1 | grep -q '[^0-9]'; then
		BOARD_NAME=`echo "$1" | tr "a-z" "A-Z"`
	else
		BOARD_NAME=${BOARD_TARGET[$1]}
	fi
fi
echo "$BOARD_NAME"
while [ 1 ]; do
	if [ "$BOARD_NAME" = "CLEAN" ] ; then
		make clean
		break
	elif [ "$BOARD_NAME" = "IMI2280_DDR" ] ; then
		echo " IMI2280 DDR Selected"
		make clean all O=output TARGET_BOARD=2280_ddr
		break
	elif [ "$BOARD_NAME" = "IMI1680_FPGA" ] ; then
		echo " IMI1680 FPGA Selected"
		make clean all O=output TARGET_BOARD=1680_fpga
		break
	else
		print_target_nemu
		read -p "Please choose target board: " cho

		if [ -z "$cho" ] ; then
			sel=0
		elif (echo -n $cho | grep -q -e "^[0-9][0-9]*$") ; then
			if [ $cho -le ${#BOARD_TARGET[@]} -a $cho -ge 0 ] ; then
				sel=$cho
			else
				echo
				echo "Invalid choice!: $cho"
				continue
			fi
		else
			echo "Invalid choice!"
			continue
		fi
		BOARD_NAME=${BOARD_TARGET[$sel-1]}
	fi
done


