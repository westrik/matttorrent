#include <stdio.h>
#include <stdlib.h>

#include "bencode.h"

int main(int argc, char**argv)
{
    FILE *torrentfile = NULL;

    if (argc == 2)
    {
        torrentfile = fopen(argv[1],"r");
        if (!torrentfile)
        {
            printf("Could not open '%s'", argv[1]);
        }
    }
    else
    {
        printf("Usage: %s torrent_file", argv[0]);
    }

    // Initialize 
    parse_torrent_file(torrentfile);
    fclose(torrentfile);

    // Get peers from tracker

    // Connect to peers, start downloading
        // verify each block

    // Finish, clean up
}
