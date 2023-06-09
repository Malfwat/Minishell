#!/bin/bash

TOTAL_TESTS=0
TOTAL_EV_SUCCESS=0 # EV = EXIT_VALUE
TOTAL_OV_SUCCESS=0 # OP = OUTPUT_VALUE
TOTAL_VLG_SUCCESS=0 # VLG = VALGRIND
CWD=$(pwd)

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
	valgrind --track-fds=yes --leak-check=full -s --log-file=".vlg.out" $CWD/minishell -c "$1" > output
	local PROGRAM_EXIT_VALUE="$(echo $?)"
	local VLG_OUTPUT=$(cat .vlg.out)
	local NB_ERROR=$(grep -E "(ERROR.*[1-9].*suppressed.*)" .vlg.out | awk '{print $4}')
	printf "$NB_ERROR"
	local OPEN_FDS="$(grep -E "Open file descriptor" .vlg.out | wc -l)"
	local PARENT_FDS=$(grep -E "<inherited from parent>" .vlg.out | wc -l)
	rm -f .vlg.out
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
	if [ "$PROGRAM_EXIT_VALUE" == "$EXPECTED_EXIT_VALUE" ]; then
		echo -e "Exit value: \033[92mOK\033[0m"
		((TOTAL_EV_SUCCESS++))
	else
		echo -e "Exit value: \033[91mKO\033[0m \n\tO:$EXPECTED_EXIT_VALUE\n\tY:$PROGRAM_EXIT_VALUE"

	fi

	# Check valgrind errors
	if [[ ( "$OPEN_FDS" == "0" || "$OPEN_FDS" == "$PARENT_FDS" ) && !"$NB_ERROR" ]]; then
		echo -e "Valgrind: \033[92mOK\033[0m"
		((TOTAL_VLG_SUCCESS++))
	else
		echo \"$NB_ERROR\"
		echo \"$OPEN_FDS\"
		echo \"$PARENT_FDS\"
		echo -e "Valgrind: \033[91mKO\033[0m \n"
		echo "$VLG_OUTPUT"
	fi


	if [ "$TOTAL_EV_SUCCESS" != "$TOTAL_TESTS" ] || [ "$TOTAL_OV_SUCCESS" != "$TOTAL_TESTS" ] \
	|| [ "$TOTAL_VLG_SUCCESS" != "$TOTAL_TESTS" ]; then
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
	echo "valgrind successes:$TOTAL_VLG_SUCCESS/$TOTAL_TESTS"
}

rm -rf .minishell_test_environment
Build_test_environment

############################################################################
#                                                                          #
#                                                                          #
#                             GENERAL TESTS                                #
#                                                                          #
#                                                                          #
############################################################################
Test "export | adam"
Test "echo test"
Test "echo $"
Test "echo $?"
Test "echo test >/dev/full"
Test "echo test >/dev/null"
Test "ls > /dev/full"
Test "cat /dev/null"
Test "echo \"'$TERM'\""
Test "echo '\"$TERM\"'"
Test "echo $ PATH"
Test "inexistant_command"
Test "echo *"
Test "echo libft/ft_strchr.*"
Test "echo test < inexistant_input"
Test "export bla='test' && echo \$bla"
Test "(export bla='test' | echo \$bla) && echo \$bla"
Test "head < /dev/null"
Test "cd /dev/null"
Test "echo /dev/full"
Test "cat /dev/null | head | grep a"
Test "(echo test | (cat | cat | echo test | ls | export EEE=lol) | export EEE='123' | echo \$EEE)"
Test ""

Test "''"
Test "(mkdir ttt0 ttt1 ttt1/ttt2 ; cd ttt0 ; cd ../ttt1/ttt2 ; rm -rf ../../ttt1 ; ls) ; ls"
Test "(mkdir ttt0 ttt1 ttt1/ttt2 ; cd ttt0 ; cd ../ttt1/ttt2 ; rm -rf ../../ttt1 ; mkdir) ; mkdir ok"
Test "(mkdir ttt0 ttt1 ttt1/ttt2 ; cd ttt0 ; cd ../ttt1/ttt2 ; rm -rf ../../ttt1)"
Test "echo '\$USER"\$PATH"'"
Test "echo '$USER"'$PATH'"'"
Test "echo \$USER"'$PATH'""
Test "echo "'$PATH'""
Test "echo '"\$PATH"'"
Test "export a=\"i_pipe_1 i_pipe_2 i_pipe_3\" && cat"
Test "export a=\"i_pipe_1 i_pipe_2 i_pipe_3\" && export b="$a" && cat"
Test "export a=\"i_pipe_1 i_pipe_2 i_pipe_3\" && export b='$a' && cat"
Test "export a=\"i_pipe_1 i_pipe_2 i_pipe_3\" || export b="$a" && cat"

Test "export bla='test' && echo \$b*a"
Test "*"
Test "cd *"
Test "**"
Test " *"
Test "* "
Test "* t5"
Test "* * *"
Test "ls **"
Test "ls ** "
Test "ls * *"
Test " ls * * "
Test "echo test"
Test "echo $"
Test "echo $?"
Test "echo test >/dev/full"
Test "echo test >/dev/null"
Test "ls > /dev/full"
Test "cat /dev/null"
Test "echo \"'$TERM'\""
Test "echo '\"$TERM\"'"
Test "echo $ *PATH"
Test "inexistant_command*"
Test "echo *"
Test "echo libft/*f*_st*chr.**"
Test "echo test < inexistant*input"
Test "export bla='test' && echo \$b*a"
Test "(export bla='test' | echo \$bla) && echo \$bla"
Test "head < /dev/null"
Test "cd /dev/nul*"
Test "echo /dev/f*ll"
Test "cat /dev/null | head | grep a"
Test "(echo test | (cat | cat | echo test | ls | export EEE=lol) | export EEE='123' | echo \$EEE)"
Test ""
Test "''"
Test "(mkdir ttt0 ttt1 ttt1/ttt2 ; cd ttt0 ; cd ../ttt1/ttt2 ; rm -rf ../../ttt* ; ls) ; ls"
Test "(mkdir ttt0 ttt1 ttt1/ttt2 ; cd ttt0 ; cd ../ttt1/ttt2 ; rm -rf ../../ttt* ; mkdir) ; mkdir ok"
Test "(mkdir ttt0 ttt1 ttt1/ttt2 ; cd ttt0 ; cd ../ttt1/ttt2 ; rm -rf ../../ttt*)"
Test "echo '\$USER"\$PATH"'"
Test "echo '$USER"'$PATH'"'"
Test "echo \$USER"'$PATH'""
Test "echo "'$PATH'""
Test "echo '"\$PATH"'"
Test "export a=\"i_pipe_1 i_pipe_2 i_pipe_3\" && cat"
Test "export a=\"i_pipe_1 i_pipe_2 i_pipe_3\" && export b="$a" && cat"
Test "export a=\"i_pipe_1 i_pipe_2 i_pipe_3\" && export b='$a' && cat"
Test "export a=\"i_pipe_1 i_pipe_2 i_pipe_3\" || export b="$a" && cat"

############################################################################
#                                                                          #
#                                                                          #
#                              ENV VARS TESTS                              #
#                                                                          #
#                                                                          #
############################################################################

Test "unset HOME"
Test "unset PWD"
Test "unset USER"
Test "unset HOME USER"
Test "unset HOME PWD"
Test "unset PATH"
Test "unset PATH PWD"
Test "unset PATH PWD USER HOME"
Test "unset PATH ; echo test"
Test "unset PATH ; cd test"
Test "unset PATH ; ls"
Test "unset PATH ; echo test"
Test "(cd .. ; unset PWD ; pwd)"
Test "unset HOME ; cd"
Test "unset PWD HOME OLDPWD ; cd -"
Test "unset PWD HOME OLDPWD ; cd .."
Test "unset PWD HOME OLDPWD ; cd"
Test "unset PWD HOME OLDPWD ; cd *"

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
Test "inexistant_command lol < test_files/i_pipe_4 | echo test "
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
Test "inexistant_command lol < test_files/i_pipe_4 && echo test"
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
Test "inexistant_command lol < test_files/i_pipe_4 ; echo test"
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
Test "inexistant_command lol < test_files/i_pipe_4 || echo test"
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
