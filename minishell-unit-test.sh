#!/bin/bash


TOTAL_TESTS=0
TOTAL_EV_SUCCESS=0 # EV = EXIT_VALUE
TOTAL_OV_SUCCESS=0 # OP = OUTPUT_VALUE

Build_test_environment()
{
	rm -rf .minishell_test_environment
	mkdir .minishell_test_environment
	cd .minishell_test_environment
	mkdir expected_output_dir expected_output_dir/test_files original_output_dir original_output_dir/test_files

	cd expected_output_dir/test_files
	touch i_pipe_0 i_pipe_1 i_pipe_2 i_pipe_3 i_pipe_4 i_pipe_5 i_pipe_6 i_pipe_7
	ls i_pipe_* | xargs -I {} bash -c "echo "test" > {}"
	touch a_pipe_0 a_pipe_1 a_pipe_2 a_pipe_3 a_pipe_4 a_pipe_5 a_pipe_6 a_pipe_7
	ls a_pipe_* | xargs -I {} bash -c "echo "test_{}_" > {}"
	cd ../..
	cd original_output_dir/test_files
	touch i_pipe_0 i_pipe_1 i_pipe_2 i_pipe_3 i_pipe_4 i_pipe_5 i_pipe_6 i_pipe_7
	ls i_pipe_* | xargs -I {} bash -c "echo "test" > {}"
	touch a_pipe_0 a_pipe_1 a_pipe_2 a_pipe_3 a_pipe_4 a_pipe_5 a_pipe_6 a_pipe_7
	ls a_pipe_* | xargs -I {} bash -c "echo "test_{}_" > {}"
	cd ../..
}

Test()
{
	((TOTAL_TESTS++))
	cd expected_output_dir
	bash --posix -c "$1" > output
	local EXPECTED_EXIT_VALUE="$(echo $?)"
	cd ../original_output_dir
	$1 > output
	local PROGRAM_EXIT_VALUE="$(echo $?)"
	cd ..
	local OUTPUTS_DIFFS=$(diff --color=always -ru expected_output_dir original_output_dir)

	echo -e "\033[38;5;208m==== TEST $TOTAL_TESTS ====\033[0m"
	echo -e "\033[37mcommand: \033[0m\033[4m$1\033[0m"
	# Check that $OUTPUTS_DIFFS is empty => Would mean the test is successful
	if [ -z "$OUTPUTS_DIFFS" ]; then
		echo -e "Output: \033[92mOK\033[0m"
		((TOTAL_OV_SUCCESS++))
	else
		echo -e "Output: \033[91mKO\033[0m"
		echo "=========== DIFFS ==========="
		echo "$OUTPUTS_DIFFS" | sed -e 's/\n/\n\t/g'
		echo "============================="
	fi

	# Check exit value
	if [ "$PROGRAM_EXIT_VALUE" = "$EXPECTED_EXIT_VALUE" ]; then
		echo -e "Exit value: \033[92mOK\033[0m"
		((TOTAL_EV_SUCCESS++))
	else
		echo -e "Exit value: \033[91mKO\033[0m \n\tO:$EXPECTED_EXIT_VALUE\n\tY:$PROGRAM_EXIT_VALUE"
	fi

	if [ "$TOTAL_EV_SUCCESS" != "$TOTAL_TESTS" ] || [ "$TOTAL_OV_SUCCESS" != "$TOTAL_TESTS" ]; then
		exit
	fi
	# rm -rf expected_output_dir/* original_output_dir/*
	rm -rf expected_output_dir/output original_output_dir/output
}

Final_result()
{
	echo -e "\033[38;5;208m==== RESULT ====\033[0m"
	echo "correct exit values:$TOTAL_EV_SUCCESS/$TOTAL_TESTS"
	echo "correct output values:$TOTAL_OV_SUCCESS/$TOTAL_TESTS"
}

rm -rf .minishell_test_environment
Build_test_environment

############################################################################
#                                                                          #
#                                                                          #
#                             NO OPERATORS                                 #
#                                                                          #
#                                                                          #
############################################################################
Test "echo test"
Test "inexistant_command"
############################################################################
#                                                                          #
#                                                                          #
#                          PIPE OPERATORS TESTS                            #
#                                                                          #
#                                                                          #
############################################################################
Test "echo lol | echo test1"
Test "inexistant_command lol | echo test"
Test "cat inexistant_file | echo ergerg"
Test "inexistant_command | inexistant_command"
Test "echo test | cat"
Test "echo test | cat | rev"
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 >test_files/o_pipe_2 | echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 | echo test"
Test "cat inexistant_file | echo ergerg > test_files/o_pipe_5"
Test "inexistant_command | > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 | echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol< test_files/i_pipe_4 | echo test"
Test "cat inexistant_file | echo ergerg < test_files/i_pipe_5"
Test "inexistant_command | < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol>> test_files/a_pipe_1 >> test_files/a_pipe_2 | echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4 | echo test"
Test "cat inexistant_file | echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command | >>test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 000 
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 | echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 | echo test"
Test "cat inexistant_file | echo ergerg > test_files/o_pipe_5"
Test "inexistant_command | > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 | echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol< test_files/i_pipe_4 | echo test"
Test "cat inexistant_file | echo ergerg < test_files/i_pipe_5"
Test "inexistant_command | < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 | echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4 | echo test"
Test "cat inexistant_file | echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command | >> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 666
Test "> o_pipe_1"
Test "> o_pipe_1 > o_pipe_2"
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 | echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 | echo test"
Test "cat inexistant_file | echo ergerg > test_files/o_pipe_5 | egre > test_files/o_pipe_51"
Test "inexistant_command | > test_files/o_pipe_6 inexistant_command | echo test"
Test "< i_pipe_1"
Test "< i_pipe_1 > o_pipe_2"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 | echo test1 < test_files/i_pipe_3 | < i_pipe_1"
Test "inexistant_command lol < test_files/i_pipe_4 | echo test | "
Test "cat inexistant_file | echo ergerg < test_files/i_pipe_5 | | echo test"
Test "inexistant_command | < test_files/i_pipe_6 inexistant_command | cat inexistant_file"
Test ">> a_pipe_1"
Test ">> a_pipe_1 >> a_pipe_2"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 | echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4| echo test"
Test "cat inexistant_file | echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command | >> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 000 
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 | echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 |echo test"
Test "cat inexistant_file | echo ergerg > test_files/o_pipe_5"
Test "inexistant_command | > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 | echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol < test_files/i_pipe_4 | echo test"
Test "cat inexistant_file | echo ergerg < test_files/i_pipe_5"
Test "inexistant_command | < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 | echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >>test_files/a_pipe_4 |echo test"
Test "cat inexistant_file | echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command|>> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 666

############################################################################
#                                                                          #
#                                                                          #
#                          AND OPERATORS TESTS                             #
#                                                                          #
#                                                                          #
############################################################################

Test "echo lol && echo test1"
Test "inexistant_command lol && echo test"
Test "cat inexistant_file && echo ergerg"
Test "inexistant_command && inexistant_command"
Test "echo test && echo lol"
Test "echo test && echo lol && echo 434343rgrg"
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 >test_files/o_pipe_2 && echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 && echo test"
Test "cat inexistant_file && echo ergerg > test_files/o_pipe_5"
Test "inexistant_command && > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 && echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol< test_files/i_pipe_4 && echo test"
Test "cat inexistant_file && echo ergerg < test_files/i_pipe_5"
Test "inexistant_command && < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol>> test_files/a_pipe_1 >> test_files/a_pipe_2 && echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4 && echo test"
Test "cat inexistant_file && echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command && >>test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 000 
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 && echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 && echo test"
Test "cat inexistant_file && echo ergerg > test_files/o_pipe_5"
Test "inexistant_command && > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 && echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol< test_files/i_pipe_4 && echo test"
Test "cat inexistant_file && echo ergerg < test_files/i_pipe_5"
Test "inexistant_command && < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 && echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4 && echo test"
Test "cat inexistant_file && echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command && >> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 666
Test "> o_pipe_1"
Test "> o_pipe_1 > o_pipe_2"
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 && echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 && echo test && cat"
Test "cat inexistant_file && echo ergerg > test_files/o_pipe_5 && egre > test_files/o_pipe_51"
Test "inexistant_command && > test_files/o_pipe_6 inexistant_command && echo test"
Test "< i_pipe_1"
Test "< i_pipe_1 > o_pipe_2"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 && echo test1 < test_files/i_pipe_3 && < i_pipe_1"
Test "inexistant_command lol < test_files/i_pipe_4 && echo test && "
Test "cat inexistant_file && echo ergerg < test_files/i_pipe_5 && && echo test"
Test "inexistant_command && < test_files/i_pipe_6 inexistant_command && cat inexistant_file"
Test ">> a_pipe_1"
Test ">> a_pipe_1 >> a_pipe_2"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 && echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4&& echo test"
Test "cat inexistant_file && echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command && >> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 000 
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 && echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 &&echo test"
Test "cat inexistant_file && echo ergerg > test_files/o_pipe_5"
Test "inexistant_command && > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 && echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol < test_files/i_pipe_4 && echo test"
Test "cat inexistant_file && echo ergerg < test_files/i_pipe_5"
Test "inexistant_command && < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 && echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >>test_files/a_pipe_4 &&echo test"
Test "cat inexistant_file && echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command&&>> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 666

############################################################################
#                                                                          #
#                                                                          #
#                          SEMICOLON OPERATOR TESTS                        #
#                                                                          #
#                                                                          #
############################################################################

Test "echo lol ; echo test1"
Test "inexistant_command lol ; echo test"
Test "cat inexistant_file ; echo ergerg"
Test "inexistant_command ; inexistant_command"
Test "echo test ; echo 4grgrgr"
Test "echo test; echo grbtgr ; echo tee"
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 >test_files/o_pipe_2 ; echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 ; echo test"
Test "cat inexistant_file ; echo ergerg > test_files/o_pipe_5"
Test "inexistant_command ; > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 ; echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol< test_files/i_pipe_4 ; echo test"
Test "cat inexistant_file ; echo ergerg < test_files/i_pipe_5"
Test "inexistant_command ; < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol>> test_files/a_pipe_1 >> test_files/a_pipe_2 ; echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4 ; echo test"
Test "cat inexistant_file ; echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command ; >>test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 000 
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 ; echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 ; echo test"
Test "cat inexistant_file ; echo ergerg > test_files/o_pipe_5"
Test "inexistant_command ; > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 ; echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol< test_files/i_pipe_4 ; echo test"
Test "cat inexistant_file ; echo ergerg < test_files/i_pipe_5"
Test "inexistant_command ; < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 ; echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4 ; echo test"
Test "cat inexistant_file ; echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command ; >> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 666
Test "> o_pipe_1"
Test "> o_pipe_1 > o_pipe_2"
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 ; echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 ; echo test"
Test "cat inexistant_file ; echo ergerg > test_files/o_pipe_5 ; egre > test_files/o_pipe_51"
Test "inexistant_command ; > test_files/o_pipe_6 inexistant_command ; echo test"
Test "< i_pipe_1"
Test "< i_pipe_1 > o_pipe_2"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 ; echo test1 < test_files/i_pipe_3 ; < i_pipe_1"
Test "inexistant_command lol < test_files/i_pipe_4 ; echo test ; "
Test "cat inexistant_file ; echo ergerg < test_files/i_pipe_5 ; ; echo test"
Test "inexistant_command ; < test_files/i_pipe_6 inexistant_command ; cat inexistant_file"
Test ">> a_pipe_1"
Test ">> a_pipe_1 >> a_pipe_2"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 ; echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4; echo test"
Test "cat inexistant_file ; echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command ; >> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 000 
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 ; echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 ;echo test"
Test "cat inexistant_file ; echo ergerg > test_files/o_pipe_5"
Test "inexistant_command ; > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 ; echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol < test_files/i_pipe_4 ; echo test"
Test "cat inexistant_file ; echo ergerg < test_files/i_pipe_5"
Test "inexistant_command ; < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 ; echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >>test_files/a_pipe_4 ;echo test"
Test "cat inexistant_file ; echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command;>> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 666

############################################################################
#                                                                          #
#                                                                          #
#                            OR OPERATOR TESTS                             #
#                                                                          #
#                                                                          #
############################################################################

Test "echo lol || echo test1"
Test "inexistant_command lol || echo test"
Test "cat inexistant_file || echo ergerg"
Test "inexistant_command || inexistant_command"
Test "echo test || echo cat"
Test "echo test || echo cat || echo rev"
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 >test_files/o_pipe_2 || echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 || echo test"
Test "cat inexistant_file || echo ergerg > test_files/o_pipe_5"
Test "inexistant_command || > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 || echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol< test_files/i_pipe_4 || echo test"
Test "cat inexistant_file || echo ergerg < test_files/i_pipe_5"
Test "inexistant_command || < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol>> test_files/a_pipe_1 >> test_files/a_pipe_2 || echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4 || echo test"
Test "cat inexistant_file || echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command || >>test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 000 
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 || echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 || echo test"
Test "cat inexistant_file || echo ergerg > test_files/o_pipe_5"
Test "inexistant_command || > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 || echo test1 < test_files/i_pipe_3"
Test "inexistant_command lol< test_files/i_pipe_4 || echo test"
Test "cat inexistant_file || echo ergerg < test_files/i_pipe_5"
Test "inexistant_command || < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 || echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4 || echo test"
Test "cat inexistant_file || echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command || >> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 666
Test "> o_pipe_1"
Test "> o_pipe_1 > o_pipe_2"
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 || echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 || echo test"
Test "cat inexistant_file || echo ergerg > test_files/o_pipe_5 || egre > test_files/o_pipe_51"
Test "inexistant_command || > test_files/o_pipe_6 inexistant_command || echo test"
Test "< i_pipe_1"
Test "< i_pipe_1 > o_pipe_2"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 || echo test1 < test_files/i_pipe_3 || < i_pipe_1"
Test "inexistant_command lol < test_files/i_pipe_4 || echo test || "
Test "cat inexistant_file || echo ergerg < test_files/i_pipe_5 || || echo test"
Test "inexistant_command || < test_files/i_pipe_6 inexistant_command || cat inexistant_file"
Test ">> a_pipe_1"
Test ">> a_pipe_1 >> a_pipe_2"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 || echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >> test_files/a_pipe_4|| echo test"
Test "cat inexistant_file || echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command || >> test_files/a_pipe_6 inexistant_command"
ls test_files/*pipe* | xargs chmod 000 
Test "> test_files/o_pipe_1 echo lol > test_files/o_pipe_1 > test_files/o_pipe_2 || echo test1 > test_files/o_pipe_3"
Test "inexistant_command lol > test_files/o_pipe_4 ||echo test"
Test "cat inexistant_file || echo ergerg > test_files/o_pipe_5"
Test "inexistant_command || > test_files/o_pipe_6 inexistant_command"
Test "< test_files/i_pipe_1 echo lol < test_files/i_pipe_1 < test_files/i_pipe_2 || echo test1 < test_files/i_pipe_3" #250
Test "inexistant_command lol < test_files/i_pipe_4 || echo test"
Test "cat inexistant_file || echo ergerg < test_files/i_pipe_5"
Test "inexistant_command || < test_files/i_pipe_6 inexistant_command"
Test ">> test_files/a_pipe_1 echo lol >> test_files/a_pipe_1 >> test_files/a_pipe_2 || echo test1 >> test_files/a_pipe_3"
Test "inexistant_command lol >>test_files/a_pipe_4 ||echo test"
Test "cat inexistant_file || echo ergerg >> test_files/a_pipe_5"
Test "inexistant_command||>> test_files/a_pipe_6 inexistant_command" #257
ls test_files/*pipe* | xargs chmod 666
Final_result
# rm -rf ../minishell_test_environment
