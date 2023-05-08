git filter-branch --env-filter '
if [ "$GIT_AUTHOR_EMAIL" = "hateisse@student.42.fr" ] && [ "$GIT_AUTHOR_NAME" != "teisseire117" ];
then
    GIT_AUTHOR_EMAIL="hateisse@student.42.fr";
    GIT_AUTHOR_NAME="teisseire117";
fi
export GIT_AUTHOR_EMAIL;
export GIT_AUTHOR_NAME;
' --tag-name-filter cat -- --branches --tags