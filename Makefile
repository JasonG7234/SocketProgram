make: netclient.c
        gcc -g -Wall -Werror -fsanitize=address netclient.c -o netclient
server: netfileserver.c
        gcc -g -Wall -Werror -fsanitize=address netfileserver.c -o server
clean:
        rm server rm client
