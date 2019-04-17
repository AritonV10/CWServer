#include "file.h"


char * no_chuncks_f(char * path, size_t * f_len) {
    FILE * _file;
    char * file_out;

    _file = fopen(path, READ);

    if(_file == NULL)
        return NULL; // stop the server

    if(fseek(_file, 0L, SEEK_END) == 0) {

        size_t bufsize = ftell(_file);

        if(bufsize == -1)
            return NULL;

        if((file_out = malloc(sizeof(char) * (bufsize + 1)))){

            if((fseek(_file, 0L, SEEK_SET)) == 0) {

                size_t _len = fread(file_out, sizeof(char), bufsize, _file);

                if(ferror(_file) != 0)
                    return NULL;
                else
                    file_out[_len] = '\0';
                fclose(_file);
                *f_len = bufsize;
                return file_out;

            }
        }

        return NULL;
    }

}



