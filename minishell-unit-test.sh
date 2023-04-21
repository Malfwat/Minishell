#!/bin/bash


TOTAL_TESTS=0
TOTAL_EV_SUCCESS=0 # EV = EXIT_VALUE
TOTAL_OV_SUCCESS=0 # OP = OUTPUT_VALUE

Build_test_environment()
{
	rm -rf .minishell_test_environment
	mkdir .minishell_test_environment
	cd .minishell_test_environment
	mkdir test_files expected_output_dir original_output_dir

	cd test_files
	touch 1 2 3 4
	echo "111" > 1
	echo "222" > 2
	echo "333" > 3
	echo "444" > 4

	cd ..
}

Test()
{
	((TOTAL_TESTS++))
	cd expected_output_dir
	local EXPECTED_EXIT_VALUE="$(echo $?)"
	cd ../original_output_dir
	local PROGRAM_EXIT_VALUE="$(echo $?)"
	cd ..
	local OUTPUTS_DIFFS=$(diff -rq expected_output_dir original_output_dir)

	echo "==== TEST $TOTAL_TESTS ===="
	echo -e "\033[37mcommand: \033[0m\033[4m$1\033[0m"
	# Check that $OUTPUTS_DIFFS is empty => Would mean the test is successful
	if [ -z "$OUTPUTS_DIFFS" ]; then
		echo -e "Output: \033[92mOK\033[0m"
	((TOTAL_OV_SUCCESS++))
	else
		echo -e "Output: \033[91mKO\033[0m"
		echo "=========== DIFFS ==========="
		echo $(echo "$OUTPUTS_DIFFS" | sed -e 's/\n/\n\t/g')
		echo "============================="
	fi

	# Check exit value
	if [ "$PROGRAM_EXIT_VALUE" = "$EXPECTED_EXIT_VALUE" ]; then
		echo -e "Exit value: \033[92mOK\033[0m"
	((TOTAL_EV_SUCCESS++))
	else
		echo -e "Exit value: \033[91mKO\033[0m \n\tO:$EXPECTED_EXIT_VALUE\n\tY:$PROGRAM_EXIT_VALUE"
	fi
}

Final_result()
{
	echo "correct exit values:$TOTAL_EV_SUCCESS/$TOTAL_TESTS"
	echo "correct output values:$TOTAL_OV_SUCCESS/$TOTAL_TESTS"
}

Build_test_environment
Test "echo test"
Test "cat a"
Final_result
rm -rf .minishell_test_environment