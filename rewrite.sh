git filter-branch --env-filter -f '
if [ "$GIT_AUTHOR_EMAIL" = "oldemailhere" ] && [ "$GIT_AUTHOR_NAME" != "oldusernamehere" ];
then
    GIT_AUTHOR_EMAIL="newemailhere";
    GIT_AUTHOR_NAME="usernamehere";
fi
export GIT_AUTHOR_EMAIL;
export GIT_AUTHOR_NAME;
' --tag-name-filter cat -- --branches --tags
