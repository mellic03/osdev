#include <string.h>
#include <stdlib.h>

// char *strtok( char *str, const char *delim )
// {
//     // SYSLOG_BEGIN("strtok");
//     static char *strbase = nullptr;
//     static char *strbuf  = nullptr;
//     static char *tokbuf  = nullptr;

//     if (strbase == nullptr)
//     {
//         size_t nbytes = strlen(str) + 1;

//         strbase = (char*)malloc(nbytes);
//         strcpy(strbase, str);
//         strbuf = strbase;

//         tokbuf = (char*)malloc(nbytes);
//         memset(tokbuf, '\0', nbytes);
//     }


//     // buf = "mov rax, 0x15\nadd rax, 0x01";
//     // buf = "ov rax, 0x15\nadd rax, 0x01";
//     // buf = "v rax, 0x15\nadd rax, 0x01";
//     // buf = " rax, 0x15\nadd rax, 0x01";
//     //       " " <-- delim here
//     // buf = " rax, 0x15\nadd rax, 0x01";

//     char  *head = strbuf;
//     size_t len = 0;
//     size_t delim_len = strlen(delim);
    

//     while (*strbuf)
//     {
//         if (strncmp(strbuf, delim, delim_len) == 0)
//         {
//             memcpy(tokbuf, head, len+1);
//             tokbuf[len] = '\0';
//             strbuf++;
//             return tokbuf;
//         }

//         len++;
//         strbuf++;
//     }

//     // Reached end of strbuf
//     free(strbase);
//     strbase = nullptr;
//     strbuf  = nullptr;
//     tokbuf  = nullptr;

//     // SYSLOG_END();
//     return tokbuf;

// }


// char *strtok(char *s, const char *delim)
// {
//     static char *olds;
//     char *token;

//     if (s == NULL)
//         s = olds;

//     /* Scan leading delimiters.  */
//     s += strspn(s, delim);

//     /* if *s points to the null byte \0, that means
//         we have reached the end of the string and
//         we return NULL
//     */
//     if (*s == '\0')
//     {
//         olds = s;
//         return (NULL);
//     }

//     /* Find the end of the token.  */
//     token = s;
//     s = strpbrk(token, delim);
//     if (s == NULL)
//         /* This token finishes the string.  */
//         olds = strchr(token, '\0');
//     else
//     {
//         /* Terminate the token and make OLDS point past it.  */
//         *s = '\0';
//         olds = s + 1;
//     }
//     return (token);
// }

static char *olds;


char *reeeeeeee( const char *s, int c )
{
    for (int i=0; i<64; i++)
    {
      if (!s[i]) { return nullptr; }
        if (*s == (unsigned char)c)
        {
            return (char*)s;
        }
    }

    return nullptr;
}



char *strtok( char *s, const char *delim )
{
    char *token;
  
    if (s == NULL)
      s = olds;
  
    /* Scan leading delimiters.  */
    s += strspn (s, delim);
    if (*s == '\0')
      {
        olds = s;
        return NULL;
      }
  
    /* Find the end of the token.  */
    token = s;
    s = strpbrk (token, delim);
    if (s == NULL)
      /* This token finishes the string.  */
      olds = reeeeeeee(token, '\0');
    else
      {
        /* Terminate the token and make OLDS point past it.  */
        *s = '\0';
        olds = s + 1;
      }
    return token;
  }