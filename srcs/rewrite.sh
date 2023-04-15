git filter-branch --env-filter '
if [ "$GIT_AUTHOR_EMAIL" = "hateisse@student.42.fr" ];
then
    GIT_AUTHOR_EMAIL="h.teisseire@hotmail.fr";
    GIT_AUTHOR_NAME="teisseire117";
fi
export GIT_AUTHOR_EMAIL;
export GIT_AUTHOR_NAME;
' --tag-name-filter cat -- --branches --tags