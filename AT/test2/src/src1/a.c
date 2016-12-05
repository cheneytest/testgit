#include    <stdio.h>
#include    <unistd.h>
#include    <stdlib.h>
#include    <string.h>
#include    <fcntl.h>


#define    PUT_8_2(t,    s,    n)    ((t)    |=\
(((s)    &    0xC000)    >>    ((n)    +    0))    |\
(((s)    &    0x3000)    >>    ((n)    +    2))    |\
(((s)    &    0x0C00)    >>    ((n)    +    4))    |\
(((s)    &    0x0300)    >>    ((n)    +    6)))

#define    PUT_4_4(t,    s,    n)    ((t)    |=    ((((s)    &    0xC000)    >>    (n))    |    (((s)    &    0x3000)    >>    ((n)    +    6))))



int    main(int    argc,    char    *argv[])
{
        char    *input_file_name    =    NULL;
        char    *output_file_name    =    NULL;
        int    fd_in;
        int    fd_out;

        if    (argc    !=    3)    {
                printf("%s\n", argv[0]);
                return    0;
        }    else    {
                input_file_name    =    argv[1];
                output_file_name    =    argv[2];
        }

        fd_in    =    open(input_file_name,    O_RDONLY    |    O_BINARY,    0);

        if    (fd_in    ==    -1)    {
                printf("%s\n", input_file_name);
                return    1;
        }

        fd_out    =    open(output_file_name,    O_CREAT    |    O_TRUNC    |    O_WRONLY    |    O_BINARY,    0);

        if    (fd_out    ==    -1)    {
                printf("%s\n", output_file_name);
                return    1;
        }


        close(fd_in);
        close(fd_out);

        return    0;
}
