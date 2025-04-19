#include "../minishell.h"

int pwd()
{
    char *c = getcwd(NULL, 0);
    printf("%s\n", c);
    free(c);
    return 0;
}
