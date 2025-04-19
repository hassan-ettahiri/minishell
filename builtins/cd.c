#include "../minishell.h"

int cd(char **str, int size)
{
    if(size > 1){
        printf("minishell: cd: too many arguments");
        return 1;
    }else if(size == 0)
    {
        chdir("/home/..");
        return 0;
    }
    if (chdir(str[0]) == 0)
        return 0;
    printf("cd: no such file or directory: %s\n", str[0]);
    return 1;
}