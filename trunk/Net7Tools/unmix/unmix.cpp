// unmix.cpp -- extracts all files from Westwood Studios .MIX file
//
// Written by VectoR.360
//            email: VectoR.360@gmail.com
// Placed in the Public Domain on 26-Oct-2004
//
// This is a simple utility for extracting all files from the
// Westwood Studios .MIX files for Earth & Beyond.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Forward references
void unmix_all();
int unmix(char *mixfile, char *outpath);
unsigned long grand_total = 0;

struct mix_entry
{
    unsigned long id;
    unsigned long offset;
    unsigned long size;
};

int main(int argc, char* argv[])
{
    char outpath[MAX_PATH] = {NULL};

    if (argc == 2)
    {
        // User wants to extract all files
        // This will take about 5 to 10 minutes
        if (strcmp(argv[1], "all") == 0)
        {
            unmix_all();
            return 0;
        }

        // Only the mix file was specified
        // Extract the filename without the path or extension
        char * p = argv[1];
        while (p-1)
        {
            strcpy(outpath, p);
            p = strstr(p+1, "\\")+1;
        }
        strtok(outpath, ".");
    }
    else if (argc == 3)
    {
        // The mix file and the destination path was specified
        strcpy(outpath, argv[2]);
    }
    else
    {
        printf("Usage:\n\n");
        printf("  unmix MixFile.mix [OutPath]\n\n");
        printf("  where MixFile.mix is the name of the .MIX file and OutPath is an optional\n");
        printf("  output folder name.  If OutPath is not specified, a folder with the same\n");
        printf("  name as the filename (less the .MIX extension will be used.  If the output\n");
        printf("  folder does not exist, it will be created\n\n");
        printf("Examples:\n\n");
        printf("To extract all files from mixfile_art_01.mix to the 'mixfile_art_01' folder\n");
        printf("  unmix mixfile_art_01.mix\n\n");
        printf("To extract all files from mixfile_art_01.mix to the 'Art' folder:\n");
        printf("  unmix mixfile_art_01.mix Art\n\n");
        printf("To extract all files from all mix files:\n");
        printf("  unmix all\n");
        printf("  (Note: this will create three folders: Art, Sound, and Talktree)\n");
        return 0;
    }

    // Create the output directory - ignore errors
    CreateDirectory(outpath, NULL);

    // Extract all files from the mix file
    if (unmix(argv[1], outpath))
    {
        printf("Unmix encountered an error while processing the MIX file\n");
    }

    return 0;
}

void unmix_all()
{
    WIN32_FIND_DATA fdata = {NULL};
    HANDLE h = FindFirstFile("*.*", &fdata);

    // Loop through the list of all files in the folder
    do
    {
        // Don't process folders.  Process only .MIX files.
        char *filename = fdata.cFileName;
        if (((fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) &&
            (strstr(filename, ".mix")))
        {
            // Determine the folder where we will put the files
            char *outpath = "Data";
            if (strstr(filename, "art"))
            {
                outpath = "Art";
            }
            else if (strstr(filename, "sound"))
            {
                outpath = "Sound";
            }
            else if (strstr(filename, "talktree"))
            {
                outpath = "Talktree";
            }

            // Create the output directory - ignore errors
            CreateDirectory(outpath, NULL);

            // Now extract the files from this mix file
            if (unmix(filename, outpath))
            {
                printf("Unmix encountered an error while processing '%s'\n", filename);
            }
        }
    } while (FindNextFile(h, &fdata));

    printf("Grand total of %d files were extracted\n", grand_total);
}

// Returns 1 if an error was detected but no error message was printed
int unmix(char *mixfile, char *outpath)
{
    long  extracted = 0;
    char  signature[4] = {NULL};
    long  entry_offset = 0;
    long  string_offset = 0;
    long  num_entries = 0;
    long  num_filenames = 0;
    long  body_size = 0;
    unsigned char length = 0;
    char filename[256];
    char filepath[256];

    // Open the mix file for read access
    FILE *f = fopen(mixfile, "rb");
    if (!f)
    {
        printf("Unable to open mixfile \"%s\"\n", mixfile);
        return 0; // no need to print an error message
    }

    // Read the signature
    if (fread(signature, 1, 4, f) != 4)
    {
        fclose(f);
        printf("This is not a MIX file: '%s'\n", mixfile);
        return 0; // no need to print an error message
    }

    // First four bytes contain the MIX1 signature
    if (strncmp(signature, "MIX1", 4) != 0)
    {
        fclose(f);
        printf("This is not a MIX file: '%s'\n", mixfile);
        return 0; // no need to print an error message
    }

    printf("Extracting files from '%s'\n", mixfile);

    // Next four bytes contains the offset to the table of entries
    if (fread(&entry_offset, 4, 1, f) != 1)
    {
        fclose(f);
        return 1;
    }

    // Next four bytes contains the offset to the strings
    if (fread(&string_offset, 4, 1, f) != 1)
    {
        fclose(f);
        return 1;
    }

    // Move to the table of entries
    if (fseek(f, entry_offset, SEEK_SET) != 0)
    {
        fclose(f);
        return 1;
    }

    // First four bytes contains the number of entries in the table
    if (fread(&num_entries, 4, 1, f) != 1)
    {
        fclose(f);
        return 1;
    }

    // Fail if no entries
    if (num_entries == 0)
    {
        fclose(f);
        return 1;
    }

    // Allocate memory for the entry table
    mix_entry * entries = new mix_entry[num_entries];
    if (!entries)
    {
        fclose(f);
        return 1;
    }
    memset(entries, NULL, sizeof(mix_entry) * num_entries);

    // Load the entire entry table into memory
    if (fread(entries, sizeof(mix_entry), num_entries, f) != (unsigned int) num_entries)
    {
        fclose(f);
        delete entries;
        return 1;
    }

    // Now move to the list filenames
    if (fseek(f, string_offset, SEEK_SET) != 0)
    {
        fclose(f);
        delete entries;
        return 1;
    }

    // The first four bytes contains the number of filenames
    if (fread(&num_filenames, 4, 1, f) != 1)
    {
        fclose(f);
        delete entries;
        return 1;
    }

    // Fail if no strings
    if (num_filenames == 0)
    {
        fclose(f);
        delete entries;
        return 1;
    }

    // The number of strings (filenames) must equal the number of entries
    if (num_filenames != num_entries)
    {
        fclose(f);
        delete entries;
        return 1;
    }

    // Process each file listed in the mix file
    for (int i=0; i<num_filenames; i++)
    {
        // Read the length byte
        if (fread(&length, 1, 1, f) != 1)
        {
            fclose(f);
            delete entries;
            return 1;
        }

        // Read the string containing the filename
        // Note that the string is null terminated in the file
        if (fread(filename, 1, length, f) != length)
        {
            fclose(f);
            delete entries;
            return 1;
        }

        // Display the file information
        // printf("%4d : %08X : %08x : %8d : %s\n", i+1, entries[i].id, entries[i].offset, entries[i].size, filename);
        printf("%d %s\n", i+1, filename);

        // Determine the output file name
        sprintf(filepath, "%s\\%s", outpath, filename);

        if (entries[i].size == 0)
        {
            // Don't bother to create empty files
            printf("Warning: '%s' is empty!\n", filename);
        }
        else
        {
            // Save the position of the next filename
            long pos = ftell(f);

            // Locate the data to be read
            if (fseek(f, entries[i].offset, SEEK_SET) != 0)
            {
                fclose(f);
                delete entries;
                return 1;
            }

            // Allocate space for the data
            unsigned char *data = new unsigned char[entries[i].size];

            // Read the entire block of data
            if (fread(data, 1, entries[i].size, f) != (unsigned int) entries[i].size)
            {
                fclose(f);
                delete entries;
                delete data;
                return 1;
            }

            // Write the data to the output file
            FILE *outf = fopen(filepath, "wb");
            if (outf)
            {
                fwrite(data, entries[i].size, 1, outf);
                fclose(outf);
                extracted++;
            }
            else
            {
                printf("Unable to open '%s' for output\n");
            }
            delete data;

            // Move back into position to read the next filename
            if (fseek(f, pos, SEEK_SET) != 0)
            {
                fclose(f);
                delete entries;
                return 1;
            }
        }
    }

    // Done!  Close the file and print a summary
    fclose(f);
    delete entries;
    printf("Extracted %d files\n", extracted);
    grand_total += extracted;
	return 0;
}
